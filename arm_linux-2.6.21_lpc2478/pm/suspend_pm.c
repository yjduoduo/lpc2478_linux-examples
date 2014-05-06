/*
 * suspend_pm.c
 *
 */
#include <linux/suspend.h>
#include <linux/module.h>

#ifdef CONFIG_PM
/* kernel/power/swsusp.c */


#if defined(CONFIG_VT) && defined(CONFIG_VT_CONSOLE)
extern int pm_prepare_console(void);
extern void pm_restore_console(void);

#else

inline int pm_prepare_console(void) { return 0; }
inline void pm_restore_console(void) {}

#endif /* defined(CONFIG_VT) && defined(CONFIG_VT_CONSOLE) */

EXPORT_SYMBOL(pm_prepare_console);
EXPORT_SYMBOL(pm_restore_console);
#else

int software_suspend(void)
{
	printk("Warning: fake suspend called\n");
	return -ENOSYS;
}

EXPORT_SYMBOL(software_suspend);

#endif /* CONFIG_PM */