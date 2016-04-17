/*
 * VBoxProxy.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "VBoxProxy.h"
#include "Utils.h"
#include <sstream>

VBoxProxy::VBoxProxy(const std::string& guest)
	: guest_(guest)
{
}

VBoxProxy::~VBoxProxy()
{
}

void VBoxProxy::PutScancodes(const Data& scancodes)
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

}
