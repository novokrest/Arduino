/*
 * UsbDevice.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef USBDEVICE_H_
#define USBDEVICE_H_

#include "common.h"
#include "LibUsbContext.h"
#include "UsbDeviceListener.h"
#include "HotplugDetector.h"

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

class UsbDevice {

	enum class TransferState {
		PENDING,
		COMPLETED,
		FAILED
	};

	LibUsbContext& ctx_;
	libusb_device_handle *devh_;

	uint16_t vendor_;
	uint16_t product_;
	uint8_t ifNumber_;
	uint8_t epNumber_;
	uint8_t epDirection_;
	uint8_t epType_;

	TransferState lastTransferState_;

	static void transfer_callback(libusb_transfer *transfer);
	void SubmitTransfer(Data& buffer);
	void OnTransferFailed(libusb_transfer *transfer);
	void OnTransferCompleted(libusb_transfer *transfer);

protected:
	void Open();
	void Close();

	void ReceiveData(size_t dataSize);

	virtual void OnDataReceived(const Data& data) = 0;

public:
	UsbDevice(LibUsbContext& ctx, const DeviceDescription& device);
	virtual ~UsbDevice();
};

class HotPluggableUsbDevice : public UsbDevice
{
	HotPlugDetector *detector_;

public:
	HotPluggableUsbDevice(LibUsbContext& ctx, const DeviceDescription& device);
	virtual ~HotPluggableUsbDevice() override;

	void WaitForArrived();
	bool IsPresent();
};

#endif /* USBDEVICE_H_ */
