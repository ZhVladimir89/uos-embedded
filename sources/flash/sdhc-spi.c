//
// Драйвер карт SDHC поверх SPI
//

//
// Особенности реализации.
//
// Разные карты памяти имеют разное время ответа на ту или иную команду.
// По стандарту это время варьируется от 1 до 8 байтов на шине SPI.
// Таким образом после подачи команды необходимо опрашивать драйвер
// SPI, чтобы получить статус выполнения команды.
// Времена ответа на команды манипуляции с данными (чтение, запись) 
// более непредсказуемые.
// Самый простой простой алгоритм ожидания заключается в циклическом
// чтении одного байта из SPI до получения ответа. Как правило, этот
// алгоритм неэффективен с точки зрения использования драйвера и
// аппаратуры SPI. Но в некоторых случаях он реализуется в этом 
// драйвере - в ожидании статусов операций чтения, стирания и записи, 
// поскольку время этих операций не зависит от количества считанных 
// байтов по SPI.
// При ответе на другие команды время не может превышать длительности 
// передачи 8 байтов, поэтому нужно считать эти байты как можно быстрее.
// Для этого выделяется достаточный буфер в памяти и затем ответ 
// ищется в считанных за один приём байтах. (Но для операций с данными,
// в свою очередь, этот приём оказывается неэффективен, поскольку
// чтение большого числа байтов само по себе занимает значительное
// время - она выполняется последовательно на частоте SPI, поэтому
// статистически быстрее принимать по байту и сразу их анализировать,
// т.е. эффективнее становится первый алгоритм).
// Иногда эти методы приходится совмещать, поскольку за командами могут
// следовать блоки данных и т.д. Всё это вносит определённые сложности
// в код драйвера. Например, в функции wait_data_token сначала
// анализируется байты, пришедшие в ответ на команду, поскольку они
// помимо ответа уже могут содержать, например, маркер начала блока 
// данных, а затем, если маркер не найден, драйвер SDHC дожидается его
// с помощью побайтного чтения.
//
// Для ускорения операций чтения и записи по последовательным адресам
// используется многоблочные чтение и запись. В эти режимы карта
// карта вводится с помощью специальной команды. В таком режиме
// карта может выполнять только чтение или только запись. Для 
// выполнения других операций нужно сначала вывести карту в нормальный
// режим работы. Текущий режим карты запоминается в переменной state.
// 

#include <runtime/lib.h>
#include <kernel/uos.h>
#include <flash/sdhc-spi.h>
#include <flash/sd-private.h>

#define OP_WRITE    0       // Операция записи
#define OP_READ     1       // Операция чтения

// Используемые маркеры данных
#define START_BLOCK_TOKEN           0xFE    // Начало блока данных
#define MULTI_START_BLOCK_TOKEN     0xFC    // Начало блока данных
                                            // при многоблочной передаче
#define STOP_TRAN_TOKEN             0xFD    // Требование прекращения   
                                            // многоблочной передачи

// Маркеры ответов от карты
#define DATA_RESPONSE_MASK          0x1F    // Маска используемых битов
#define DATA_ACCEPTED               0x05    // Данные приняты
#define DATA_CRC_ERROR              0x0B    // Ошибка CRC
#define DATA_WRITE_ERROR            0x0D    // Ошибка записи во флеш

//
// Ожидание начала блока данных от карты.
//
// См. Особенности реализации.
// start - указатель на начало "хвоста" ответа на предыдущую команду,
// может быть равен 0, если "хвоста" нет. В этом же параметре 
// возвращается указатель на маркер начала блока данных, если он найден,
// или 0, в противном случае.
//
static int
wait_data_token(sdhc_spi_t *m, uint8_t **start)
{
    // Ищем маркер в "хвосте", если "хвост" есть
    if (*start != 0) {
        while (*start < m->databuf + m->msg.word_count) {
            if (**start == START_BLOCK_TOKEN)
                return FLASH_ERR_OK;    // Найден маркер
            ++(*start);
        }
        *start = 0; // Не найден маркер
    }
    // Ищем маркер путём побайтного чтения из SPI
    m->msg.word_count = 1;
    do {
        m->databuf[0] = 0xFF;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) 
            return FLASH_ERR_IO;
    } while (m->databuf[0] != START_BLOCK_TOKEN);
    return FLASH_ERR_OK;
}

