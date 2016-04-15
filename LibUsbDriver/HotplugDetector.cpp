/*
 * HotplugDetector.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#include "HotplugDetector.h"
#include "Logger.h"
#include <string>
#include <unistd.h>

HotplugDetector::HotplugDetector(libusb_context &ctx, uint16_t vendor, uint16_t product)
	: ctx_(ctx), vendor_(vendor), product_(product)

{
}

HotplugDetector::~HotplugDetector() {
	// TODO Auto-generated destructor stub
}

static int count = 0;
int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data) {
  static libusb_device_handle *handle = NULL;
  struct libusb_device_descriptor desc;
  int rc;
  (void)libusb_get_device_descriptor(dev, &desc);
  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
	  Logger::Log("Arrived");
    rc = libusb_open(dev, &handle);
    if (LIBUSB_SUCCESS != rc) {
      Logger::Log("Could not open USB device\n");
    }
  } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
	  Logger::Log("Left");
    if (handle) {
      libusb_close(handle);
      handle = NULL;
    }
  } else {
    Logger::Log("Unhandled event %d\n");
  }
  count++;
  return 0;
}

int HotplugDetector::Detect()
{
	  libusb_hotplug_callback_handle handle;
	  libusb_hotplug_event events = (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
	  libusb_hotplug_flag flags = (libusb_hotplug_flag)0;
	  int rc;
	  libusb_init(NULL);
	  rc = libusb_hotplug_register_callback(NULL, events, flags, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY,
	                                        LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL,
	                                        &handle);
	  if (LIBUSB_SUCCESS != rc) {
	    Logger::Log("Error creating a hotplug callback\n");
	    libusb_exit(NULL);
	    return EXIT_FAILURE;
	  }
	  while (count < 2) {
	    libusb_handle_events_completed(NULL, NULL);
	    Logger::Log("after");
	    usleep(10000);
	  }
	  libusb_hotplug_deregister_callback(NULL, handle);
	  libusb_exit(NULL);
	  return 0;
}
