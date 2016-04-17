/*
 * Verifiers.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "Verifiers.h"
#include "Exceptions.h"
#include "Logger.h"

Verifiers::Verifiers()
{
}

Verifiers::~Verifiers()
{
}

void Verifiers::Assert(bool b, const std::string& message)
{
	if (!b) {
		Logger::LogError(message);
		throw LibUsbException(message);
	}
}

void Verifiers::Verify(bool b, const std::string& message)
{
	Assert(b, message);
}
