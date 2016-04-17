/*
 * LibUsbContext.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef LIBUSBCONTEXT_H_
#define LIBUSBCONTEXT_H_

#include <libusb-1.0/libusb.h>

class LibUsbContext {
	libusb_context *ctx_;

public:
	LibUsbContext();
	~LibUsbContext();

	libusb_context* operator &();

	void HandleEvents();
};

#endif /* LIBUSBCONTEXT_H_ */
