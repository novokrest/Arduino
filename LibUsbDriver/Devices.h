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

	static Keyboard* CreateJustKeyboard(LibUsbContext& ctx, bool encrypted);
	static Keyboard* CreateJustArduino(LibUsbContext& ctx, bool encrypted);
	static Keyboard* CreateArduinoKeyboard(LibUsbContext& ctx, bool encrypted);

public:
	static Keyboard* Create(LibUsbContext& ctx, DeviceType device, bool encrypted);
};

#endif /* DEVICES_H_ */
