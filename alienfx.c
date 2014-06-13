/*
 * Alienware AlienFX driver
 *
 * Copyright (C) 2013 flätr0ze <fanmail@uberspace.net>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 3.
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>

/*
 * Version Information
 */

#define DRIVER_AUTHOR "flätr0ze <fanmail@uberspace.net>"
#define DRIVER_DESC "Alienware AlienFX Driver"

/*
 * Module parameters.
 */

#define HZ 20

enum alienfx_type {
	M11XR1, M11XR2, M11XR25, M11XR3, M14XR1, M15X, M17X, M17XR3, M18XR2, AURORA, AREA51
};

/* table of devices that work with this driver */
static const struct usb_device_id alienfx_table[] = {
	{ USB_DEVICE(0x187c, 0x0514), .driver_info = M11XR1 },
	{ USB_DEVICE(0x187c, 0x0515), .driver_info = M11XR2 },
	{ USB_DEVICE(0x187c, 0x0516), .driver_info = M11XR25 },
	{ USB_DEVICE(0x187c, 0x0522), .driver_info = M11XR3 },
	{ USB_DEVICE(0x187c, 0x0521), .driver_info = M14XR1 },
	{ USB_DEVICE(0x187c, 0x0512), .driver_info = M15X },
	{ USB_DEVICE(0x187c, 0x0512), .driver_info = M17X },
	{ USB_DEVICE(0x187c, 0x0520), .driver_info = M17XR3 },
	{ USB_DEVICE(0x187c, 0x0518), .driver_info = M18XR2 },
	{ USB_DEVICE(0x187c, 0x0513), .driver_info = AURORA },
	{ USB_DEVICE(0x187c, 0x0511), .driver_info = AREA51 },
	{},
};
MODULE_DEVICE_TABLE(usb, alienfx_table);

struct alienfx {
	struct usb_device *udev;
	unsigned char      val;
	enum alienfx_type  type;
};

