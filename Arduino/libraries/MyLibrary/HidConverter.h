#ifndef __HIDCONVERTER_H__
#define __HIDCONVERTER_H__

#include "ArduinoDefines.h"

class Modifiers
{
	byte _mod;
public:
	Modifiers(byte mod);

	bool IsCtrlPressed() const;
	bool IsShiftPressed() const;
	bool IsAltPressed() const;
	bool IsGuiPressed() const;

	bool IsAny() const;

	String ToString();
};

class SymbolDesc
{
	String _symbol;
	String _symbolWithShift;

public:
	SymbolDesc(String &symbol, String &symbolWithShift);
	const String& Symbol() const;
	const String& SymbolWithShift() const;
};

class HidStringConverter
{
	String Convert(String strs[2], byte mod);

public:
	String Convert(byte HIDbyte, byte mod);
};

#endif //__HIDCONVERTER_H__