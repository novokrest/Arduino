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

void VBoxProxy::PutMouseEvent(const MouseEvent& event)
{
	Logger::Log("Nothing");
}

VBoxMouseKeyboardConnector::VBoxMouseKeyboardConnector(VBoxProxy& vboxProxy)
	: vboxProxy_(vboxProxy)
{
}

VBoxMouseKeyboardConnector::~VBoxMouseKeyboardConnector()
{
}

void VBoxMouseKeyboardConnector::OnKeyboardStateChanged(const KeyboardState& state)
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

//TODO: pass in MouseInfo?
void VBoxMouseKeyboardConnector::OnMouseStateChanged(const MouseState& state)
{
	MouseEvent mouseEvent;

	mouseEvent.dx = state.GetDisplacementX();
	mouseEvent.dy = state.GetDisplacementY();
	mouseEvent.buttonState = (state.IsLeftButtonPressed() ? MOUSE_LEFT_BUTTON_MASK : 0)
							 | (state.IsRightButtonPressed() ? MOUSE_RIGHT_BUTTON_MASK : 0)
							 | (state.IsMiddleButtonPressed() ? MOUSE_MIDDLE_BUTTON_MASK : 0);

	vboxProxy_.PutMouseEvent(mouseEvent);
}


/**
 * MouseEvent
 * */

MouseEvent::MouseEvent()
	: dx(0), dy(0), dz(0), dw(0), buttonState(0)
{
}
