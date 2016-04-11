/*
 * DeviceSearcher.h
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#ifndef DEVICESEARCHER_H_
#define DEVICESEARCHER_H_

#include "libusb-1.0/libusb.h"

class Device
{
private:
	libusb_device_handle *devh;

public:
	Device(libusb_device_handle *devh);
	virtual ~Device();
};

class DeviceSearcher {
private:
	const libusb_context &ctx_;

public:
	DeviceSearcher(const libusb_context &ctx);
	virtual ~DeviceSearcher();

	/**
	 * Returns handle for first matched device
	 * Client should free this handle by libusb_close
	 * */
	libusb_device_handle* FindFirst(uint16_t vendorId, uint16_t productId);
};

#endif /* DEVICESEARCHER_H_ */
