/*
 * Mouse.h
 *
 *  Created on: Apr 24, 2016
 *      Author: unovokrest
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include "UsbDevice.h"
#include "DesCryptor.h"
#include <exception>
#include <string>

struct MouseInfo
{
    struct {
            uint8_t bmLeftButton : 1;
            uint8_t bmRightButton : 1;
            uint8_t bmMiddleButton : 1;
            uint8_t bmDummy : 5;
    };
    int8_t dX;
    int8_t dY;
};

class MouseState
{
	union {
		MouseInfo mouseInfo;
		uint8_t bInfo[sizeof(MouseInfo)];
	} info_;

public:
	MouseState(const Data& data);

	bool IsLeftButtonPressed() const;
	bool IsRightButtonPressed() const;
	bool IsMiddleButtonPressed() const;

	int GetDisplacementX() const;
	int GetDisplacementY() const;

	std::string ToString() const;
};

class MouseObserver : public virtual UsbDeviceObserver
{
protected:
	MouseObserver();

public:
	virtual ~MouseObserver();
	virtual void OnMouseStateChanged(const MouseState& state) = 0;
};

class Mouse : public HotPluggableUsbDevice
{
	typedef std::vector<MouseObserver*> MouseObservers;

	MouseObservers observers_;

	void MouseStateChanged(const MouseState& state);
	void AddMouseObserver(MouseObserver* observer);

protected:
	virtual void OnDataReceived(const Data& data) override;

public:
	Mouse(LibUsbContext& ctx, const DeviceDescription& device, bool encrypted);
	virtual ~Mouse() override;

	void AddObserver(UsbDeviceObserver* observer);
	void Start();
};

#endif /* MOUSE_H_ */
