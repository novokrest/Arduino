/*
 * UsbDevice.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "UsbDevice.h"
#include "Logger.h"
#include "Exceptions.h"
#include "Verifiers.h"

UsbDevice::UsbDevice(LibUsbContext &ctx, const DeviceDescription& device)
	: ctx_(ctx), devh_(NULL)
	, vendor_(device.VendorId)
	, product_(device.ProductId)
	, ifNumber_(device.InterfaceNumber)
	, epNumber_(device.EndpointNumber)
	, epDirection_(device.EndpointDirection)
	, epType_(device.EndpointType)
{

}

void UsbDevice::Open()
{
	Verifiers::Assert(devh_ == NULL, "Device has been already opened");

	Logger::Log("Try to open device...");

	devh_ = libusb_open_device_with_vid_pid(&ctx_, vendor_, product_);
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

	Logger::Log("Device has been opened successfully");
}

void UsbDevice::transfer_callback(libusb_transfer *transfer)
{
	UsbDevice *device = static_cast<UsbDevice*>(transfer->user_data);
	if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
		device->OnTransferCompleted(transfer);
	}
	else {
		device->OnTransferFailed(transfer);
	}

	libusb_free_transfer(transfer);
}

void UsbDevice::OnTransferCompleted(libusb_transfer *transfer)
{
	Logger::Log("Transfer completed");
	lastTransferState_ = TransferState::COMPLETED;
}

void UsbDevice::OnTransferFailed(libusb_transfer *transfer)
{
	Logger::Log("Transfer failed");
	lastTransferState_ = TransferState::FAILED;
}

void UsbDevice::SubmitTransfer(Data& buffer)
{
	libusb_transfer *transfer;

	transfer = libusb_alloc_transfer(0);
	if (!transfer) {
		Logger::Log("Failed to allocate transfer");
		return;
	}

	if (epType_ == ENDPOINT_BULK_TYPE) {
		libusb_fill_bulk_transfer(transfer, devh_, (epNumber_ | epDirection_), &buffer.front(), buffer.size(), &UsbDevice::transfer_callback, this, 0);
	}
	else if (epType_ == ENDPOINT_INTERRUPT_TYPE) {
		libusb_fill_interrupt_transfer(transfer, devh_, (epNumber_ | epDirection_), &buffer.front(), buffer.size(), &UsbDevice::transfer_callback, this, 0);
	}

	if (libusb_submit_transfer(transfer)) {
		Logger::Log("Failed to submit transfer");
		lastTransferState_ = TransferState::FAILED;
		libusb_free_transfer(transfer);
	}
	else {
		Logger::Log("Transfer submitted successfully");
		lastTransferState_ = TransferState::PENDING;
	}
	ctx_.HandleEvents();
}

//TODO: Check size of received data
void UsbDevice::ReceiveData(size_t dataSize)
{
	Data data(dataSize);
	SubmitTransfer(data);
	if (lastTransferState_ == TransferState::COMPLETED) {
		Logger::Log("Data has been received");
		OnDataReceived(data);
	}
}

void UsbDevice::Close()
{
	Verifiers::Assert(devh_ != NULL, "Device is not opened");

	Logger::Log("Try to close device...");

	if (libusb_release_interface(devh_, ifNumber_)) {
		Logger::Log("Failed to release interface");
	}

	if (libusb_attach_kernel_driver(devh_, ifNumber_)) {
		Logger::Log("Failed to attach kernel driver");
	}

	libusb_close(devh_);
	devh_ = NULL;

	Logger::Log("Device has been closed successfully");
}

UsbDevice::~UsbDevice() {
	if (devh_) {
		Close();
	}
}


HotPluggableUsbDevice::HotPluggableUsbDevice(LibUsbContext& ctx, const DeviceDescription& device)
	: UsbDevice(ctx, device)
	, detector_(new HotPlugDetector(ctx, device.VendorId, device.ProductId))
{

}

void HotPluggableUsbDevice::WaitForArrived()
{
	detector_->WaitForArrived();
}

bool HotPluggableUsbDevice::IsPresent()
{
	return detector_->IsPresent();
}

HotPluggableUsbDevice::~HotPluggableUsbDevice()
{
	delete detector_;
}
