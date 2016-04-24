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


Keyboard::Keyboard(LibUsbContext& ctx, const DeviceDescription& device, bool encrypted)
	: HotPluggableUsbDevice(ctx, device, encrypted)
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
	//state_ = data;
	KeyboardState state(data);
	KeyboardStateChanged(state);
}

void Keyboard::KeyboardStateChanged(const KeyboardState& state)
{
	Logger::Log("Keyboard state: " + state.ToString());
	for (auto& observer: observers_) {
		observer->OnKeyboardStateChanged(state);
	}
}

void Keyboard::AddKeyboardObserver(KeyboardObserver *observer)
{
	observers_.push_back(observer);
}

void Keyboard::AddObserver(UsbDeviceObserver *observer)
{
	KeyboardObserver *kObserver = dynamic_cast<KeyboardObserver*>(observer);
	AddKeyboardObserver(kObserver);
}

Keyboard::~Keyboard()
{
}

KeyboardState::KeyboardState()
{
	modifiers.bmLeftCtrl = 0;
	modifiers.bmLeftShift = 0;
	modifiers.bmLeftAlt = 0;
	modifiers.bmLeftGUI = 0;
	modifiers.bmRightCtrl = 0;
	modifiers.bmRightShift = 0;
	modifiers.bmRightAlt = 0;
	modifiers.bmRightGUI = 0;

	for (size_t i = 0; i < sizeof(keys); ++i) {
		keys[i] = 0;
	}
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

	for (size_t i = 0; i < sizeof(keys); ++i) {
		KeyCode keyCode = keys[i];
		if (keyCode) {
			keySet_.insert(KeyCodesConverter::ConvertFromHidUsageId(keys[i]));
		}
	}
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

KeysReport::KeysReport()
{
}

KeysReport::KeysReport(const KeySet& pressed, const KeySet& released)
	: pressed_(pressed), released_(released)
{
}

KeysReport::KeysReport(const KeysReport& report)
	: pressed_(report.pressed_), released_(report.released_)
{
}

KeysReport& KeysReport::operator=(const KeysReport& report)
{
	pressed_ = report.pressed_;
	released_ = report.released_;

	return *this;
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
			released.insert(key);
		}
	}

	for (auto& key: released) {
		pressed_.erase(key);
	}

	for (auto& key: state.Keys()) {
		if (pressed_.find(key) == pressed_.end()) {
			pressed_.insert(key);
			newpressed.insert(key);
		}
	}

	return KeysReport(newpressed, released);
}

