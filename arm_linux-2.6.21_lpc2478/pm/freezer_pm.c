/*
 *freezer_pm.c
 *freezer powermangment
 */

#include <linux/freezer.h>
#include <linux/module.h>

#ifdef CONFIG_PM
/*
* Check if a process has been frozen
*/
int frozen(struct task_struct *p)
{
	return p->flags & PF_FROZEN;
}

/*
* Check if there is a request to freeze a process
*/
int freezing(struct task_struct *p)
{
	return test_tsk_thread_flag(p, TIF_FREEZE);
}

/*
* Request that a process be frozen
*/
void freeze(struct task_struct *p)
{
	set_tsk_thread_flag(p, TIF_FREEZE);
}

/*
* Sometimes we may need to cancel the previous 'freeze' request
*/
void do_not_freeze(struct task_struct *p)
{
	clear_tsk_thread_flag(p, TIF_FREEZE);
}

/*
* Wake up a frozen process
*/
int thaw_process(struct task_struct *p)
{
	if (frozen(p)) {
		p->flags &= ~PF_FROZEN;
		wake_up_process(p);
		return 1;
	}
	return 0;
}

/*
* freezing is complete, mark process as frozen
*/
void frozen_process(struct task_struct *p)
{
	p->flags |= PF_FROZEN;
	wmb();
	clear_tsk_thread_flag(p, TIF_FREEZE);
}



int try_to_freeze(void)
{
	if (freezing(current)) {
		refrigerator();
		return 1;
	} else
		return 0;
}



#else
 int frozen(struct task_struct *p) { return 0; }
 int freezing(struct task_struct *p) { return 0; }
 void freeze(struct task_struct *p) { BUG(); }
 int thaw_process(struct task_struct *p) { return 1; }
 void frozen_process(struct task_struct *p) { BUG(); }

 void refrigerator(void) {}
 int freeze_processes(void) { BUG(); return 0; }
 void thaw_processes(void) {}

 int try_to_freeze(void) { return 0; }

#endif

EXPORT_SYMBOL(frozen);
EXPORT_SYMBOL(freezing);
EXPORT_SYMBOL(freeze);
EXPORT_SYMBOL(thaw_process);
EXPORT_SYMBOL(frozen_process);
EXPORT_SYMBOL(refrigerator);
EXPORT_SYMBOL(freeze_processes);
EXPORT_SYMBOL(thaw_processes);
EXPORT_SYMBOL(try_to_freeze);