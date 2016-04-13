/*
 * Keyboard.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"
#include "Logger.h"
#include "Util.h"
#include "Utils.h"
#include <exception>

Keyboard::Keyboard(uint16_t vendor, uint16_t product) : ctx_(NULL), devh_(NULL), vendor_(vendor), product_(product)
{
	int result;
	result = libusb_init(&ctx_);
	if (result < 0) {
		throw LibUsbException("Fail: libusb_init");
	}
}

Keyboard::Keyboard(const DeviceDescription& device, bool encrypted)
	: ctx_(NULL), devh_(NULL), encrypted_(encrypted), received_(0)
	, vendor_(device.VendorId)
	, product_(device.ProductId)
	, ifNumber_(device.InterfaceNumber)
	, epNumber_(device.EndpointNumber)
	, epDirection_(device.EndpointDirection)
	, epType_(device.EndpointType)
{
	int result;
	result = libusb_init(&ctx_);
	if (result < 0) {
		throw LibUsbException("Fail: libusb_init");
	}
}

void Keyboard::Open()
{
	devh_ = libusb_open_device_with_vid_pid(ctx_, vendor_, product_);
	if (devh_ == NULL) {
		throw LibUsbException("Failed to open device");
	}

	if (libusb_kernel_driver_active(devh_, ifNumber_) == 1) {
		Logger::Log("Kernel driver is active");
		if (libusb_detach_kernel_driver(devh_, ifNumber_) == 0) {
			Logger::Log("Kernel driver is detached");
		}
	}

	if (libusb_claim_interface(devh_, ifNumber_) < 0) {
		Logger::Log("Failed to claim interface");
		throw LibUsbException("Failed: libusb_claim_interface");
	}
}

void Keyboard::Receive()
{
	int count, result;
	Data data(8), decrypted(8);

	if (epType_ == ENDPOINT_BULK_TYPE) {
		result = libusb_bulk_transfer(devh_, (epNumber_ | epDirection_), (unsigned char*)&data.front(), data.size(), &count, 0);
	}
	else {
		result = libusb_interrupt_transfer(devh_, (epNumber_ | epDirection_), (unsigned char*)&data.front(), data.size(), &count, 0);
	}
	if (result) {
		throw LibUsbException("Transfer error!");
	}

	if (encrypted_) {
		cryptor_.Decrypt(data, data);
	}

	Logger::Log(std::string("Id: ") + std::to_string(received_));
	Logger::Log(std::string("Count ") + std::to_string(count) + ": " + Utils::ToString(data));
}

void Keyboard::Start()
{
	Open();
	while(true) {
		Receive();
		++received_;
	}
}

void Keyboard::Loop()
{
	isLoop_ = true;
	while (isLoop_) {
		libusb_handle_events(ctx_);
	}
}

void Keyboard::StartAsync()
{

}

Keyboard::~Keyboard()
{
	if (devh_ != NULL) {
		libusb_close(devh_);
	}
	if (ctx_ != NULL) {
		libusb_exit(ctx_);
	}
}

LibUsbException::LibUsbException(const std::string &message)
	: message_(message)
{

}

LibUsbException::~LibUsbException() throw()
{
}

const char *LibUsbException::what() const throw()
{
	return message_.c_str();
}

