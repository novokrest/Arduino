/*
 * Devices.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include "Keyboard.h"

enum class DeviceType
{
	JUST_KEYBOARD = 0,
	JUST_ARDUINO,
	ARDUINO_KEYBOARD,
	NONE
};

class DevicesCreator {
	DevicesCreator() = delete;
	~DevicesCreator() = delete;

	static Keyboard* CreateJustKeyboard(LibUsbContext& ctx);
	static Keyboard* CreateJustArduino(LibUsbContext& ctx);
	static Keyboard* CreateArduinoKeyboard(LibUsbContext& ctx);

public:
	static Keyboard* Create(LibUsbContext& ctx, DeviceType device);
};

#endif /* DEVICES_H_ */
