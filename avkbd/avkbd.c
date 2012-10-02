/*

   Android Virtual Keyboard Driver (avkbd), part of Android™ Virtual Input for Linux project

   Copyright 2012 Piotr Zintel
   zintelpiotr@gmail.com

   Android is a trademark of Google Inc.

*/

/*

   Avkbd is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   Avkbd is distributed in the hope that it will be useful,
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
//#include <asm/system.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

#ifndef avkbd_MAJOR
#define avkbd_MAJOR 61
#endif

// print debugging messages
//#define AVKBD_DEBUG

//Event types definitions:
#define AVKBD_KEY_GRAVE 0
#define AVKBD_KEY_ESC 1
#define AVKBD_KEY_1 2
#define AVKBD_KEY_2 3
#define AVKBD_KEY_3 4
#define AVKBD_KEY_4 5
#define AVKBD_KEY_5 6
#define AVKBD_KEY_6 7
#define AVKBD_KEY_7 8
#define AVKBD_KEY_8 9
#define AVKBD_KEY_9 10
#define AVKBD_KEY_0 11
#define AVKBD_KEY_MINUS 12
#define AVKBD_KEY_EQUAL 13
#define AVKBD_KEY_BACKSPACE 14
#define AVKBD_KEY_TAB 15
#define AVKBD_KEY_Q 16
#define AVKBD_KEY_W 17
#define AVKBD_KEY_E 18
#define AVKBD_KEY_R 19
#define AVKBD_KEY_T 20
#define AVKBD_KEY_Y 21
#define AVKBD_KEY_U 22
#define AVKBD_KEY_I 23
#define AVKBD_KEY_O 24
#define AVKBD_KEY_P 25
#define AVKBD_KEY_LEFTBRACE 26
#define AVKBD_KEY_RIGHTBRACE 27
#define AVKBD_KEY_BACKSLASH 28
#define AVKBD_KEY_CAPSLOCK 29
#define AVKBD_KEY_A 30
#define AVKBD_KEY_S 31
#define AVKBD_KEY_D 32
#define AVKBD_KEY_F 33
#define AVKBD_KEY_G 34
#define AVKBD_KEY_H 35
#define AVKBD_KEY_J 36
#define AVKBD_KEY_K 37
#define AVKBD_KEY_L 38
#define AVKBD_KEY_SEMICOLON 39
#define AVKBD_KEY_APOSTROPHE 40
#define AVKBD_KEY_ENTER 41
#define AVKBD_KEY_LEFTSHIFT 42
#define AVKBD_KEY_Z 43
#define AVKBD_KEY_X 44
#define AVKBD_KEY_C 45
#define AVKBD_KEY_V 46
#define AVKBD_KEY_B 47
#define AVKBD_KEY_N 48
#define AVKBD_KEY_M 49
#define AVKBD_KEY_COMMA 50
#define AVKBD_KEY_DOT 51
#define AVKBD_KEY_SLASH 52
#define AVKBD_KEY_RIGHTSHIFT 53
#define AVKBD_KEY_LEFTCTRL 54
#define AVKBD_KEY_LEFTMETA 55
#define AVKBD_KEY_LEFTALT 56
#define AVKBD_KEY_SPACE 57
#define AVKBD_KEY_RIGHTALT 58
#define AVKBD_KEY_RIGHTCTRL 59
#define AVKBD_KEY_UP 60
#define AVKBD_KEY_DOWN 61
#define AVKBD_KEY_LEFT 62
#define AVKBD_KEY_RIGHT 63
#define AVKBD_KEY_PAGEUP 64
#define AVKBD_KEY_PAGEDOWN 65
#define AVKBD_KEY_F1 66
#define AVKBD_KEY_F2 67
#define AVKBD_KEY_F3 68
#define AVKBD_KEY_F4 69
#define AVKBD_KEY_F5 70
#define AVKBD_KEY_F6 71
#define AVKBD_KEY_F7 72
#define AVKBD_KEY_F8 73
#define AVKBD_KEY_F9 74
#define AVKBD_KEY_F10 75
#define AVKBD_KEY_F11 76
#define AVKBD_KEY_F12 77
#define AVKBD_KEY_HOME 78
#define AVKBD_KEY_END 79
#define AVKBD_KEY_INSERT 80
#define AVKBD_KEY_DELETE 81
#define AVKBD_KEY_SYSRQ 82
#define AVKBD_KEY_PRINTSCRN AVKBD_KEY_SYSRQ
#define AVKBD_KEY_KP1 83
#define AVKBD_KEY_KP2 84
#define AVKBD_KEY_KP3 85
#define AVKBD_KEY_KP4 86
#define AVKBD_KEY_KP5 87
#define AVKBD_KEY_KP6 88
#define AVKBD_KEY_KP7 89
#define AVKBD_KEY_KP8 90
#define AVKBD_KEY_KP9 91
#define AVKBD_KEY_KP0 92
#define AVKBD_KEY_VOLUMEUP 93
#define AVKBD_KEY_VOLUMEDOWN 94
#define AVKBD_KEY_MUTE 95
#define AVKBD_KEY_PLAYPAUSE 96
#define AVKBD_KEY_PREVIOUSSONG 97
#define AVKBD_KEY_NEXTSONG 98

#define AVKBD_KEYMAP_SIZE 0x63

static unsigned char avkbd_keycode[AVKBD_KEYMAP_SIZE] = {
	[AVKBD_KEY_GRAVE] = KEY_GRAVE,
	[AVKBD_KEY_ESC] = KEY_ESC,
	[AVKBD_KEY_1] = KEY_1,
	[AVKBD_KEY_2] = KEY_2,
	[AVKBD_KEY_3] = KEY_3,
	[AVKBD_KEY_4] = KEY_4,
	[AVKBD_KEY_5] = KEY_5,
	[AVKBD_KEY_6] = KEY_6,
	[AVKBD_KEY_7] = KEY_7,
	[AVKBD_KEY_8] = KEY_8,
	[AVKBD_KEY_9] = KEY_9,
	[AVKBD_KEY_0] = KEY_0,
	[AVKBD_KEY_MINUS] = KEY_MINUS,
	[AVKBD_KEY_EQUAL] = KEY_EQUAL,
	[AVKBD_KEY_BACKSPACE] = KEY_BACKSPACE,
	[AVKBD_KEY_TAB] = KEY_TAB,
	[AVKBD_KEY_Q] = KEY_Q,
	[AVKBD_KEY_W] = KEY_W,
	[AVKBD_KEY_E] = KEY_E,
	[AVKBD_KEY_R] = KEY_R,
	[AVKBD_KEY_T] = KEY_T,
	[AVKBD_KEY_Y] = KEY_Y,
	[AVKBD_KEY_U] = KEY_U,
	[AVKBD_KEY_I] = KEY_I,
	[AVKBD_KEY_O] = KEY_O,
	[AVKBD_KEY_P] = KEY_P,
	[AVKBD_KEY_Q] = KEY_Q,
	[AVKBD_KEY_W] = KEY_W,
	[AVKBD_KEY_E] = KEY_E,
	[AVKBD_KEY_R] = KEY_R,
	[AVKBD_KEY_T] = KEY_T,
	[AVKBD_KEY_Y] = KEY_Y,
	[AVKBD_KEY_U] = KEY_U,
	[AVKBD_KEY_I] = KEY_I,
	[AVKBD_KEY_O] = KEY_O,
	[AVKBD_KEY_P] = KEY_P,
	[AVKBD_KEY_LEFTBRACE] = KEY_LEFTBRACE,
	[AVKBD_KEY_RIGHTBRACE] = KEY_RIGHTBRACE,
	[AVKBD_KEY_BACKSLASH] = KEY_BACKSLASH,
	[AVKBD_KEY_CAPSLOCK] = KEY_CAPSLOCK,
	[AVKBD_KEY_A] = KEY_A,
	[AVKBD_KEY_S] = KEY_S,
	[AVKBD_KEY_D] = KEY_D,
	[AVKBD_KEY_F] = KEY_F,
	[AVKBD_KEY_G] = KEY_G,
	[AVKBD_KEY_H] = KEY_H,
	[AVKBD_KEY_J] = KEY_J,
	[AVKBD_KEY_K] = KEY_K,
	[AVKBD_KEY_L] = KEY_L,
	[AVKBD_KEY_SEMICOLON] = KEY_SEMICOLON,
	[AVKBD_KEY_APOSTROPHE] = KEY_APOSTROPHE,
	[AVKBD_KEY_ENTER] = KEY_ENTER,
	[AVKBD_KEY_LEFTSHIFT] = KEY_LEFTSHIFT,
	[AVKBD_KEY_Z] = KEY_Z,
	[AVKBD_KEY_X] = KEY_X,
	[AVKBD_KEY_C] = KEY_C,
	[AVKBD_KEY_V] = KEY_V,
	[AVKBD_KEY_B] = KEY_B,
	[AVKBD_KEY_N] = KEY_N,
	[AVKBD_KEY_M] = KEY_M,
	[AVKBD_KEY_COMMA] = KEY_COMMA,
	[AVKBD_KEY_DOT] = KEY_DOT,
	[AVKBD_KEY_SLASH] = KEY_SLASH,
	[AVKBD_KEY_RIGHTSHIFT] = KEY_RIGHTSHIFT,
	[AVKBD_KEY_LEFTCTRL] = KEY_LEFTCTRL,
	[AVKBD_KEY_LEFTMETA] = KEY_LEFTMETA,
	[AVKBD_KEY_LEFTALT] = KEY_LEFTALT,
	[AVKBD_KEY_SPACE] = KEY_SPACE,
	[AVKBD_KEY_RIGHTALT] = KEY_RIGHTALT,
	[AVKBD_KEY_RIGHTCTRL] = KEY_RIGHTCTRL,
	[AVKBD_KEY_UP] = KEY_UP,
	[AVKBD_KEY_DOWN] = KEY_DOWN,
	[AVKBD_KEY_LEFT] = KEY_LEFT,
	[AVKBD_KEY_RIGHT] = KEY_RIGHT,
	[AVKBD_KEY_PAGEUP] = KEY_PAGEUP,
	[AVKBD_KEY_PAGEDOWN] = KEY_PAGEDOWN,
	[AVKBD_KEY_F1] = KEY_F1,
	[AVKBD_KEY_F2] = KEY_F2,
	[AVKBD_KEY_F3] = KEY_F3,
	[AVKBD_KEY_F4] = KEY_F4,
	[AVKBD_KEY_F5] = KEY_F5,
	[AVKBD_KEY_F6] = KEY_F6,
	[AVKBD_KEY_F7] = KEY_F7,
	[AVKBD_KEY_F8] = KEY_F8,
	[AVKBD_KEY_F9] = KEY_F9,
	[AVKBD_KEY_F10] = KEY_F10,
	[AVKBD_KEY_F11] = KEY_F11,
	[AVKBD_KEY_F12] = KEY_F12,
	[AVKBD_KEY_HOME] = KEY_HOME,
	[AVKBD_KEY_END] = KEY_END,
	[AVKBD_KEY_INSERT] = KEY_INSERT,
	[AVKBD_KEY_DELETE] = KEY_DELETE,
	[AVKBD_KEY_SYSRQ] = KEY_SYSRQ,
	[AVKBD_KEY_KP1] = KEY_KP1,
	[AVKBD_KEY_KP2] = KEY_KP2,
	[AVKBD_KEY_KP3] = KEY_KP3,
	[AVKBD_KEY_KP4] = KEY_KP4,
	[AVKBD_KEY_KP5] = KEY_KP5,
	[AVKBD_KEY_KP6] = KEY_KP6,
	[AVKBD_KEY_KP7] = KEY_KP7,
	[AVKBD_KEY_KP8] = KEY_KP8,
	[AVKBD_KEY_KP9] = KEY_KP9,
	[AVKBD_KEY_KP0] = KEY_KP0,
	[AVKBD_KEY_VOLUMEUP] = KEY_VOLUMEUP,
	[AVKBD_KEY_VOLUMEDOWN] = KEY_VOLUMEDOWN,
	[AVKBD_KEY_MUTE] = KEY_MUTE,
	[AVKBD_KEY_PLAYPAUSE] = KEY_PLAYPAUSE,
	[AVKBD_KEY_PREVIOUSSONG] = KEY_PREVIOUSSONG,
	[AVKBD_KEY_NEXTSONG] = KEY_NEXTSONG
};

struct input_dev *avkbd_input_dev;
struct cdev *avkbd_cdev;
dev_t avkbd_dev;

int avkbd_major = avkbd_MAJOR;

static ssize_t avkbd_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void avkbd_exit(void);
static int avkbd_init(void);

static struct file_operations avkbd_fops = {
	write: avkbd_write
};

static ssize_t avkbd_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos) {

	const char *tmp;
	char *write_buffer=kmalloc(2*sizeof(char),GFP_KERNEL);

	unsigned long result;
	unsigned char key, value;

	tmp=buf+count-2*sizeof(char);
	if ( (result = copy_from_user(write_buffer,tmp,2*sizeof(char)) ) > 0) {
		printk("<3>avkbd: %ld bytes could not be read. buf: %s\n",result,buf);
		return -EFAULT;
	}
	key = write_buffer[0];
	value = write_buffer[1];
	if ( key >= AVKBD_KEYMAP_SIZE ) {
		printk("<3>avkbd: %u - key out of keymap range\n",(unsigned int)key);
	} else if ( ( value != 0) && (value != 1) ) {
		printk("<3>avkbd: %u - incorrect value (0,1)\n",(unsigned int)value);
	} else {
#ifdef AVKBD_DEBUG
		printk("<3>avkbd: %u,%u\n",(unsigned int)key,(unsigned int)value);
#endif
		input_report_key(avkbd_input_dev,avkbd_keycode[(unsigned int)key],value);
		input_sync(avkbd_input_dev);
	}
	kfree(write_buffer);
	return count;
}

static int __init avkbd_init(void)
{
	int i, result;
	
	if (avkbd_major) {
		avkbd_dev = MKDEV(avkbd_major, 0);
		result = register_chrdev_region(avkbd_dev, 1, "avkbd");
	} else {
		result = alloc_chrdev_region(&avkbd_dev, 0, 1,"avkbd");
		avkbd_major = MAJOR(avkbd_dev);
	}
	if (result < 0) {
		printk("<3>Bad chrdev_region(), major nr: %d\n",avkbd_major);
		return result;
	}

	avkbd_cdev = cdev_alloc();
	if (avkbd_cdev == NULL) {
		printk("<3>Bad cdev_alloc()\n");
		return -1;
	}
	avkbd_cdev->ops = &avkbd_fops;
	avkbd_cdev->owner = THIS_MODULE;

	result = cdev_add(avkbd_cdev, avkbd_dev, 1);
	if (result < 0) {
		printk("<3>cdev_add() error");
		cdev_del(avkbd_cdev);
		unregister_chrdev_region(avkbd_dev, 1);
		return -1;
	}

	avkbd_input_dev = input_allocate_device();

	if (!avkbd_input_dev) {
		printk("<3>avkbd: Bad input_alloc_device()\n");
		unregister_chrdev(avkbd_major,"avkbd");
	}

	avkbd_input_dev->name = "Android Virtual Keyboard";
	avkbd_input_dev->phys = "avkbd";
	avkbd_input_dev->id.bustype = BUS_VIRTUAL;
	avkbd_input_dev->id.vendor = 0x0000;
	avkbd_input_dev->id.product = 0x0000;
	avkbd_input_dev->id.version = 0x0000;

	set_bit(EV_KEY, avkbd_input_dev->evbit);
	avkbd_input_dev->keycode = avkbd_keycode;
	avkbd_input_dev->keycodesize = sizeof(unsigned char);
	avkbd_input_dev->keycodemax = ARRAY_SIZE(avkbd_keycode);

	for ( i = 0; i < AVKBD_KEYMAP_SIZE; ++i ) {
		if ( avkbd_keycode[i] != KEY_RESERVED )
			set_bit(avkbd_keycode[i], avkbd_input_dev->keybit);
	}

	if ( (result = input_register_device(avkbd_input_dev)) != 0 ) {
		printk("<3>avkbd: could not register input_device\n");
		unregister_chrdev(avkbd_major,"avkbd");
		return result;
	}
	printk("<3>Android Virtual Keyboard Driver Initialized.\n");
	return 0;
}

static void avkbd_exit(void)
{
	input_unregister_device(avkbd_input_dev);
	cdev_del(avkbd_cdev);
	unregister_chrdev_region(avkbd_dev, 1);
	printk("<3>Android Virtual Keyboard Driver unloaded.\n");
}

module_init(avkbd_init);
module_exit(avkbd_exit);
