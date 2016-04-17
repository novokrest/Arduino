/*
 * UsbDeviceListener.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef USBDEVICELISTENER_H_
#define USBDEVICELISTENER_H_

#include "common.h"

class UsbDeviceListener
{
public:
	virtual ~UsbDeviceListener();
	virtual void OnDataReceived(const Data& data) = 0;
};

#endif /* USBDEVICELISTENER_H_ */
