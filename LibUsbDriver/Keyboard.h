/*
 * Keyboard.h
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "UsbDevice.h"
#include "DesCryptor.h"
#include <exception>
#include <string>


enum class DeviceState
{
	PLUGGED,
	UNPLUGGED
};


/**
 * This structure has been obtained form UsbHostShiel/hidboot.h
 * */
class KeyboardState {
	struct {
			uint8_t bmLeftCtrl : 1;
			uint8_t bmLeftShift : 1;
			uint8_t bmLeftAlt : 1;
			uint8_t bmLeftGUI : 1;
			uint8_t bmRightCtrl : 1;
			uint8_t bmRightShift : 1;
			uint8_t bmRightAlt : 1;
			uint8_t bmRightGUI : 1;
	} modifiers;
	uint8_t bReserved;
	uint8_t keys[6];

public:
	KeyboardState();
	KeyboardState(const Data& data);
	std::string ToString() const;
};

class KeyboardObserver
{
protected:
	KeyboardObserver();

public:
	virtual ~KeyboardObserver();
	virtual void OnKeyboardStateChanged(const KeyboardState& state) = 0;
};

class Keyboard : public HotPluggableUsbDevice
{
	typedef std::vector<KeyboardObserver*> KeyboardObservers;
	KeyboardObservers observers_;

	KeyboardState state_;

	void KeyboardStateChanged();

protected:
	virtual void OnDataReceived(const Data& data) override;

public:
	Keyboard(LibUsbContext& ctx, const DeviceDescription& device);
	virtual ~Keyboard();

	void AddObserver(KeyboardObserver *observer);

	void Start();
};

class KeyboardStateTracker
{
	KeyboardState prev_;

public:
	KeyboardStateTracker();
	void UpdateKeyboardState(const Data& data);
};


#endif /* KEYBOARD_H_ */
