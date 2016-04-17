/*
 * main.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"
#include "Logger.h"
#include "HotplugDetector.h"
#include "Devices.h"
#include "VBoxProxy.h"
#include <unistd.h>
#include <memory>

enum class EncryptionMode
{
	REQUIRED = 0,
	NONE
};

struct Arguments
{
	EncryptionMode Encryption;
	DeviceType Device;
	std::string VirtualBoxGuestName;

	static Arguments Default()
	{
		Arguments arguments;

		arguments.Encryption = EncryptionMode::NONE;
		arguments.Device = DeviceType::NONE;

		return arguments;
	}

	std::string ToString() const
	{
		static const std::string EncryptionToString[] = {
				[EncryptionMode::REQUIRED] = "required",
				[EncryptionMode::NONE] = "none"
		};

		static const std::string DeviceToString[] = {
				[DeviceType::JUST_KEYBOARD] = "just-keyboard",
				[DeviceType::JUST_ARDUINO] = "just-arduino",
				[DeviceType::ARDUINO_KEYBOARD] = "arduino-keyboard",
				[DeviceType::NONE] = "none"
		};

		std::stringstream ss;

		ss << "Device: " << DeviceToString[(int)Device]
		   << ", encryption: " <<  EncryptionToString[(int)Encryption]
		   << ", guest: " << VirtualBoxGuestName;

		return ss.str();
	}
};

static Arguments ParseArguments(int argc, char * const argv[])
{
	static const std::string DeviceToDeviceType[] = {
			[DeviceType::JUST_KEYBOARD] = "jk",
			[DeviceType::JUST_ARDUINO] = "ja",
			[DeviceType::ARDUINO_KEYBOARD] = "ak",
			[DeviceType::NONE] = "none"
	};

	Arguments arguments = Arguments::Default();

	int opt;
	bool encryptionRequired = false;
	std::string device;
	std::string vbGuestName;

	while ((opt = getopt(argc, argv, "et:g:")) != -1){
		switch(opt){
		case 'e':
			encryptionRequired = true;
			Logger::Log("Encryption required");
			break;

		case 't':
			device = optarg;
			Logger::Log("Device: " + device);
			break;

		case 'g':
			vbGuestName = optarg;
			Logger::Log("VirtualBox guest name: " + vbGuestName);

		case '?':
			Logger::Log("Unknown option: " + std::to_string(opt));
			break;
		}
	};

	arguments.Encryption = encryptionRequired ? EncryptionMode::REQUIRED : EncryptionMode::NONE;
	for (int deviceType = 0, none = (int)DeviceType::NONE; deviceType < none; ++deviceType) {
		if (DeviceToDeviceType[deviceType] == device) {
			arguments.Device = (DeviceType)deviceType;
		}
	}
	arguments.VirtualBoxGuestName = vbGuestName;

	return arguments;
}

//TODO: use auto-ptr
int main(int argc, char * const argv[])
{
	//test();return 0;
	Arguments arguments = ParseArguments(argc, argv);
	Logger::Log(arguments.ToString());

	LibUsbContext context;
	VBoxProxy vbox(arguments.VirtualBoxGuestName);
	VBoxKeyboardConnector connector(vbox);
	std::unique_ptr<Keyboard> keyboard (DevicesCreator::Create(context, arguments.Device, arguments.Encryption == EncryptionMode::REQUIRED));

	keyboard->AddObserver(&connector);

	try{
		keyboard->Start();
	}
	catch (std::exception& exception) {
		Logger::LogError(exception.what());
	}

	return 0;
}