//typedef enum afx_cmd {
	unsigned char AFX_CMD_CHG[9] = {0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Changing color (chain)
	unsigned char AFX_CMD_BLI[9] = {0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Blinking
	unsigned char AFX_CMD_STA[9] = {0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Static color
	unsigned char AFX_CMD_END[9] = {0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//,
	unsigned char AFX_CMD_PGE[9] = {0x02,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, 
	unsigned char AFX_CMD_CHK[9] = {0x02,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Check
	unsigned char AFX_CMD_RST[9] = {0x02,0x07,0x04,0x00,0x00,0x00,0x00,0x00,0x00};//, // Reset
	unsigned char AFX_CMD_ADD[9] = {0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Add program number
	unsigned char AFX_CMD_SVE[9] = {0x02,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//, // Add program number
	unsigned char AFX_CMD_SPE[9] = {0x02,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//,
//} afx_cmd;

/*

CHG: 


*/

// CHK should return [10 00 00 00 00 00 00 00] --always 8 bytes (it's an "OK" message)
//                   [11 ... -- initial CHK value (seems like the RDY version of OK)

/*
#define AW_SETCOLOR(zone,r,g,b) \
{	\
	unsigned char data[9]={0x02,0x03,0x05,0x00,0x00,0x01,0x0f,0xf0,0x00}; \
	data[4]=(zone>>8)&0xff;	\
	data[5]=(zone>>0)&0xff;	\
	data[6]=(r<<4)&0xf0;	\
	data[6]|=(g&0xf);	\
	data[7]=(b<<4)&0xf0;		\
	retval=usbwrite(usbhandle,data,9); \
}
#define AW_SETCOLORS(zone,r1,g1,b1, r2,g2,b2) \
{	\
	unsigned char data[9]={0x02,0x03,0x05,0x00,0x00,0x01,0x0f,0xf0,0x00}; \
	data[4]=(zone>>8)&0xff;	\
	data[5]=(zone>>0)&0xff;	\
	data[6]=(r1<<4)&0xf0;	\
	data[6]|=(g1&0xf);	\
	data[7]=(b1<<4)&0xf0;		\
	data[7]|=(r1&0xf);		\
	data[8]=(g1<<4)&0xf0;	\
	data[8]=(b1&0xf);	\
	retval=usbwrite(usbhandle,data,9); \
}
#define	AW_WAIT	\
{	\
	unsigned char rply[8];	\
	int ready=0;	\
	while (!ready)	\
	{	\
		AW_CHK				\
		usbread(usbhandle,rply,8);	\
		if (rply[0]==0x11) ready=1;	\
	}	\
}	
#define	CHANGECOLOR(zone,r,g,b) \
{	\
	AW_SETCOLOR(zone,r,g,b); 	\
	AW_END;		\
	AW_PGE;	\
	AW_RESET;	\
	AW_WAIT;	\
}
*/

/*
static unsigned int alienfx_read(struct alienfx *alien, unsigned int size)
{
	return size;
}
*/

static int alienfx_bulk_rw(struct usb_device *udev, unsigned char *data)
{
	int alen, ret;

	//unsigned char test[9] = {0x02,0x03,0x05,0x00,0x00,0x01,0x0f,0xf0,0x00};
	//ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
	//			0x09, 0x21, 0x202, 0,
	//			data, 9,/*NULL,0,*/ 2000);
//
	//udelay(9999);

	/* do a blocking bulk read to get data from the device */
	ret = usb_bulk_msg(udev,
				usb_rcvbulkpipe(udev, dev->bulk_in_endpointAddr),
				dev->bulk_in_buffer,
				sizeof(data),
				&alen, HZ*10);

	/* if the read was successful, copy the data to user space */
	if (!ret) {
		if (copy_to_user(buffer, dev->bulk_in_buffer, count))
			ret = -EFAULT;
		else
			ret = count;
	}


	return ret;
}

static void change_color(struct alienfx *alien)
{
	int retval = 0;

	dev_info(&alien->udev->dev, "AlienFX: change_color\n");

	//retval = alienfx_bulk_rw(alien->udev, AFX_CMD_RST);

	//retval = alienfx_bulk_rw(alien/*, buffer*/);
//	alienfx_bulk_rw(alien->udev, AFX_CMD_RST);
	// <read 8 bytes here from RST>
	unsigned char test[9] = {0x02,0x03,0x05,0x00,0x00,0x01,0x1f,0xf0,0x00};
	alienfx_bulk_rw(alien->udev, test);
//	alienfx_bulk_rw(alien->udev, AFX_CMD_END);
//	alienfx_bulk_rw(alien->udev, AFX_CMD_PGE);
//	alienfx_bulk_rw(alien->udev, AFX_CMD_RST);
//	alienfx_bulk_rw(alien->udev, AFX_CMD_CHK);
	// <read 8 bytes here from CHK>

}




/*struct i2c_client *client = to_i2c_client(dev); \
 struct chip_data *data = i2c_get_clientdata(client); \
 \
 chip_update_client(client); \
 return sprintf(buf, "%d\n", data->value); \*/

// TODO: send a read signal, and read the response back to output 44F632 (or 0x4735C1)
#define show_set(area) \
static ssize_t show_##area(struct device *dev, struct device_attribute *attr,\
				char *buf)                                                  \
{                                                                       \
	struct usb_interface *intf = to_usb_interface(dev);             \
	struct alienfx *alien = usb_get_intfdata(intf);                   \
	                                                                \
	return sprintf(buf, "%d\n", alien->val);                           \
}                                                                       \
static ssize_t set_##area(struct device *dev, struct device_attribute *attr,\
				const char *buf, size_t count)                              \
{                                                                       \
	struct usb_interface *intf = to_usb_interface(dev);             \
	struct alienfx *alien = usb_get_intfdata(intf);                   \
	change_color(alien);                                              \
	return count;                                                   \
}                                                                       \
static DEVICE_ATTR(area, S_IRUGO | S_IWUSR, show_##area, set_##area);
show_set(keyboard);

static int alienfx_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	struct alienfx *dev = NULL;
	int retval = -ENOMEM;

	dev = kzalloc(sizeof(struct alienfx), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&interface->dev, "out of memory\n");
		goto error_mem;
	}

	dev->udev = usb_get_dev(udev);
	dev->type = id->driver_info;

	usb_set_intfdata(interface, dev);

	retval = device_create_file(&interface->dev, &dev_attr_keyboard);
	if (retval)
		goto error;

	dev_info(&interface->dev, "AlienFX device connected.\n");
		return 0;

error:
	device_remove_file(&interface->dev, &dev_attr_keyboard);
	usb_set_intfdata (interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);
error_mem:
	return retval;
}

static void alienfx_disconnect(struct usb_interface *interface)
{
	struct alienfx *dev;

	dev = usb_get_intfdata(interface);

	device_remove_file(&interface->dev, &dev_attr_keyboard);

	/* first remove the files, then set the pointer to NULL */
	device_remove_file(&interface->dev, &dev_attr_keyboard);
	usb_set_intfdata (interface, NULL);

	usb_put_dev(dev->udev);

	kfree(dev);

	dev_info(&interface->dev, "AlienFX device now disconnected.\n");
}

static struct usb_driver alienfx_driver = {
	.name		= "alienfx",
	.probe		= alienfx_probe,
	.disconnect	= alienfx_disconnect,
	.id_table	= alienfx_table,
};

/*
static int __init alienfx_init(void)
{
	int retval = 0;

	retval = usb_register(&alienfx_driver);
	if (retval)
		NULL;//dev_err(&interface->dev, "usb_register failed. Error number %d", retval);
	return retval;
}

static void __exit alienfx_exit(void)
{
	usb_deregister(&alienfx_driver);
}

module_init(alienfx_init);
module_exit(alienfx_exit);
*/

module_usb_driver(alienfx_driver);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

