/*
 * VBoxProxy.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "VBoxProxy.h"
#include "Logger.h"
#include "Utils.h"
#include <sstream>

VBoxProxy::VBoxProxy(const std::string& guest)
	: guest_(guest)
{
}

VBoxProxy::~VBoxProxy()
{
}

void VBoxProxy::PutScancodes(const KeyCodesVec& scancodes)
{
	std::stringstream command;

	command << "VBoxManage controlvm " << guest_ << " keyboardputscancode ";
	command << Utils::ToHexString(scancodes, " ");

	cmdExecutor_.Execute(command.str());
}

VBoxKeyboardConnector::VBoxKeyboardConnector(VBoxProxy& vboxProxy)
	: vboxProxy_(vboxProxy)
{
}

VBoxKeyboardConnector::~VBoxKeyboardConnector()
{
}

void VBoxKeyboardConnector::OnKeyboardStateChanged(const KeyboardState& state)
{
	KeysReport report = keyboardTracker_.UpdateKeyboardState(state);
	KeyCodesVec scancodes;

	for (auto& key: report.Released()) {
		KeyCodesVec scancode = KeyCodesConverter::ConvertToPS2Set1Scancodes(key, false);
		scancodes.insert(scancodes.begin(), scancode.begin(), scancode.end());
	}

	for (auto& key: report.Pressed()) {
		KeyCodesVec scancode = KeyCodesConverter::ConvertToPS2Set1Scancodes(key, true);
		scancodes.insert(scancodes.begin(), scancode.begin(), scancode.end());
	}

	vboxProxy_.PutScancodes(scancodes);
}
