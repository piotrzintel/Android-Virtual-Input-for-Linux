/*

   Android Virtual Mouse Driver (avms), part of Android™ Virtual Input for Linux project

   Copyright 2012 Piotr Zintel
   zintelpiotr@gmail.com

   Android is a trademark of Google Inc.

*/

/*

   Avms is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   Avms is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <linux/cdev.h>

#ifndef avms_MAJOR
#define avms_MAJOR 60
#endif

#define EV_COORDS 0
#define EV_BTN_LEFT_PRESS 1
#define EV_BTN_LEFT_RELEASE 2
#define EV_BTN_RIGHT_PRESS 3
#define EV_BTN_RIGHT_RELEASE 4
#define EV_BTN_MIDDLE_PRESS 5
#define EV_BTN_MIDDLE_RELEASE 6
#define EV_SCROLL_HORIZ 7
#define EV_SCROLL_VERT 8

MODULE_LICENSE("GPL");

// Uncomment to print debugging messages
//#define AVMS_DEBUG

struct input_dev *avms_input_dev;
struct cdev *avms_cdev;
dev_t avms_dev;

static int avms_major = avms_MAJOR;

static ssize_t avms_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void avms_exit(void);
static int avms_init(void);

struct file_operations avms_fops = {
	write: avms_write
};

static ssize_t avms_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos) {

	const char *tmp;
	char *write_buffer=(char *)kmalloc(sizeof(char)+2*sizeof(int),GFP_KERNEL);

	int Xval;
	int Yval;
	unsigned char evType;
	unsigned long result;

	tmp=buf+count-2*sizeof(int)-sizeof(char);
	if ( (result = copy_from_user(write_buffer,tmp,sizeof(char)+2*sizeof(int)) ) > 0) {
		printk("<3>avms: %ld bytes could not be read. buf: %s\n",result,buf);
		kfree(write_buffer);
		return -EFAULT;
	}

	memcpy(&evType,write_buffer,sizeof(char));
	memcpy(&Xval,(write_buffer+sizeof(char)),sizeof(int));
	memcpy(&Yval,(write_buffer+sizeof(char)+sizeof(int)),sizeof(int));

#ifdef AVMS_DEBUG
	printk("<3>avms: received: evType: %u, Xval: %d, Yval: %d\n",(unsigned int)evType,Xval,Yval);
#endif

	switch (evType) {

	case EV_COORDS:
		input_report_rel(avms_input_dev, REL_X, Xval);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting relative x: %d\n",Xval);
#endif
		input_report_rel(avms_input_dev, REL_Y, Yval);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting relative y: %d\n",Yval);
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_SCROLL_VERT:
		input_report_rel(avms_input_dev, REL_WHEEL, Xval);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting relative wheel: %d\n",Xval);
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_SCROLL_HORIZ:
		input_report_rel(avms_input_dev, REL_HWHEEL, Xval);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting relative horizontal wheel: %d\n",Xval);
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;


	case EV_BTN_LEFT_PRESS: // Report BTN_LEFT pressed
		input_report_key(avms_input_dev, BTN_LEFT, 1);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting BTN_LEFT, 1");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_BTN_LEFT_RELEASE: // Report BTN_LEFT released
		input_report_key(avms_input_dev, BTN_LEFT, 0);
#ifdef AVMS_DEBUG
		printk("<3>Reporting BTN_LEFT, 0");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_BTN_RIGHT_PRESS: // Report BTN_RIGHT pressed
		input_report_key(avms_input_dev, BTN_RIGHT, 1);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting BTN_RIGHT, 1");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_BTN_RIGHT_RELEASE: // Report BTN_RIGHT released
		input_report_key(avms_input_dev, BTN_RIGHT, 0);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting BTN_RIGHT, 0");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_BTN_MIDDLE_PRESS: // Report BTN_MIDDLE pressed
		input_report_key(avms_input_dev, BTN_MIDDLE, 1);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting BTN_MIDDLE, 1");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	case EV_BTN_MIDDLE_RELEASE: // Report BTN_MIDDLE released
		input_report_key(avms_input_dev, BTN_MIDDLE, 0);
#ifdef AVMS_DEBUG
		printk("<3>avms: Reporting BTN_MIDDLE, 0");
#endif
		input_sync(avms_input_dev);
#ifdef AVMS_DEBUG
		printk("<3>avms: Synced \n");
#endif
	break;

	default:
		printk("<3>avms: unknown event type. \n");
	break;
	}
	input_sync(avms_input_dev);
	kfree(write_buffer);
	return count;
}

static int __init avms_init(void)
{
	int result;
	
	if (avms_major) {
		avms_dev = MKDEV(avms_major, 0);
		result = register_chrdev_region(avms_dev, 1, "avms");
	} else {
		result = alloc_chrdev_region(&avms_dev, 0, 1,"avms");
		avms_major = MAJOR(avms_dev);
	}
	if (result < 0) {
		printk("<3>Bad chrdev_region(), major nr: %d\n",avms_major);
		return result;
	}

	avms_cdev = cdev_alloc();
	if (avms_cdev == NULL) {
		printk("<3>Bad cdev_alloc()\n");
		return -1;
	}
	avms_cdev->ops = &avms_fops;
	avms_cdev->owner = THIS_MODULE;

	result = cdev_add(avms_cdev, avms_dev, 1);
	if (result < 0) {
		printk("<3>cdev_add() error");
		cdev_del(avms_cdev);
		unregister_chrdev_region(avms_dev, 1);
		return -1;
	}

	avms_input_dev = input_allocate_device();

	if (avms_input_dev == NULL) {
		printk("<3>Bad input_alloc_device()\n");
		cdev_del(avms_cdev);
		unregister_chrdev_region(avms_dev, 1);
		return -1;
	}

	avms_input_dev->name = "Android Virtual Mouse";
	avms_input_dev->phys = "avms";
	avms_input_dev->id.bustype = BUS_VIRTUAL;
	avms_input_dev->id.vendor = 0x0000;
	avms_input_dev->id.product = 0x0000;
	avms_input_dev->id.version = 0x0000;

	set_bit(EV_REL, avms_input_dev->evbit);
	set_bit(REL_X, avms_input_dev->relbit);
	set_bit(REL_Y, avms_input_dev->relbit);
	set_bit(REL_WHEEL, avms_input_dev->relbit);
	set_bit(REL_HWHEEL, avms_input_dev->relbit);

	set_bit(EV_KEY, avms_input_dev->evbit);
	set_bit(BTN_LEFT, avms_input_dev->keybit);
	set_bit(BTN_RIGHT, avms_input_dev->keybit);
	set_bit(BTN_MIDDLE, avms_input_dev->keybit);

	if ( (result = input_register_device(avms_input_dev)) != 0 ) {
		printk("<3>avms: cannot register input_device\n");
		cdev_del(avms_cdev);
		unregister_chrdev_region(avms_dev, 1);
		return result;
	}

	printk("<3>Android Virtual Mouse Driver Initialized.\n");
	return 0;
}

static void avms_exit(void)
{
	input_unregister_device(avms_input_dev);
	cdev_del(avms_cdev);
	unregister_chrdev_region(avms_dev, 1);
	printk("<3>Android Virtual Mouse Driver unloaded.\n");
}

module_init(avms_init);
module_exit(avms_exit);
