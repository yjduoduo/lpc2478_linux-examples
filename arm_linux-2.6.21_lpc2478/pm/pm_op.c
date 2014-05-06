/*
 * pm_op.c
 * power mangment operator
 */

#include <linux/pm.h>
#include <linux/device.h>
#include <linux/module.h>

#ifdef CONFIG_PM

void device_set_wakeup_enable(struct device *dev,unsigned val)
{
	dev->power.should_wakeup = !!val;
}

int device_may_wakeup(struct device *dev)
{
	return (device_can_wakeup(dev) && (dev)->power.should_wakeup);
}

void suspend_report_result(void *fn, int ret)
{							
	__suspend_report_result(__FUNCTION__, fn, ret);		
} 

#else /* !CONFIG_PM */

int device_suspend(pm_message_t state)
{
	return 0;
}

void device_set_wakeup_enable(struct device *dev,unsigned val) {}

 int device_may_wakeup(struct device *dev)			
{
	return 0;
}

int dpm_runtime_suspend(struct device * dev, pm_message_t state)
{
	return 0;
}

void dpm_runtime_resume(struct device * dev)
{
}

void suspend_report_result(void *fn, int ret)
{
}
EXPORT_SYMBOL(device_suspend);
EXPORT_SYMBOL(dpm_runtime_suspend);
EXPORT_SYMBOL(dpm_runtime_resume);

#endif

EXPORT_SYMBOL(device_set_wakeup_enable);
EXPORT_SYMBOL(device_may_wakeup);
EXPORT_SYMBOL(suspend_report_result);