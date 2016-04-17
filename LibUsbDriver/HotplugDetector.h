/*
 * HotplugDetector.h
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#ifndef HOTPLUGDETECTOR_H_
#define HOTPLUGDETECTOR_H_

#include "LibUsbContext.h"

class HotPlugDetector {
	LibUsbContext& ctx_;
	uint16_t vendor_;
	uint16_t product_;

	libusb_hotplug_callback_handle handle_;
	bool registered_;
	bool arrived_;
	bool left_;

	static int dispatch_hotplug_callback(libusb_context *ctx, libusb_device *dev,
	                                     libusb_hotplug_event event, void *user_data);

	void Register();
	void Arrived();
	void Left();

public:
	HotPlugDetector(LibUsbContext &ctx, uint16_t vendor, uint16_t product);
	~HotPlugDetector();

	void WaitForArrived();
	bool IsPresent();
};

#endif /* HOTPLUGDETECTOR_H_ */
