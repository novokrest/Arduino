/*
 * Logger.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Logger.h"
#include <iostream>

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::Log(const std::string& message)
{
	std::cout << message << std::endl;
}

