/*
 * DeviceSearcher.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "DeviceSearcher.h"

DeviceSearcher::DeviceSearcher(const libusb_context &ctx) : ctx_(ctx)
{
}

DeviceSearcher::~DeviceSearcher() {
	// TODO Auto-generated destructor stub
}
