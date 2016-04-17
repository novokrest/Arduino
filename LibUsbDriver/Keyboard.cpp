/*
 * Keyboard.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Utils.h"
#include "Verifiers.h"
#include <cstring>


Keyboard::Keyboard(LibUsbContext& ctx, const DeviceDescription& device)
	: HotPluggableUsbDevice(ctx, device)
{
}

void Keyboard::Start()
{
	WaitForArrived();
	Open();
	while(IsPresent()) {
		ReceiveData(KEYBOARD_BOOT_PROTOCOL_MESSAGE_SIZE);
	}
}

//TODO: Remove data copying
void Keyboard::OnDataReceived(const Data& data)
{
	state_ = data;
	KeyboardStateChanged();
}

void Keyboard::KeyboardStateChanged()
{
	for (auto& observer: observers_) {
		observer->OnKeyboardStateChanged(state_);
	}
}

void Keyboard::AddObserver(KeyboardObserver *observer)
{
	observers_.push_back(observer);
}

Keyboard::~Keyboard()
{
}

KeyboardState::KeyboardState()
{
}

KeyboardState::KeyboardState(const Data& data)
{
	Verifiers::Verify(data.size() == KEYBOARD_BOOT_PROTOCOL_MESSAGE_SIZE, "Unexpected data length");

	memcpy(&modifiers, &data[0], 1);
	bReserved = data[1];
	memcpy((void*)keys, (const void*)&data[2], data.size() - 2);
}

std::string KeyboardState::ToString() const
{
	std::stringstream ss;

	if (modifiers.bmLeftAlt) ss << "LAlt+";
	if (modifiers.bmLeftCtrl) ss << "LCtrl+";
	if (modifiers.bmLeftGUI) ss << "LWin+";
	if (modifiers.bmLeftShift) ss << "LShift+";
	if (modifiers.bmRightAlt) ss << "RAlt+";
	if (modifiers.bmRightCtrl) ss << "RCtrl+";
	if (modifiers.bmRightGUI) ss << "RWin+";
	if (modifiers.bmRightShift) ss << "RShift+";

	ss << "{";
	for (size_t i = 0; i < sizeof(keys); ++i) {
		ss << keys[i];
		if (i != sizeof(keys) - 1) {
			ss << ", ";
		}
	}
	ss << "}";

	return ss.str();
}

KeyboardObserver::KeyboardObserver()
{
}

KeyboardObserver::~KeyboardObserver()
{
}

KeyboardStateTracker::KeyboardStateTracker()
{
}

void KeyboardStateTracker::UpdateKeyboardState(const Data& data)
{
	KeyboardState current(data);

	Logger::Log("Keyboard state: " + current.ToString());
}

