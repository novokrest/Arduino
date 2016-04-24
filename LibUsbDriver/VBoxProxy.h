/*
 * VBoxProxy.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef VBOXPROXY_H_
#define VBOXPROXY_H_

#include "common.h"
#include "CommandExecutor.h"
#include "UsbDeviceListener.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <string>

struct MouseEvent
{
	int dx;
	int dy;
	int dz;
	int dw;
	int buttonState;

public:
	MouseEvent();
};

class VBoxProxy
{
	std::string guest_;
	CommandExecutor cmdExecutor_;

public:
	VBoxProxy(const std::string& guest);
	~VBoxProxy();

	void PutScancodes(const Data& scancodes);
	void PutMouseEvent(const MouseEvent& event);
};

//TODO: create VBoxProxy?
class VBoxMouseKeyboardConnector : public KeyboardObserver, public MouseObserver
{
	KeyboardStateTracker keyboardTracker_;
	VBoxProxy& vboxProxy_;

public:
	VBoxMouseKeyboardConnector(VBoxProxy &vboxProxy);
	virtual ~VBoxMouseKeyboardConnector() override;

	void OnKeyboardStateChanged(const KeyboardState& state) override;
	void OnMouseStateChanged(const MouseState& state) override;
};

#endif /* VBOXPROXY_H_ */
