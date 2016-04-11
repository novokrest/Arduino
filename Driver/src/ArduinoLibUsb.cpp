//============================================================================
// Name        : ArduinoLibUsb.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <libusb-1.0/libusb.h>
using std::cout;
using std::endl;

#define VENDOR_ID 0x2341
#define PRODUCT_ID 0x0001

void printDevice(libusb_device *dev)
{
	libusb_device_descriptor desc;

	int r = libusb_get_device_descriptor(dev, &desc);
	if (r < 0) {
		cout << "Failed to get device descriptor: " << r << endl;
		return;
	}

	cout<<"Number of possible configurations: "<<(int)desc.bNumConfigurations<<"  ";
    cout<<"Device Class: "<<(int)desc.bDeviceClass<<"  ";
	cout<<"VendorID: "<< std::hex << desc.idVendor<<"  ";
	cout<<"ProductID: "<<desc.idProduct<<endl;

	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);
	cout<<"Interfaces: "<<(int)config->bNumInterfaces<<" ||| ";
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	for(int i=0; i<(int)config->bNumInterfaces; i++) {
		inter = &config->interface[i];
	    cout<<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
	    for(int j=0; j<inter->num_altsetting; j++) {
	    	interdesc = &inter->altsetting[j];
	        cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
	        cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<<" | ";
	        for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
	        	epdesc = &interdesc->endpoint[k];
	            cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
	            cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<" | ";
	        }
	    }
	}
	cout<<endl<<endl<<endl;
	libusb_free_config_descriptor(config);
}

int findDevices()
{
	libusb_device **devs;
	libusb_device_handle *devh;
	libusb_context *ctx = NULL;
	int r;
	ssize_t cnt;

	r = libusb_init(&ctx);
	if (r < 0) {
		cout << "libusb_init error: " << r << endl;
		return 1;
	}

	libusb_set_debug(ctx, 3);

	cnt = libusb_get_device_list(ctx, &devs);
	if (cnt < 0) {
		cout << "Get devices error: " << r << endl;
	}

	cout << cnt << " devices in list" << endl;
	for (int i = 0; i < cnt; ++i) {
		printDevice(devs[i]);
	}

	devh = libusb_open_device_with_vid_pid(ctx, 0x2341, 0x0001);
	//devh = libusb_open_device_with_vid_pid(ctx, 0x03eb, 0x2042);
	if (devh == NULL) {
		cout << "Failed to open device" << endl;
	}
	else {
		cout << "Device is opened" << endl;
	}
	libusb_free_device_list(devs, 1);

	if (libusb_kernel_driver_active(devh, 0) == 1) {
		cout << "Kernel Driver Active" << endl;

		if (libusb_detach_kernel_driver(devh, 0) == 0) {
			cout << "Kernel driver is detached" << endl;
		}
	}

	r = libusb_claim_interface(devh, 1);
	//r = libusb_claim_interface(devh, 0);
	if (r < 0) {
		cout << "Cannot claim interface" << endl;
	}

	for (int i = 0; i < 1; ++i) {
	unsigned char data[8] = { 0 };
	int count = 0;
	r = libusb_bulk_transfer(devh, (0x83 | LIBUSB_ENDPOINT_IN), data, sizeof(data), &count, 0);
	//r = libusb_bulk_transfer(devh, (0x81 | LIBUSB_ENDPOINT_IN), data, sizeof(data), &count, 0);
	cout << "Count: " << count << endl;
	if (r == 0) {
		cout << "Transfer successfull" << endl;
		cout << (int)data[2] << endl;
	}
	else {
		cout << "Transfer error: " << r << endl;
	}
	}

	r = libusb_release_interface(devh, 1);
	//r = libusb_release_interface(devh, 0);
	if (r != 0) {
		cout << "Cannot release interface" << endl;
	}

	libusb_attach_kernel_driver(devh, 1);

	libusb_close(devh);
	libusb_exit(ctx);

	return 0;
}


int test1() {
	struct libusb_device_handle *devh = NULL;
	int r = 0;

	r = libusb_init(NULL);
	if (r < 0) {
		cout << "Failed to initialize libusb" << endl;
		return -1;
	}

	devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
	if (devh == NULL) {
		cout << "Failed to open device" << endl;
		goto error;
	}

	libusb_close(devh);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

error:
	libusb_exit(NULL);

	return 0;
}

int main() {

	findDevices();
	return 0;
}
