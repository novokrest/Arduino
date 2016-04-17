/*
 * KeyCodes.h
 *
 *  Created on: Apr 17, 2016
 *      Author: root
 */

#ifndef KEYCODES_H_
#define KEYCODES_H_

#include "common.h"

enum class KeyboardKey
{
	a, A,
	b, B,
	c, C,
	d, D,
	e, E,
	f, F,
	g, G,
	h, H,
	i, I,
	j, J,
	k, K,
	l, L,
	m, M,
	n, N,
	o, O,
	p, P,
	q, Q,
	r, R,
	s, S,
	t, T,
	u, U,
	v, V,
	w, W,
	x, X,
	y, Y,
	z, Z,
	num1, Bang,
	num2, At,
	num3, Sharp,
	num4, Dollar,
	num5, Percent,
	num6, Xor,
	num7, And,
	num8, Star,
	num9, LeftParenthesis,
	num0, RightParenthesis,
	Return,
	Escape,
	Backspace,
	Tab,
	Space,
	Minus, Underscore,
	Equal, Plus,
	LeftBracket, LeftBrace,
	RightBracket, RightBrace,
	BackSlash, VerticalLine,
	Semicolon, Colon,
	Quote, DoubleQuote,
	BackQuote, Tilda,
	Comma, LT,
	Dot, GT,
	Slash, Question,
	CapsLock,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	PrintScreen,
	ScrollLock,
	Break,
	Pause,
	Insert,
	Home,
	PageUp,
	Delete,
	End,
	PageDown,
	RightArrow,
	LeftArrow,
	DownArrow,
	UpArrow,
	NumLock,
	KeypadSlash,
	KeypadStar,
	KeypadMinus,
	KeypadPlus,
	KeypadEnter,
	Keypad1, KeypadEnd,
	Keypad2, KeypadDown,
	Keypad3, KeypadPageDown,
	Keypad4, KeypadLeft,
	Keypad5,
	Keypad6, KeypadRight,
	Keypad7, KeypadHome,
	Keypad8, KeypadUp,
	Keypad9, KeypadPageUp,
	Keypad0, KeypadInstert,
	KeypadDot, KeypadDelete,
	LeftControl, RightControl,
	LeftAlt, RightAlt,
	LeftShift, RightShift,
	LeftGUI, RightGUI,
	NONE
};

class KeyCodesConverter {
public:
	static KeyCodesVec ConvertToPS2Set1Scancodes(KeyboardKey key, bool isPressed);

	static KeyCode ConvertToHidUsageId(KeyboardKey key);
	static KeyboardKey ConvertFromHidUsageId(KeyCode hid);
};

#endif /* KEYCODES_H_ */
