/*
 * Util.h
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cstdlib>
#include <string>

class Util {
public:
	Util();
	virtual ~Util();

	static std::string CharsToIntString(unsigned char *data, size_t size);
};

#endif /* UTIL_H_ */