//
// Ожидание освобождения линии.
//
// Функция дожидается первого байта 0xFF.
//
static int
wait_line_free(sdhc_spi_t *m)
{
    m->msg.word_count = 1;
    do {
        m->databuf[0] = 0xFF;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) 
            return FLASH_ERR_IO;
    } while (m->databuf[0] == 0);
    return FLASH_ERR_OK;
}

//
// Отправка команды и поиск ответа.
//
// В r1 возвращается указатель на ответ от карты.
//
static int send_command(sdhc_spi_t *m, uint8_t **r1)
{
	int i;
/*
debug_printf("command:  ");
for (i = 0; i < m->msg.word_count; ++i)
	debug_printf("%02X ", m->databuf[i]);
debug_printf("\n");
*/
	if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;
/*
debug_printf("response: ");
for (i = 0; i < m->msg.word_count; ++i)
	debug_printf("%02X ", m->databuf[i]);
debug_printf("\n\n");    
*/
    for (i = 7; i < m->msg.word_count; ++i) {
        if (m->databuf[i] != 0xFF) {
            *r1 = &m->databuf[i];
            break;
        }
    }

    if (i == m->msg.word_count)
        return FLASH_ERR_BAD_ANSWER;

    return FLASH_ERR_OK;
}

//
// Вспомогательная функция копирования данных с изменением 
// порядка следования байт на обратный (при передаче по SPI используется
// формат big-endian, а драйвер рассчитан на архитектуру процессора
// little-endian.
//
static void reverse_copy(uint8_t *dst, uint8_t *src, unsigned size)
{
    unsigned i, j;
    for (i = 0, j = size - 1; i < size; ++i, --j)
        dst[i] = src[j];
}

