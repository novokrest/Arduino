/*
 * Keyboard.h
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "libusb-1.0/libusb.h"
#include "DesCryptor.h"
#include <exception>
#include <string>

#define ENDPOINT_CONTROL_TYPE       0x00
#define ENDPOINT_ISOCHRONOUS_TYPE   0x01
#define ENDPOINT_BULK_TYPE          0x10
#define ENDPOINT_INTERRUPT_TYPE     0x11

struct DeviceDescription
{
	uint16_t VendorId;
	uint16_t ProductId;
	uint8_t InterfaceNumber;
	uint8_t EndpointNumber;
	uint8_t EndpointDirection;
	uint8_t EndpointType;
};

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

enum class DeviceState
{
	PLUGGED,
	UNPLUGGED
};

class Keyboard {
	libusb_context *ctx_;
	libusb_device_handle *devh_;
	libusb_hotplug_callback_handle handle_;
	Data data_;

	DeviceState state_;

	DesCryptor cryptor_;
	bool encrypted_;

	uint32_t received_;

	uint16_t vendor_;
	uint16_t product_;
	uint8_t ifNumber_;
	uint8_t epNumber_;
	uint8_t epDirection_;
	uint8_t epType_;

	bool isLoop_;

	bool RegisterHotplug();

	void Open();
	void Close();
	void Receive();

	void SubmitTransfer();
	void OnTransferCompleted(libusb_transfer *transfer);
	void OnTransferFailed(libusb_transfer *transfer);

	void Loop();

	static int dispatch_hotplug_callback(libusb_context *ctx, libusb_device *dev,
	                                     libusb_hotplug_event event, void *user_data);

	static void transfer_callback(libusb_transfer *transfer);

	void OnArrived(libusb_device *dev, libusb_hotplug_event event);
	void OnLeft(libusb_device *dev, libusb_hotplug_event event);

public:
	Keyboard(const DeviceDescription& device, bool encrypted);
	virtual ~Keyboard();

	void Start();
	void StartAsync();
};

class LibUsbException : public std::exception
{
private:
	const std::string message_;

public:
	LibUsbException(const std::string &message);
	virtual ~LibUsbException() throw();
	virtual const char *what() const throw();
};

#endif /* KEYBOARD_H_ */
