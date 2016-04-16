/*
 * Keyboard.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Util.h"
#include "Utils.h"
#include <cassert>


Keyboard::Keyboard(const DeviceDescription& device, bool encrypted)
	: ctx_(NULL), devh_(NULL), handle_(0), data_(8)
	, state_(DeviceState::UNPLUGGED)
	, encrypted_(encrypted), received_(0)
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

void Keyboard::Close()
{
	if (libusb_release_interface(devh_, ifNumber_)) {
		Logger::Log("Failed to release interface");
	}

	if (libusb_attach_kernel_driver(devh_, ifNumber_)) {
		Logger::Log("Failed to attach kernel driver");
	}

	libusb_close(devh_);
}

void Keyboard::Receive()
{
	int count, result;
	Data data(8), decrypted(8);

	if (epType_ == ENDPOINT_BULK_TYPE) {
		data.resize(30);
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

int Keyboard::dispatch_hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data)
{
	Keyboard *keyboard = static_cast<Keyboard*>(user_data);
	if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
		keyboard->OnArrived(dev, event);
	}
	else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
		keyboard->OnLeft(dev, event);
	}
	else {
		Logger::Log("Unexpected event: " + std::to_string((int)event));
	}

	return 0;
}

void Keyboard::OnArrived(libusb_device *dev, libusb_hotplug_event event)
{
	assert(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED);
	Open();
	state_ = DeviceState::PLUGGED;
}

void Keyboard::OnLeft(libusb_device *dev, libusb_hotplug_event event)
{
	assert(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
	Close();
	state_ = DeviceState::UNPLUGGED;
}

bool Keyboard::RegisterHotplug()
{
	libusb_hotplug_event events = (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT);
	libusb_hotplug_flag flags = (libusb_hotplug_flag) LIBUSB_HOTPLUG_ENUMERATE;
	int rc;

	if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)){
		Logger::Log("Hotplugging is not supported");
		return false;
	}

	Logger::Log("Hotplugging is supported");

	rc = libusb_hotplug_register_callback(ctx_,
			  events, flags,
			  vendor_, product_, LIBUSB_HOTPLUG_MATCH_ANY,
			  &Keyboard::dispatch_hotplug_callback, this,
              &handle_);
	if (rc != LIBUSB_SUCCESS) {
		  Logger::Log("Failed to register hotplugging");
		  return false;
	}

	Logger::Log("Hotplugging has been registered successfully");
	return true;
}

void Keyboard::transfer_callback(libusb_transfer *transfer)
{
	Keyboard *keyboard = static_cast<Keyboard*>(transfer->user_data);
	if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
		keyboard->OnTransferCompleted(transfer);
	}
	else {
		keyboard->OnTransferFailed(transfer);
	}

	libusb_free_transfer(transfer);
}

void Keyboard::OnTransferCompleted(libusb_transfer *transfer)
{
	Logger::Log("Transfer completed");
}

void Keyboard::OnTransferFailed(libusb_transfer *transfer)
{
	Logger::Log("Transfer failed");
}


void Keyboard::SubmitTransfer()
{
	libusb_transfer *transfer;

	transfer = libusb_alloc_transfer(0);
	if (!transfer) {
		Logger::Log("Failed to allocate transfer");
		return;
	}

	if (epType_ == ENDPOINT_BULK_TYPE) {
		libusb_fill_bulk_transfer(transfer, devh_, (epNumber_ | epDirection_), &data_.front(), data_.size(), &Keyboard::transfer_callback, this, 0);
	}
	else if (epType_ == ENDPOINT_INTERRUPT_TYPE) {
		libusb_fill_interrupt_transfer(transfer, devh_, (epNumber_ | epDirection_), &data_.front(), data_.size(), &Keyboard::transfer_callback, this, 0);
	}

	if (libusb_submit_transfer(transfer)) {
		Logger::Log("Failed to submit transfer");
	}
	else {
		Logger::Log("Transfer submitted successfully");
	}
}

void Keyboard::Loop()
{
	if (!RegisterHotplug()) {
		return;
	}

	while(state_ != DeviceState::PLUGGED) {
		libusb_handle_events_completed(ctx_, NULL);
	}

	Logger::Log("Start to receive data...");

	isLoop_ = true;
	while (state_ == DeviceState::PLUGGED) {
		SubmitTransfer();
		libusb_handle_events_completed(ctx_, NULL);
	}
}

void Keyboard::StartAsync()
{
	Loop();
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

