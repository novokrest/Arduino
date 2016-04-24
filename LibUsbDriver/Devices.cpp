/*
 * Devices.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "Devices.h"
#include "Verifiers.h"
#include <string>

static const DeviceDescription JustKeyboardDescription = {
		.VendorId = 0x045e,
		.ProductId = 0x07b9,
		.InterfaceNumber = 0,
		.EndpointNumber = 0x81,
		.EndpointDirection = LIBUSB_ENDPOINT_IN,
		.EndpointType = ENDPOINT_INTERRUPT_TYPE
};

static const DeviceDescription JustArduinoDescription = {
		.VendorId = 0x2341,
		.ProductId = 0x0001,
		.InterfaceNumber = 1,
		.EndpointNumber = 0x83,
		.EndpointDirection = LIBUSB_ENDPOINT_IN,
		.EndpointType = ENDPOINT_BULK_TYPE
};

static const DeviceDescription ArduinoKeyboardDescription = {
		.VendorId = 0x03eb,
		.ProductId = 0x2042,
		.InterfaceNumber = 0,
		.EndpointNumber = 0x81,
		.EndpointDirection = LIBUSB_ENDPOINT_IN,
		.EndpointType = ENDPOINT_INTERRUPT_TYPE
};

static const DeviceDescription ArduinoMouseDescription = {
		.VendorId = 0x03eb,
		.ProductId = 0x2042,
		.InterfaceNumber = 0,
		.EndpointNumber = 0x81,
		.EndpointDirection = LIBUSB_ENDPOINT_IN,
		.EndpointType = ENDPOINT_INTERRUPT_TYPE
};

//TODO: Implement move-contructors
UsbDevice* DevicesCreator::Create(LibUsbContext& ctx, DeviceType device, bool encrypted)
{
	switch(device) {

	case DeviceType::JUST_KEYBOARD:
		return CreateJustKeyboard(ctx, encrypted);

	case DeviceType::JUST_ARDUINO:
		return CreateJustArduino(ctx, encrypted);

	case DeviceType::ARDUINO_KEYBOARD:
		return CreateArduinoKeyboard(ctx, encrypted);

	case DeviceType::ARDUINO_MOUSE:
		return CreateArduinoMouse(ctx, encrypted);

	default:
		Verifiers::Verify(false, "Unknown device type: " + std::to_string((int)device));
	}

	return NULL;
}

Keyboard* DevicesCreator::CreateJustKeyboard(LibUsbContext& ctx, bool encrypted)
{
	return new Keyboard(ctx, JustKeyboardDescription, encrypted);
}

Keyboard* DevicesCreator::CreateJustArduino(LibUsbContext& ctx, bool encrypted)
{
	return new Keyboard(ctx, JustArduinoDescription, encrypted);
}

Keyboard* DevicesCreator::CreateArduinoKeyboard(LibUsbContext& ctx, bool encrypted)
{
	return new Keyboard(ctx, ArduinoKeyboardDescription, encrypted);
}

Mouse* DevicesCreator::CreateArduinoMouse(LibUsbContext& ctx, bool encrypted)
{
	return new Mouse(ctx, ArduinoMouseDescription, encrypted);
}
