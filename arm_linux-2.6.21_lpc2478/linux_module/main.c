/** 
  * Copyright (C) 2010 ZLGMCU CO.,Ltd
  * All right reserved
  *
  * the template char module
  * 
  * @file    main.c
  * @author  tkstudio
  * @date    2010/11/02
  * @version 1.0
  *
  *   History
  *------------------------------------------------------------------------------------------
  *
  */
 

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h>		/**< kmalloc() */
#include <linux/fs.h>		/**< everything... */
#include <linux/cdev.h>
#include <linux/errno.h>	/**< error codes */
#include <linux/types.h>	/**< size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <linux/aio.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include<linux/device.h>

#define DEBUG_CHAR 1
#ifdef DEBUG_CHAR
#define DBG(format,arg...) printk(format,## arg)
#else
#define DBG(format,...)
#endif

#define CHAR_NAME "CharDev"

static int char_major = 0;
static int char_devs =  1;	/**< number of char devices */


module_param(char_major, int, 0);
module_param(char_devs, int, 0);
MODULE_AUTHOR("Tkstudio");
MODULE_LICENSE("Dual BSD/GPL");


struct cdev *char_cdev; /**< allocated in char_init */


/**
  * Open and close
  */
int char_open (struct inode *inode, struct file *filp)
{
	DBG("char_open\n");
	return 1;          /**< success */
}

int char_release (struct inode *inode, struct file *filp)
{
	DBG("char_release\n");
	return 0;
}



/**
  * Data management: read and write
  */
ssize_t char_read (struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
	ssize_t retval = count;
	
	DBG("char_read\n");
	return retval;
}

ssize_t char_write (struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	size_t retval = count;
	
	DBG("char_write\n");
	return retval;
}

/**
  * The ioctl() implementation
  */
int char_ioctl (struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
	DBG("char_ioctl\n");
	return 0;
}

/**
  * The "extended" operations
  */
loff_t char_llseek (struct file *filp, loff_t off, int whence)
{
	DBG("char_llseek\n");
	return 0;
}

/*
 * The fops
 */

struct file_operations char_fops = {
	.owner =     THIS_MODULE,
	.llseek =    char_llseek,
	.read =	     char_read,
	.write =     char_write,
	.ioctl =     char_ioctl,
	.open =	     char_open,
	.release =   char_release,
};




static void char_setup_cdev(struct cdev *dev, int index)
{
	int err, devno = MKDEV(char_major, index);
    
	cdev_init(dev, &char_fops);
	dev->owner = THIS_MODULE;
	err = cdev_add (dev, devno, 1);
	
	/** Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}



/** char device class for udev/mdev generial /dev */
static struct class *g_char_class;

/**
  * Finally, the module stuff
  */
static int __init char_init(void)
{
	int result, i;
	dev_t dev = MKDEV(char_major, 0);
	
	/*
	 * Register your major, and accept a dynamic number.
	 */
	if (char_major)
		result = register_chrdev_region(dev, char_devs, "scullc");
	else {
		result = alloc_chrdev_region(&dev, 0, char_devs, "scullc");
		char_major = MAJOR(dev);
	}
	if (result < 0)
		return result;

	
	/* 
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	char_cdev = kmalloc(char_devs*sizeof (struct cdev), GFP_KERNEL);
	if (!char_cdev) {
		result = -ENOMEM;
		goto fail_malloc;
	}
	
	g_char_class = class_create(THIS_MODULE,CHAR_NAME);
	
	memset(char_cdev, 0, char_devs*sizeof (struct cdev));
	for (i = 0; i < char_devs; i++) {
		char_setup_cdev(char_cdev + i, i);
		class_device_create(g_char_class,NULL,MKDEV(char_major,i),NULL,"Char%d",i);
	}

	
	return 0; /* succeed */

  fail_malloc:
	unregister_chrdev_region(dev, char_devs);
	return result;
}



void __exit char_cleanup(void)
{
	int i;

	for (i = 0; i < char_devs; i++) {
		cdev_del(&char_cdev[i]);
		class_device_destroy(g_char_class,MKDEV(char_major,i));
	}
	
	class_destroy(g_char_class);
	kfree(char_cdev);

	unregister_chrdev_region(MKDEV (char_major, 0), char_devs);
}


module_init(char_init);
module_exit(char_cleanup);
