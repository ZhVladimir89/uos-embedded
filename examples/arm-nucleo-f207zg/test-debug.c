/*
 * Проверка отладочной печати.
 */
#include <runtime/lib.h>

int main (void)
{
	for (;;) {
		debug_puts ("Hello, World!\n");
		debug_getchar();
	}
}
