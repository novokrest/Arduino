/*
 * HotplugDetector.h
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#ifndef HOTPLUGDETECTOR_H_
#define HOTPLUGDETECTOR_H_

#include <libusb-1.0/libusb.h>

class HotplugDetector {
	libusb_context& ctx_;
	uint16_t vendor_;
	uint16_t product_;

	//bool IsAlreadyExists();

public:
	HotplugDetector(libusb_context &ctx, uint16_t vendor, uint16_t product);
	virtual ~HotplugDetector();

	int Detect();
};

#endif /* HOTPLUGDETECTOR_H_ */
