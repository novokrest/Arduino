/*
 * main.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"
#include "Logger.h"
#include <unistd.h>

enum class EncryptionMode
{
	REQUIRED = 0,
	NONE
};

enum class DeviceType
{
	JUST_KEYBOARD = 0,
	ARDUINO_KEYBOARD,
	NONE
};

struct Arguments
{
	EncryptionMode Encryption;
	DeviceType Device;

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
				[DeviceType::ARDUINO_KEYBOARD] = "arduino-keyboard",
				[DeviceType::NONE] = "none"
		};

		std::stringstream ss;

		ss << "Device: " << DeviceToString[(int)Device];
		ss << ", encryption: " <<  EncryptionToString[(int)Encryption];

		return ss.str();
	}
};

static Arguments ParseArguments(int argc, char * const argv[])
{
	static const std::string DeviceFlagToDeviceType[] = {
			[DeviceType::JUST_KEYBOARD] = "jk",
			[DeviceType::ARDUINO_KEYBOARD] = "ak",
			[DeviceType::NONE] = "none"
	};

	Arguments arguments = Arguments::Default();

	int opt;
	bool encryptionRequired = false;
	std::string deviceCode;
	while ((opt = getopt(argc, argv, "et:")) != -1){
		switch(opt){
		case 'e':
			encryptionRequired = true;
			Logger::Log("Encryption required");
			break;

		case 't':
			deviceCode = optarg;
			Logger::Log("Device: " + deviceCode);
			break;

		case '?':
			Logger::Log("Unknown option: " + std::to_string(opt));
			break;
		}
	};

	arguments.Encryption = encryptionRequired ? EncryptionMode::REQUIRED : EncryptionMode::NONE;
	for (int device = 0, none = (int)DeviceType::NONE; device < none; ++device) {
		if (DeviceFlagToDeviceType[device] == deviceCode) {
			arguments.Device = (DeviceType)device;
		}
	}

	return arguments;
}

int main(int argc, char * const argv[])
{
	Arguments arguments = ParseArguments(argc, argv);
	Logger::Log(arguments.ToString());

	//Keyboard keyboard(ArduinoKeyboardDescription);
	if (arguments.Device == DeviceType::JUST_KEYBOARD) {
		Keyboard keyboard(JustKeyboardDecription, arguments.Encryption == EncryptionMode::REQUIRED);
		keyboard.Start();
	}

	else if (arguments.Device == DeviceType::ARDUINO_KEYBOARD) {
		Keyboard keyboard(ArduinoKeyboardDescription, arguments.Encryption == EncryptionMode::REQUIRED);
		keyboard.Start();
	}

	Logger::Log("Nothing");

	return 0;
}