//
// Определение наличия подключенной карты и её параметров.
//
static int sd_connect(flashif_t *flash)
{
	int res;
	uint8_t *r1 = 0;
	
    sdhc_spi_t *m = (sdhc_spi_t *) flash;
    mutex_lock(&flash->lock);
    
    memset(m->databuf, 0xFF, sizeof(m->databuf));
    
    m->state = 0;

    // Initial clock to activate SD controller
    m->msg.freq = 400000;
    m->msg.mode |= SPI_MODE_CS_HIGH | SPI_MODE_CS_HOLD;
    m->msg.tx_data = m->databuf;
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 10;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    // Switch SD to SPI mode    
    int nb_try = 100;
	do {
		nb_try--;
		if (nb_try <= 0) {
            mutex_unlock(&flash->lock);
			return FLASH_ERR_NOT_CONN;
        }

		m->databuf[0] = CMD_GO_IDLE_STATE;
		m->databuf[1] = 0x00;
		m->databuf[2] = 0x00;
		m->databuf[3] = 0x00;
		m->databuf[4] = 0x00;
		m->databuf[5] = 0x95;
		m->msg.mode &= ~(SPI_MODE_CS_HIGH | SPI_MODE_CS_HOLD);
		m->msg.word_count = 16; // cmd + max Ncr + r1 + 1byte spare
		
		res = send_command(m, &r1);
		if (res == FLASH_ERR_BAD_ANSWER) {
			continue;
		} else if (res != FLASH_ERR_OK) {
			mutex_unlock(&flash->lock);
			return res;
		}
    } while (*r1 != IN_IDLE_STATE);
    
    // Checking SD version
    m->msg.word_count = 20;
    m->databuf[0] = CMD_SEND_IF_COND;
    m->databuf[1] = 0x00;
    m->databuf[2] = 0x00;
    m->databuf[3] = 0x01;
    m->databuf[4] = 0xAA;
    m->databuf[5] = 0x87;
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    if (*(r1 + 4) != 0xAA)
    {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_NOT_CONN;
    }

    if (*r1 != IN_IDLE_STATE) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_NOT_SUPP;
    }
    
    memset(m->databuf, 0xFF, 20);

    // Initialize SD card
    m->msg.word_count = 16;
    while (1) {
        m->databuf[0] = CMD_APP_CMD;
        m->databuf[1] = 0x00;
        m->databuf[2] = 0x00;
        m->databuf[3] = 0x00;
        m->databuf[4] = 0x00;
        m->databuf[5] = 0xFF;
		res = send_command(m, &r1);
		if (res != FLASH_ERR_OK) {
			mutex_unlock(&flash->lock);
			return res;
		}
		if (*r1 & ERROR_MASK) {
			mutex_unlock(&flash->lock);
			return FLASH_ERR_NOT_SUPP;
		}

        m->databuf[0] = ACMD_SD_SEND_OP_COND;
        m->databuf[1] = 0x40;
        m->databuf[2] = 0x00;
        m->databuf[3] = 0x00;
        m->databuf[4] = 0x00;
        m->databuf[5] = 0xFF;

		res = send_command(m, &r1);
		if (res != FLASH_ERR_OK) {
			mutex_unlock(&flash->lock);
			return res;
		}
		if (*r1 == SD_READY)
			break;
    }
    
    // Checking if the card is SDHC (or SDXC)
    m->msg.word_count = 20;
    m->databuf[0] = CMD_READ_OCR;
    m->databuf[1] = 0x00;
    m->databuf[2] = 0x00;
    m->databuf[3] = 0x00;
    m->databuf[4] = 0x00;
    m->databuf[5] = 0xFF;
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    if (*r1 != SD_READY) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_BAD_ANSWER;
    }
    if (!(*(r1 + 1) & 0x40)) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_NOT_SUPP;
    }
    
    m->msg.freq = 25000000;
    
    // Read CSD for card parameters
    m->msg.word_count = 40;
    memset(m->databuf, 0xFF, 40);
	m->databuf[0] = CMD_SEND_CSD;
	m->databuf[1] = 0x00;
	m->databuf[2] = 0x00;
	m->databuf[3] = 0x00;
	m->databuf[4] = 0x00;
	res = send_command(m, &r1);
	if (res != FLASH_ERR_OK) {
		mutex_unlock(&flash->lock);
		return res;
	}
	if (*r1 != SD_READY) {
		mutex_unlock(&flash->lock);
		return FLASH_ERR_BAD_ANSWER;
	}
    
    ++r1;
    res = wait_data_token(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    ++r1;
    
    csd_v2_t csd_v2;
    reverse_copy((uint8_t *) &csd_v2, r1, sizeof(csd_v2_t));
    flash->page_size = 512;
    flash->nb_pages_in_sector = 4 * 1024 * 1024 / 512;
    flash->nb_sectors = (csd_v2.c_size + 1) / 8;

    // Switching to High Speed mode
    memset(m->databuf, 0xFF, 16);
    m->msg.word_count = 16;
	m->databuf[0] = CMD_SWITCH_FUNC;
	m->databuf[1] = 0x80;
	m->databuf[4] = 0xF1;
	res = send_command(m, &r1);
	if (res != FLASH_ERR_OK) {
		mutex_unlock(&flash->lock);
		return res;
	}
	if (*r1 != SD_READY) {
		mutex_unlock(&flash->lock);
		return FLASH_ERR_BAD_ANSWER;
	}
    
    ++r1;
    res = wait_data_token(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    
    int offset = 0;
    if (r1) offset = r1 - m->databuf + m->msg.word_count;
    
    m->msg.word_count = sizeof(m->databuf);
    memset(m->databuf, 0xFF, sizeof(m->databuf));
	if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }
    
    int high_speed = ((m->databuf[16 - offset] & 0xF) == 1);
    
    memset(m->databuf, 0xFF, sizeof(m->databuf));
	if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    // В случае, если удалось перейти в режим High Speed, увеличиваем
    // частоту передач в SPI до 50 Мгц.
    if (high_speed) m->msg.freq = 50000000;

    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

//
// Останов мультиблочного чтения.
// Выполняется с помощью специальной команды STOP TRANSMISSION.
//
static int
stop_multiple_read(sdhc_spi_t *m)
{
    int res;
    uint8_t *r1 = 0;
    
    memset(m->databuf, 0xFF, 16);
    
    m->databuf[0] = CMD_STOP_TRANSMISSION;
    m->msg.word_count = 16;
    
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK)
        return res;
    if (*r1 & ERROR_MASK)
        return FLASH_ERR_IO;
    return FLASH_ERR_OK;
}

