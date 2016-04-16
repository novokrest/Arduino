/*
 * Exceptions.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "Exceptions.h"

LibUsbException::LibUsbException(const std::string &message)
	: message_(message)
{

}

LibUsbException::~LibUsbException() throw()
{
}

const char *LibUsbException::what() const throw()
{
	return message_.c_str();
}
