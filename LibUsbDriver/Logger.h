/*
 * Logger.h
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

class Logger {
public:
	Logger();
	virtual ~Logger();

	static void Log(std::string const& message);
};

#endif /* LOGGER_H_ */
