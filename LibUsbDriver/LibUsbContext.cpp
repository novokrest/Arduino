/*
 * LibUsbContext.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "LibUsbContext.h"
#include "Exceptions.h"
#include <unistd.h>

LibUsbContext::LibUsbContext() : ctx_(NULL) {
	int result = libusb_init(&ctx_);
	if (result < 0) {
		throw LibUsbException("Failed to initialize libusb");
	}
}

libusb_context* LibUsbContext::operator&() {
	return ctx_;
}

void LibUsbContext::HandleEvents()
{
	libusb_handle_events_completed(ctx_, NULL);
	usleep(10000);
}

LibUsbContext::~LibUsbContext() {
	if (ctx_ != NULL) {
		libusb_exit(ctx_);
	}
}

