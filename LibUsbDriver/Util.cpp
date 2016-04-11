/*
 * Util.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Util.h"
#include <sstream>

Util::Util() {
	// TODO Auto-generated constructor stub

}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

std::string Util::CharsToIntString(unsigned char *data, size_t size)
{
	std::stringstream ss;

	ss << '{';
	for (size_t i = 0; i < size; ++i) {
		ss << ' ' << (int)data[i];
	}
	ss << " }";

	return ss.str();
}
