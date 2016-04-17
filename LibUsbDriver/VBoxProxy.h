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
#include <string>


class VBoxProxy
{
	std::string guest_;
	CommandExecutor cmdExecutor_;

public:
	VBoxProxy(const std::string& guest);
	~VBoxProxy();

	void PutScancodes(const Data& scancodes);
};

//TODO: create VBoxProxy?
class VBoxKeyboardConnector : public KeyboardObserver
{
	KeyboardStateTracker keyboardTracker_;
	VBoxProxy& vboxProxy_;

public:
	VBoxKeyboardConnector(VBoxProxy &vboxProxy);
	virtual ~VBoxKeyboardConnector() override;

	void OnKeyboardStateChanged(const KeyboardState& state) override;
};

#endif /* VBOXPROXY_H_ */
