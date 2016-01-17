#include "HidConverter.h"

#define LEFT_CTRL   0x01
#define LEFT_SHIFT  0x02
#define LEFT_ALT    0x04
#define LEFT_GUI    0x08
#define RIGHT_CTRL  0x10
#define RIGHT_SHIFT 0x20
#define RIGHT_ALT   0x40
#define RIGHT_GUI   0x80

/* Modifiers */
#define FOR_MODIFIERS(DO) \
	DO(LEFT_CTRL,   0x01, "LeftCtrl") \
	DO(LEFT_SHIFT,  0x02, "LeftShift") \
	DO(LEFT_ALT,    0x04, "LeftAlt") \
	DO(LEFT_GUI,    0x08, "LeftGui") \
	DO(RIGHT_CTRL,  0x10, "RightCtrl") \
	DO(RIGHT_SHIFT, 0x20, "RightShift") \
	DO(RIGHT_ALT,   0x40, "RightAlt") \
	DO(RIGHT_GUI,   0x80, "RightGui")

#define LETTER_A 0x04
#define LETTER_Z 0x1D

static const char* keys[255][2] = { 
	{"Reserved", 0},
	{"ErrorRollOver", 0},
	{"a", "A"},
	{"", ""},
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
	{ "", "" },
};

Modifiers::Modifiers(byte mod)
	: _mod(mod)
{

}

bool Modifiers::IsCtrlPressed() const
{
	return (_mod & LEFT_CTRL) || (_mod & RIGHT_CTRL);
}

bool Modifiers::IsShiftPressed() const
{
	return (_mod & LEFT_SHIFT) || (_mod & RIGHT_SHIFT);
}

bool Modifiers::IsAltPressed() const
{
	return (_mod & LEFT_ALT) || (_mod & RIGHT_ALT);
}

bool Modifiers::IsGuiPressed() const
{
	return (_mod & LEFT_GUI) || (_mod & RIGHT_GUI);
}

bool Modifiers::IsAny() const
{
	return _mod;
}

String Modifiers::ToString()
{
	String s;
	bool isFirst = true;

#define MOD_PRINT(mod, hex, str) \
	if (mod & _mod) { \
		if (isFirst) { isFirst = false; } \
		else { s += "+"; } \
		s += String(str); \
	}

	FOR_MODIFIERS(MOD_PRINT)

#undef MOD_PRINT
	
	return s;
}

SymbolDesc::SymbolDesc(String &symbol, String &symbolWithShift)
	: _symbol(symbol), _symbolWithShift(symbolWithShift)
{

}

const String& SymbolDesc::Symbol() const
{
	return _symbol;
}

const String& SymbolDesc::SymbolWithShift() const
{
	return _symbolWithShift;
}

String HidStringConverter::Convert(byte hidByte, byte mod)
{
	String strs[2];

	strs[0] = "default";
	strs[1] = "DEFAULT";

	//letters a-z
	if (hidByte >= LETTER_A && hidByte <= LETTER_Z) {
		const char a = 'a';
		const char A = 'A';
		strs[0] = String(char(a + (hidByte - LETTER_A)));
		strs[1] = String(char(A + (hidByte - LETTER_A)));
	}

	return Convert(strs, mod);

	///* upper row of the keyboard, numbers and special symbols */
	//if (HIDbyte >= 0x1e && HIDbyte <= 0x27) {
	//	if ((mod & SHIFT) || numLock) {    //shift key pressed
	//		switch (HIDbyte) {
	//		case BANG:  return(0x21);
	//		case AT:    return(0x40);
	//		case POUND: return(0x23);
	//		case DOLLAR: return(0x24);
	//		case PERCENT: return(0x25);
	//		case CAP: return(0x5e);
	//		case AND: return(0x26);
	//		case STAR: return(0x2a);
	//		case OPENBKT: return(0x28);
	//		case CLOSEBKT: return(0x29);
	//		}//switch( HIDbyte...
	//	}
	//	else {                  //numbers
	//		if (HIDbyte == 0x27) {  //zero
	//			return(0x30);
	//		}
	//		else {
	//			return(HIDbyte + 0x13);
	//		}
	//	}//numbers
	//}//if( HIDbyte >= 0x1e && HIDbyte <= 0x27
	// /**/
	// /* number pad. Arrows are not supported */
	//if ((HIDbyte >= 0x59 && HIDbyte <= 0x61) && (numLock == true)) {  // numbers 1-9
	//	return(HIDbyte - 0x28);
	//}
	//if ((HIDbyte == 0x62) && (numLock == true)) {                      //zero
	//	return(0x30);
	//}
	///* Letters a-z */
	//if (HIDbyte >= 0x04 && HIDbyte <= 0x1d) {
	//	if (((capsLock == true) && (mod & SHIFT) == 0) || ((capsLock == false) && (mod & SHIFT))) {  //upper case
	//		return(HIDbyte + 0x3d);
	//	}
	//	else {  //lower case
	//		return(HIDbyte + 0x5d);
	//	}
	//}//if( HIDbyte >= 0x04 && HIDbyte <= 0x1d...
	// /* Other special symbols */
	//if (HIDbyte >= 0x2c && HIDbyte <= 0x38) {
	//	switch (HIDbyte) {
	//	case SPACE: return(0x20);
	//	case HYPHEN:
	//		if ((mod & SHIFT) == false) {
	//			return(0x2d);
	//		}
	//		else {
	//			return(0x5f);
	//		}
	//	case EQUAL:
	//		if ((mod & SHIFT) == false) {
	//			return(0x3d);
	//		}
	//		else {
	//			return(0x2b);
	//		}
	//	case SQBKTOPEN:
	//		if ((mod & SHIFT) == false) {
	//			return(0x5b);
	//		}
	//		else {
	//			return(0x7b);
	//		}
	//	case SQBKTCLOSE:
	//		if ((mod & SHIFT) == false) {
	//			return(0x5d);
	//		}
	//		else {
	//			return(0x7d);
	//		}
	//	case BACKSLASH:
	//		if ((mod & SHIFT) == false) {
	//			return(0x5c);
	//		}
	//		else {
	//			return(0x7c);
	//		}
	//	case SEMICOLON:
	//		if ((mod & SHIFT) == false) {
	//			return(0x3b);
	//		}
	//		else {
	//			return(0x3a);
	//		}
	//	case INVCOMMA:
	//		if ((mod & SHIFT) == false) {
	//			return(0x27);
	//		}
	//		else {
	//			return(0x22);
	//		}
	//	case TILDE:
	//		if ((mod & SHIFT) == false) {
	//			return(0x60);
	//		}
	//		else {
	//			return(0x7e);
	//		}
	//	case COMMA:
	//		if ((mod & SHIFT) == false) {
	//			return(0x2c);
	//		}
	//		else {
	//			return(0x3c);
	//		}
	//	case PERIOD:
	//		if ((mod & SHIFT) == false) {
	//			return(0x2e);
	//		}
	//		else {
	//			return(0x3e);
	//		}
	//	case FRONTSLASH:
	//		if ((mod & SHIFT) == false) {
	//			return(0x2f);
	//		}
	//		else {
	//			return(0x3f);
	//		}
	//	default:
	//		break;
	//	}//switch( HIDbyte..
	//}//if( HIDbye >= 0x2d && HIDbyte <= 0x38..
	//return(0);
}

String HidStringConverter::Convert(String strs[2], byte mod)
{
	Modifiers modifiers(mod);
	SymbolDesc symbol(strs[0], strs[1]);

	return modifiers.IsAny() ? modifiers.ToString() + "+" + symbol.Symbol() : symbol.Symbol();
}