/*
 *srcu_preempt.c
 *src premmpt
 */
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/percpu.h>
#include <linux/preempt.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/srcu.h>
#include <linux/module.h>

#ifndef CONFIG_PREEMPT

 void srcu_barrier(void) 
{
	barrier();
}

#else /* #ifndef CONFIG_PREEMPT */

 void srcu_barrier(void) {}

#endif /* #else #ifndef CONFIG_PREEMPT */

EXPORT_SYMBOL(srcu_barrier);