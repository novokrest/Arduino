/*
 * Mouse.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: unovokrest
 */

#include "Mouse.h"
#include "Exceptions.h"
#include "Logger.h"
#include "Utils.h"
#include "Verifiers.h"
#include <cstring>

/**
 * Mouse
 * */

Mouse::Mouse(LibUsbContext& ctx, const DeviceDescription& device, bool encrypted)
	: HotPluggableUsbDevice(ctx, device, encrypted)
{
}

Mouse::~Mouse()
{
}

void Mouse::Start()
{
	WaitForArrived();
	Open();
	while(IsPresent()) {
		ReceiveData(MOUSE_BOOT_PROTOCOL_MESSAGE_SIZE);
	}
}

void Mouse::OnDataReceived(const Data& data)
{
	Logger::Log("Mouse data: " + Utils::ToString(data));
	MouseState state(data);
	MouseStateChanged(state);
}

void Mouse::MouseStateChanged(const MouseState& state)
{
	Logger::Log("Mouse state: " + state.ToString());
	for (auto& observer: observers_) {
		observer->OnMouseStateChanged(state);
	}
}

void Mouse::AddMouseObserver(MouseObserver *observer)
{
	observers_.push_back(observer);
}

void Mouse::AddObserver(UsbDeviceObserver *observer)
{
	MouseObserver *mObserver = dynamic_cast<MouseObserver*>(observer);
	AddMouseObserver(mObserver);
}

/**
 * MouseState
 * */

MouseState::MouseState(const Data& data)
{
	memcpy(info_.bInfo, &data[0], sizeof(info_.bInfo));
}

bool MouseState::IsLeftButtonPressed() const
{
	return info_.mouseInfo.bmLeftButton;
}

bool MouseState::IsRightButtonPressed() const
{
	return info_.mouseInfo.bmRightButton;
}

bool MouseState::IsMiddleButtonPressed() const
{
	return info_.mouseInfo.bmMiddleButton;
}

int MouseState::GetDisplacementX() const
{
	return info_.mouseInfo.dX;
}

int MouseState::GetDisplacementY() const
{
	return info_.mouseInfo.dY;
}

std::string MouseState::ToString() const
{
	std::stringstream ss;

	if (IsLeftButtonPressed()) {
		ss << "Left+";
	}

	if (IsRightButtonPressed()) {
		ss << "Right+";
	}

	if (IsMiddleButtonPressed()) {
		ss << "Middle+";
	}

	ss << "(" << GetDisplacementX() << ", " << GetDisplacementY() << ")";

	return ss.str();
}

/**
 * MouseObserver
 * */

MouseObserver::MouseObserver()
{
}

MouseObserver::~MouseObserver()
{
}

