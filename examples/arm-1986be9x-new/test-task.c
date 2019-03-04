/*
 * Testing task switching.
 */
#include "runtime/lib.h"
#include "kernel/uos.h"

ARRAY (task_space, 0x400);	/* Memory for task stack */

void task (void *arg)
{
	for (;;) {
		debug_printf ("Hello from `%s'!\n", arg);
		debug_printf ("Task space %d bytes, free %d bytes.\n",
			sizeof (task_space), task_stack_avail ((task_t*) task_space));
		debug_puts ("(Press Enter)\n");
		debug_getchar ();
	}
}

void uos_init (void)
{
    debug_printf ("\nTesting task on 1986BE1\n");
	task_create (task, "task", "task", 1, task_space, sizeof (task_space));
}