//
// Останов мультиблочной записи.
// Выполняется с помощью маркера STOP TRAN TOKEN.
//
static int
stop_multiple_write(sdhc_spi_t *m)
{
    int res;
    res = wait_line_free(m);
    if (res != FLASH_ERR_OK)
        return res;
    
    m->msg.word_count = 2;
    m->databuf[0] = STOP_TRAN_TOKEN;
    m->databuf[1] = 0xFF;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;
    
    // Дожидаемся, когда карта освободит линию (т.е. снимет
    // постоянный 0 с линии MISO)
    m->msg.word_count = 1;
    do {
        m->databuf[0] = 0xFF;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) 
            return FLASH_ERR_IO;
    } while (m->databuf[0] == 0);
    
    return FLASH_ERR_OK;
}

//
// Запуск мультиблочной записи или мультиблочного чтения.
// page_num - номер начальной страницы
// read - если != 0, то запускается операция чтения, == 0 - запись
// В pr1 возвращается указатель на ответ от карты в считанных байтах
// 
static int
init_multiple_op(sdhc_spi_t *m, unsigned page_num, int read, uint8_t **pr1)
{
    int res;
    uint8_t *r1 = 0;
    
    memset(m->databuf, 0xFF, 16);
    if (read)
        m->databuf[0] = CMD_READ_MULTIPLE_BLOCK;
    else m->databuf[0] = CMD_WRITE_MULTIPLE_BLOCK;
    reverse_copy(&m->databuf[1], (uint8_t*)&page_num, 4);
    m->msg.word_count = 16;
    
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) return res;

    if (*r1 & ERROR_MASK) return FLASH_ERR_IO;
    
    if (pr1) *pr1 = r1;

    return FLASH_ERR_OK;
}

//
// Чтение одной страницы данных (в режиме мультиблочного чтения).
// data - указатель на буфер, в который следует положить принятые данные
// size - размер буфера
// existing_data - если не 0, то содержит указатель на уже принятую 
// часть данных (она лежит в буфере databuf).
//
static int
do_read_one_page(sdhc_spi_t *m, void *data, unsigned size, uint8_t *existing_data)
{
    int offset = 0;

    // Перенос уже принятой части данных в буфер data
    if (existing_data) {
        offset = existing_data - m->databuf + m->msg.word_count;
        memcpy(data, existing_data, offset);
        data = (uint8_t *)data + offset;
    }
    // Приём оставшихся данных с учётом их длины
    m->msg.rx_data = data;
    m->msg.tx_data = data;
    m->msg.word_count = size - offset;
    memset(data, 0xFF, m->msg.word_count);
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;
    
    // Обмен с картой всегда происходит блоками по 512 байт.
    // Даже оставшиеся данные не нужны (size < 512), всё равно
    // их нужно дочитать из SPI - в вспомогательный буфер databuf.
    m->msg.rx_data = 0;
    m->msg.tx_data = m->databuf;
    size = flash_page_size((flashif_t*)m) - size + 3;
    
    memset(m->databuf, 0xFF, sizeof(m->databuf));
    do {
        m->msg.word_count = (sizeof(m->databuf) < size) ?
            sizeof(m->databuf) : size;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
            return FLASH_ERR_IO;
        size -= m->msg.word_count;
    } while (size > 0);
    
    m->msg.rx_data = m->databuf;
        
    return FLASH_ERR_OK;
}

