/*
 *hardirq_preempt.c
 *hardirq preempt
 */
#include <linux/hardirq.h>
#include <linux/module.h>

#if defined(CONFIG_PREEMPT) && !defined(CONFIG_PREEMPT_BKL)

 int in_atomic(void)	
{
	return ((preempt_count() & ~PREEMPT_ACTIVE) != kernel_locked());
}

#else

 int in_atomic(void)
{
	return ((preempt_count() & ~PREEMPT_ACTIVE) != 0);
}

#endif

EXPORT_SYMBOL(in_atomic);

#ifdef CONFIG_PREEMPT

 int preemptible(void)
{
	return (preempt_count() == 0 && !irqs_disabled());
}

unsigned long  IRQ_EXIT_OFFSET  = HARDIRQ_OFFSET-1;

#else

inline int preemptible(void)
{
	return 0;
}

unsigned long IRQ_EXIT_OFFSET = HARDIRQ_OFFSET;

#endif

EXPORT_SYMBOL(preemptible);
EXPORT_SYMBOL(IRQ_EXIT_OFFSET);
