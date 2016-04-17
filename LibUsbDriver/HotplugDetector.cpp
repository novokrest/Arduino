/*
 * HotplugDetector.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#include "HotplugDetector.h"
#include "Logger.h"
#include "Exceptions.h"
#include <string>
#include <unistd.h>

HotPlugDetector::HotPlugDetector(LibUsbContext &ctx, uint16_t vendor, uint16_t product)
	: ctx_(ctx), vendor_(vendor), product_(product)
	, registered_(false), arrived_(false), left_(true)
{
}

HotPlugDetector::~HotPlugDetector() {
	if (registered_) {
		libusb_hotplug_deregister_callback(&ctx_, handle_);
		registered_ = false;
	}
}

int HotPlugDetector::dispatch_hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data)
{
	HotPlugDetector *detector = static_cast<HotPlugDetector*>(user_data);
	if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
		detector->Arrived();
	}
	else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
		detector->Left();
	}
	else {
		Logger::Log("Unexpected hotplug event: " + std::to_string((int)event));
	}

	return 0;
}

void HotPlugDetector::Arrived()
{
	arrived_ = true;
	left_ = false;
	Logger::Log("Device is arrived");
}

void HotPlugDetector::Left()
{
	arrived_ = false;
	left_ = true;
	Logger::Log("Device is left");
}

void HotPlugDetector::Register()
{
	if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
		Logger::Log("Hotplugging is not supported");
		arrived_ = true; // suppose that device is presented
		return;
	}

	libusb_hotplug_event events = (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
	libusb_hotplug_flag flags = (libusb_hotplug_flag)LIBUSB_HOTPLUG_ENUMERATE;
	int rc = libusb_hotplug_register_callback(&ctx_, events, flags, vendor_, product_,
	                                              LIBUSB_HOTPLUG_MATCH_ANY, &HotPlugDetector::dispatch_hotplug_callback, this,
	                                              &handle_);

	if (rc != LIBUSB_SUCCESS) {
		  Logger::Log("Failed to register hotplugging");
		  return;
	}

	registered_ = true;
}

void HotPlugDetector::WaitForArrived()
{
	if (!registered_) {
		Register();
	}

	Logger::Log("Wait for device arriving...");
	while (!arrived_) {
		ctx_.HandleEvents();
	}
}

bool HotPlugDetector::IsPresent()
{
	return arrived_;
}