//
// Запись одной страницы данных (в режиме мультиблочной записи).
// data - указатель на буфер с данными
// size - размер буфера
//
static int
do_write_one_page(sdhc_spi_t *m, void *data, unsigned size)
{
    int res;
    
    // Дожидаемся освобождения линии
    res = wait_line_free(m);
    if (res != FLASH_ERR_OK)
        return res;
    
    // Отправляем маркер начала данных
    m->msg.word_count = 1;
    m->databuf[0] = MULTI_START_BLOCK_TOKEN;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
        return FLASH_ERR_IO;

    // Выдаём требуемый размер данных
    m->msg.rx_data = 0;
    m->msg.tx_data = data;
    m->msg.word_count = size;
    if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
        return FLASH_ERR_IO;
    }
    
    // Обмен с картой всегда происходит блоками по 512 байт.
    // Поэтому оставшиеся байты всё равно необходимо передать. Для этого
    // используем вспомогательный буфер databuf
    size = flash_page_size((flashif_t*)m) - size;
    m->msg.tx_data = m->databuf;
    if (size > 0) memset(m->databuf, 0xFF, sizeof(m->databuf));
    while (size > 0) {
        m->msg.word_count = (sizeof(m->databuf) < size) ?
            sizeof(m->databuf) : size;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK)
            return FLASH_ERR_IO;
        size -= m->msg.word_count;
    };
    
    // Дожидаемся ответа со статусом завершения операции
    m->msg.rx_data = m->databuf;
    m->msg.word_count = 1;
    uint8_t token;
    do {
        m->databuf[0] = 0xFF;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) 
            return FLASH_ERR_IO;
        token = m->databuf[0] & DATA_RESPONSE_MASK;
    } while (token != DATA_ACCEPTED && token != DATA_WRITE_ERROR);
    
    if (token == DATA_WRITE_ERROR)
        return FLASH_ERR_IO;
        
    return FLASH_ERR_OK;
}

//
// Операция стирания произвольного числа страниц памяти.
// start_page - номер первой страницы, которую нужно стереть
// end_page - номер последней страницы для стирания
//
static int
sd_erase(flashif_t *flash, unsigned start_page, unsigned end_page)
{
    int res;
    sdhc_spi_t *m = (sdhc_spi_t *) flash;
    uint8_t *r1 = 0;
    
    mutex_lock(&flash->lock);
    
    // Карта может находиться в состоянии многоблочной операции,
    // в этом случае её нужно вернуть в нормальный режим
    if (m->state == SDHC_STATE_MULTIWRITE)
        stop_multiple_write(m);
    if (m->state == SDHC_STATE_MULTIREAD)
        stop_multiple_read(m);
    m->state = SDHC_STATE_IDLE;

    // Установка номера первой страницы
    m->msg.word_count = 16;
    memset(m->databuf, 0xFF, 16);
    m->databuf[0] = CMD_ERASE_WR_BLK_START_ADDR;
    reverse_copy(&m->databuf[1], (uint8_t*)&start_page, 4);
    
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    if (*r1 & ERROR_MASK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }
    
    // Установка номера последней страницы
    memset(m->databuf, 0xFF, 16);
    m->databuf[0] = CMD_ERASE_WR_BLK_END_ADDR;
    reverse_copy(&m->databuf[1], (uint8_t*)&end_page, 4);
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) {
        mutex_unlock(&flash->lock);
        return res;
    }
    if (*r1 & ERROR_MASK) {
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }
    
    // Стирание
    memset(m->databuf, 0xFF, 16);
    m->databuf[0] = CMD_ERASE;
    m->msg.mode |= SPI_MODE_CS_HOLD;
    res = send_command(m, &r1);
    if (res != FLASH_ERR_OK) {
        m->msg.mode &= ~SPI_MODE_CS_HOLD;
        mutex_unlock(&flash->lock);
        return res;
    }
    if (*r1 & ERROR_MASK) {
        m->msg.mode &= ~SPI_MODE_CS_HOLD;
        mutex_unlock(&flash->lock);
        return FLASH_ERR_IO;
    }

    // Ожидание завершения стирания (снятия постоянного 0 с MISO)
    int cnt = 0;
    m->msg.word_count = 1;
    do {
        m->databuf[0] = 0xFF;
        if (spim_trx(m->spi, &m->msg) != SPI_ERR_OK) {
            m->msg.mode &= ~SPI_MODE_CS_HOLD;
            mutex_unlock(&flash->lock);
            return FLASH_ERR_IO;
        }
        cnt++;
    } while (m->databuf[0] == 0);
    
    m->msg.mode &= ~SPI_MODE_CS_HOLD;
    mutex_unlock(&flash->lock);
    return FLASH_ERR_OK;
}

