/*
 * preempt_op.c
 * preempt operator
 */

#include <linux/preempt.h>
#include <linux/module.h>

#ifdef CONFIG_PREEMPT

asmlinkage void preempt_schedule(void);

void preempt_disable(void) 
{
	inc_preempt_count(); 
	barrier(); 
} 

void preempt_enable_no_resched(void) 
{ 
	barrier(); 
	dec_preempt_count(); 
} 

void preempt_check_resched(void)
{ 
	if (unlikely(test_thread_flag(TIF_NEED_RESCHED))) 
		preempt_schedule(); 
} 

void preempt_enable(void) 
{ 
	preempt_enable_no_resched(); 
	barrier(); 
	preempt_check_resched(); 
} 

#else

void preempt_disable(void)		{}
void preempt_enable_no_resched(void)	{}
void preempt_enable(void)		{}
void preempt_check_resched(void)		{}

#endif

EXPORT_SYMBOL(preempt_disable);
EXPORT_SYMBOL(preempt_enable_no_resched);
EXPORT_SYMBOL(preempt_enable);
EXPORT_SYMBOL(preempt_check_resched);