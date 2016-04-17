/*
 * KeyCodes.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: root
 */

#include "KeyCodes.h"
#include <map>

static const std::map<KeyboardKey, KeyCodesVec> KeyToPS2Set1MakeScancodesMap {
	{KeyboardKey::a/*, A*/, {0x1E}},
	{KeyboardKey::b/*, B*/, {0x30}},
	{KeyboardKey::c/*, C*/, {0x2E}},
	{KeyboardKey::d/*, D*/, {0x20}},
	{KeyboardKey::e/*, E*/, {0x12}},
	{KeyboardKey::f/*, F*/, {0x21}},
	{KeyboardKey::g/*, G*/, {0x22}},
	{KeyboardKey::h/*, H*/, {0x23}},
	{KeyboardKey::i/*, I*/, {0x17}},
	{KeyboardKey::j/*, J*/, {0x24}},
	{KeyboardKey::k/*, K*/, {0x25}},
	{KeyboardKey::l/*, L*/, {0x26}},
	{KeyboardKey::m/*, M*/, {0x32}},
	{KeyboardKey::n/*, N*/, {0x31}},
	{KeyboardKey::o/*, O*/, {0x18}},
	{KeyboardKey::p/*, P*/, {0x19}},
	{KeyboardKey::q/*, Q*/, {0x10}},
	{KeyboardKey::r/*, R*/, {0x13}},
	{KeyboardKey::s/*, S*/, {0x1F}},
	{KeyboardKey::t/*, T*/, {0x14}},
	{KeyboardKey::u/*, U*/, {0x16}},
	{KeyboardKey::v/*, V*/, {0x2F}},
	{KeyboardKey::w/*, W*/, {0x11}},
	{KeyboardKey::x/*, X*/, {0x2D}},
	{KeyboardKey::y/*, Y*/, {0x15}},
	{KeyboardKey::z/*, Z*/, {0x2C}},
	{KeyboardKey::num1/*, Bang*/, {0x02}},
	{KeyboardKey::num2/*, At*/, {0x03}},
	{KeyboardKey::num3/*, Sharp*/, {0x04}},
	{KeyboardKey::num4/*, Dollar*/, {0x05}},
	{KeyboardKey::num5/*, Percent*/, {0x06}},
	{KeyboardKey::num6/*, Xor*/, {0x07}},
	{KeyboardKey::num7/*, And*/, {0x08}},
	{KeyboardKey::num8/*, Star*/, {0x09}},
	{KeyboardKey::num9/*, LeftParenthesis*/, {0x0A}},
	{KeyboardKey::num0/*, RightParenthesis*/, {0x0B}},
	{KeyboardKey::Return, {0x1C}},
	{KeyboardKey::Escape, {0x01}},
	{KeyboardKey::Backspace, {0x0E}},
	{KeyboardKey::Tab, {0x0F}},
	{KeyboardKey::Space, {0x39}},
	{KeyboardKey::Minus/*, Underscore,*/, {0x0C}},
	{KeyboardKey::Equal/*, Plus*/, {0x0D}},
	{KeyboardKey::LeftBracket/*, LeftBrace*/, {0x1A}},
	{KeyboardKey::RightBracket/*, RightBrace*/, {0x1B}},
	{KeyboardKey::BackSlash/*, VerticalLine*/, {0x2B}},
	{KeyboardKey::Semicolon/*, Colon*/, {0x27}},
	{KeyboardKey::Quote/*, DoubleQuote*/, {0x28}},
	{KeyboardKey::BackQuote/*, Tilda*/, {0x29}},
	{KeyboardKey::Comma/*, LT*/, {0x33}},
	{KeyboardKey::Dot/*, GT*/, {0x34}},
	{KeyboardKey::Slash/*, Question*/, {0x35}},
	{KeyboardKey::CapsLock, {0x3A}},
	{KeyboardKey::F1, {0x3B}},
	{KeyboardKey::F2, {0x3C}},
	{KeyboardKey::F3, {0x3D}},
	{KeyboardKey::F4, {0x3E}},
	{KeyboardKey::F5, {0x3F}},
	{KeyboardKey::F6, {0x40}},
	{KeyboardKey::F7, {0x41}},
	{KeyboardKey::F8, {0x42}},
	{KeyboardKey::F9, {0x43}},
	{KeyboardKey::F10, {0x44}},
	{KeyboardKey::F11, {0x57}},
	{KeyboardKey::F12, {0x58}},
	{KeyboardKey::PrintScreen, {0xE0, 0x37}},
	{KeyboardKey::ScrollLock, {0x46}},
	{KeyboardKey::Break, {0xE0, 0x46,   0xE0, 0xC6}},
	{KeyboardKey::Pause, {0xE1, 0x1D, 0x45,   0xE1, 0x9D, 0xC5}},
	{KeyboardKey::Insert, {0xE0, 0x52}},
	{KeyboardKey::Home, {0xE0, 0x47}},
	{KeyboardKey::PageUp, {0xE0, 0x49}},
	{KeyboardKey::Delete, {0xE0, 0x53}},
	{KeyboardKey::End, {0xE0, 0x4F}},
	{KeyboardKey::PageDown, {0xE0, 0x51}},
	{KeyboardKey::RightArrow, {0xE0, 0x4D}},
	{KeyboardKey::LeftArrow, {0xE0, 0x4B}},
	{KeyboardKey::DownArrow, {0xE0, 0x50}},
	{KeyboardKey::UpArrow, {0xE0, 0x48}},
	{KeyboardKey::NumLock, {0x45}},
	{KeyboardKey::KeypadSlash, {0xE0, 0x35}},
	{KeyboardKey::KeypadStar, {0x37}},
	{KeyboardKey::KeypadMinus, {0x4A}},
	{KeyboardKey::KeypadPlus, {0x4E}},
	{KeyboardKey::KeypadEnter, {0xE0, 0x1C}},
	{KeyboardKey::Keypad1/*, KeypadEnd*/, {0x4F}},
	{KeyboardKey::Keypad2/*, KeypadDown*/, {0x50}},
	{KeyboardKey::Keypad3/*, KeypadPageDown*/, {0x51}},
	{KeyboardKey::Keypad4/*, KeypadLeft*/, {0x4B}},
	{KeyboardKey::Keypad5, {0x4C}},
	{KeyboardKey::Keypad6/*, KeypadRight*/, {0x4D}},
	{KeyboardKey::Keypad7/*, KeypadHome*/, {0x47}},
	{KeyboardKey::Keypad8/*, KeypadUp*/, {0x48}},
 	{KeyboardKey::Keypad9/*, KeypadPageUp*/, {0x49}},
	{KeyboardKey::Keypad0/*, KeypadInstert*/, {0x52}},
	{KeyboardKey::KeypadDot/*, KeypadDelete*/, {0x53}},
	{KeyboardKey::LeftControl, {0x1D}},
	{KeyboardKey::LeftShift, {0x2A}},
	{KeyboardKey::LeftAlt, {0x38}},
	{KeyboardKey::LeftGUI, {0xE0, 0x5B}},
	{KeyboardKey::RightControl, {0xE0, 0x1D}},
	{KeyboardKey::RightShift, {0x36}},
	{KeyboardKey::RightAlt, {0xE0, 0x38}},
	{KeyboardKey::RightGUI, {0xE0, 0x5C}},
};

static const std::map<KeyboardKey, KeyCodesVec> KeyToPS2Set1BreakScancodesMap {

};

KeyCodesVec KeyCodesConverter::ConvertToPS2Set1Scancodes(KeyboardKey key, bool isPressed)
{
	if (isPressed) {
		return KeyToPS2Set1MakeScancodesMap.at(key);
	}

	return KeyToPS2Set1BreakScancodesMap.at(key);
}