//
// Интерфейсная функция стирания всей карты
//
static int 
sd_erase_all(flashif_t *flash)
{
    return sd_erase(flash, 0, 
        flash_nb_sectors(flash) * flash_nb_pages_in_sector(flash) - 1);
}

//
// Интерфейсная функция стирания заданных секторов флеш
//
static int 
sd_erase_sectors(flashif_t *flash, unsigned sector_num, 
    unsigned nb_sectors)
{
    return sd_erase(flash, sector_num * flash_nb_pages_in_sector(flash),
        (sector_num + nb_sectors) * flash_nb_pages_in_sector(flash) - 1);
}

//
// Интерфейсная функция чтения данных.
// size может быть больше, чем размер страницы, и функция должна это
// учитывать.
//
static int 
sd_read(flashif_t *flash, unsigned page_num, unsigned offset, 
        void *data, unsigned size)
{
    int res = FLASH_ERR_OK;
    sdhc_spi_t *m = (sdhc_spi_t *) flash;
    int exit = 0;
    uint8_t *r1 = 0;
    uint8_t *cur_ptr = data;
    unsigned cur_size;
    
    mutex_lock(&flash->lock);
    do {
        switch (m->state) {
        case SDHC_STATE_MULTIWRITE:
            // Карта сейчас в режиме многоблочной записи, переводим
            // её сначала в нормальный режим
            res = stop_multiple_write(m);
            if (res != FLASH_ERR_OK) {
                exit = 1;
                break;
            }
            m->state = SDHC_STATE_IDLE;
            // Intentionally no break
            
        case SDHC_STATE_IDLE:
            // Инициируем режим многоблочного чтения
            res = init_multiple_op(m, page_num, OP_READ, &r1);
            if (res != FLASH_ERR_OK) {
                exit = 1;
                break;
            }
            ++r1;
            m->state = SDHC_STATE_MULTIREAD;
            m->next_page = page_num;
            // Intentionally no break
            
        case SDHC_STATE_MULTIREAD:
            if (page_num == m->next_page) {
                // Карта в режиме многоблочного чтения и требуемый
                // номер страницы является последовательным по отношению
                // к предыдущему - выполняем приём очередного блока.
                // Дожидаемся начала блока.
                res = wait_data_token(m, &r1);
                if (res != FLASH_ERR_OK) {
                    exit = 1;
                    break;
                }
                do {
                    // Выполняем чтение одной страницы
                    cur_size = (size <= flash_page_size(flash)) ? 
                        size : flash_page_size(flash);
                    res = do_read_one_page(m, cur_ptr, cur_size, r1);
                    if (res != FLASH_ERR_OK) 
                        break;
                    m->next_page++;
                    cur_ptr += cur_size;
                    size -= cur_size;
                } while (size > 0);
                exit = 1;
            } else {
                // Карта в нужном режиме, но адрес не последовательный.
                // Останавливаем режим многоблочного чтения, чтобы
                // потом заново его инициировать, но уже с нужным
                // номером страницы.
                res = stop_multiple_read(m);
                if (res != FLASH_ERR_OK) {
                    exit = 1;
                    break;
                }
                m->state = SDHC_STATE_IDLE;
            }
        break;
        }
    } while (!exit);
        
    mutex_unlock(&flash->lock);
    return res;
}

