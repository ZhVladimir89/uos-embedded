/*
 * Five dining pfilosophers.
 */
#include <runtime/lib.h>
#include <kernel/uos.h>
#include <timer/timer.h>
#include <uart/uart.h>
#include <random/rand15.h>

#define N	5			/* Number of philosophers */

#define uart debug			/* Console driver */
timer_t timer;				/* Timer driver */
ARRAY (task [N], 1000);			/* Task space */
mutex_t fork [N];			/* Forks */
mutex_t table;				/* Broadcast communication */
mutex_t screen;				/* Access to the screen */

typedef enum {
	THINKING, EATING, WAITING,
} state_t;

void display (int i, state_t state)
{
	static const int xpos [] = { 35, 54, 47, 23, 16 };
	static const int ypos [] = {  3, 10, 20, 20, 10 };
	static int initialized = 0;

	mutex_lock (&screen);
	if (! initialized) {
		puts (&uart, "\033[2J");			/* Clear screen. */
		initialized = 1;
	}
	printf (&uart, "\033[%d;%dH", ypos [i], xpos [i]);
	switch (state) {
	case THINKING:
		printf (&uart, "\033[1;34m%d: thinking", i+1);	/* Light blue */
		break;
	case EATING:
		printf (&uart, "\033[1;32m%d: eating  ", i+1);	/* Light green */
		break;
	case WAITING:
		printf (&uart, "\033[1;31m%d: waiting ", i+1);	/* Light red */
		break;
	}
	mutex_unlock (&screen);
}

void think (int i)
{
	display (i, THINKING);
	timer_delay (&timer, rand15() % 2000);
}

void eat (int i)
{
	display (i, EATING);
	timer_delay (&timer, rand15() % 2000);
}

void get_forks (int i, mutex_t *left_fork, mutex_t *right_fork)
{
	for (;;) {
		mutex_lock (left_fork);
		if (mutex_trylock (right_fork))
			return;
		mutex_unlock (left_fork);
		display (i, WAITING);
		mutex_wait (&table);
	}
}

void put_forks (mutex_t *left_fork, mutex_t *right_fork)
{
	mutex_unlock (left_fork);
	mutex_unlock (right_fork);
	mutex_signal (&table, 0);
}

void philosopher (void *data)
{
	int i;

	i = (int) data;
	for (;;) {
		think (i);
		get_forks (i, &fork [i], &fork [(i+1) % N]);
		eat (i);
		put_forks (&fork [i], &fork [(i+1) % N]);
	}
}

task_t *tsk;

void uos_init ()
{
	int i;

	//uart_init (&uart, 1, 90, KHZ, 115200);
	timer_init (&timer, KHZ, 50);

	/* Create N philosopher tasks. */
	for (i=0; i<N; ++i) {
		tsk = task_create (philosopher, (void*) i, "phil", 1,
			task[i], sizeof (task[i]));
		debug_printf ("%08X ", tsk);
    }
    debug_printf ("\n\n");
}
