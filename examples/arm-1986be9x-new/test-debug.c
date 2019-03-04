/*
 * Проверка отладочной печати.
 * Вывод производится на UART2 на скорости 115200 бод.
 */
#include <runtime/lib.h>

int main (void)
{
	for (;;) {
		debug_puts ("Hello, World!\n");
		debug_getchar();
	}
}