//
// Интерфейсная функция записи данных.
// size может быть больше, чем размер страницы, и функция должна это
// учитывать.
//
static int 
sd_write(flashif_t *flash, unsigned page_num, unsigned offset,
    void *data, unsigned size)
{
    int res = FLASH_ERR_OK;
    sdhc_spi_t *m = (sdhc_spi_t *) flash;
    int exit = 0;
    uint8_t *cur_ptr = data;
    unsigned cur_size;

    mutex_lock(&flash->lock);

    do {
        switch (m->state) {
        case SDHC_STATE_MULTIREAD:
            // Карта сейчас в режиме многоблочного чтения, переводим
            // её сначала в нормальный режим
            res = stop_multiple_read(m);
            if (res != FLASH_ERR_OK) {
                exit = 1;
                break;
            }
            m->state = SDHC_STATE_IDLE;
            // Intentionally no break
            
        case SDHC_STATE_IDLE:
            // Инициируем режим многоблочной записи
            res = init_multiple_op(m, page_num, OP_WRITE, 0);
            if (res != FLASH_ERR_OK) {
                exit = 1;
                break;
            }
            m->state = SDHC_STATE_MULTIWRITE;
            m->next_page = page_num;
            // Intentionally no break
            
        case SDHC_STATE_MULTIWRITE:
            if (page_num == m->next_page) {
                // Карта в режиме многоблочной записи и требуемый
                // номер страницы является последовательным по отношению
                // к предыдущему - выполняем передачу очередного блока.
                // Дожидаемся начала блока.
                do {
                    // Выполняем запись одной страницы
                    cur_size = (size <= flash_page_size(flash)) ? 
                        size : flash_page_size(flash);
                    res = do_write_one_page(m, cur_ptr, cur_size);
                    if (res != FLASH_ERR_OK)
                        break;
                    m->next_page++;
                    cur_ptr += cur_size;
                    size -= cur_size;
                } while (size > 0);
                exit = 1;
            } else {
                // Карта в нужном режиме, но адрес не последовательный.
                // Останавливаем режим многоблочной записи, чтобы
                // потом заново его инициировать, но уже с нужным
                // номером страницы.
                res = stop_multiple_write(m);
                if (res != FLASH_ERR_OK) {
                    exit = 1;
                    break;
                }
                m->state = SDHC_STATE_IDLE;
            }
        break;
        }
    } while (!exit);

    mutex_unlock(&flash->lock);
    return res;

}

//
// Корректное завершение всех операций записи и чтения.
//
static int 
sd_flush(flashif_t *flash)
{
    int res = FLASH_ERR_OK;
    sdhc_spi_t *m = (sdhc_spi_t *) flash;

    mutex_lock(&flash->lock);

    switch (m->state) {
    case SDHC_STATE_MULTIREAD:
        // Карта сейчас в режиме многоблочного чтения, переводим
        // её сначала в нормальный режим
        res = stop_multiple_read(m);
        if (res != FLASH_ERR_OK)
            break;
        m->state = SDHC_STATE_IDLE;
    break;
        
    case SDHC_STATE_MULTIWRITE:
        res = stop_multiple_write(m);
        if (res != FLASH_ERR_OK)
            break;
        m->state = SDHC_STATE_IDLE;
    break;
    }

    mutex_unlock(&flash->lock);
    return res;
}

int sd_no_exp_erase(flashif_t *flash)
{
    return 0;
}

void sd_spi_init(sdhc_spi_t *m, spimif_t *s, unsigned mode)
{
    m->spi = s;
    flashif_t *f = &m->flashif;

    f->connect = sd_connect;
    f->erase_all = sd_erase_all;
    f->erase_sectors = sd_erase_sectors;
    f->write = sd_write;
    f->read = sd_read;
    f->flush = sd_flush;
    f->needs_explicit_erase = sd_no_exp_erase;

    m->msg.mode = (mode & 0xFF07) | SPI_MODE_NB_BITS(8);
}
