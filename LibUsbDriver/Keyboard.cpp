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
	Logger::Log(state_.ToString());
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

	//Fill set of keyboard's key
	if (modifiers.bmLeftCtrl) keySet_.insert(KeyboardKey::LeftControl);
	if (modifiers.bmLeftShift) keySet_.insert(KeyboardKey::LeftShift);
	if (modifiers.bmLeftAlt) keySet_.insert(KeyboardKey::LeftAlt);
	if (modifiers.bmLeftGUI) keySet_.insert(KeyboardKey::LeftGUI);
	if (modifiers.bmRightCtrl) keySet_.insert(KeyboardKey::RightControl);
	if (modifiers.bmRightShift) keySet_.insert(KeyboardKey::RightShift);
	if (modifiers.bmRightAlt) keySet_.insert(KeyboardKey::RightAlt);
	if (modifiers.bmRightGUI) keySet_.insert(KeyboardKey::RightGUI);
}

const KeySet& KeyboardState::Keys() const
{
	return keySet_;
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
		ss << (int)keys[i];
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

KeysReport::KeysReport(const KeySet& pressed, const KeySet& released)
	: pressed_(pressed), released_(released)
{

}

const KeySet& KeysReport::Pressed() const
{
	return pressed_;
}

const KeySet& KeysReport::Released() const
{
	return released_;
}

KeyboardStateTracker::KeyboardStateTracker()
{
}

KeysReport KeyboardStateTracker::UpdateKeyboardState(const KeyboardState& state)
{
	KeySet newpressed, released;

	for (auto& key: pressed_) {
		if (state.Keys().find(key) == state.Keys().end()) {
			pressed_.erase(key);
			released.insert(key);
		}
	}

	for (auto& key: state.Keys()) {
		if (pressed_.find(key) == pressed_.end()) {
			pressed_.insert(key);
			newpressed.insert(key);
		}
	}

	return KeysReport(newpressed, released);
}

