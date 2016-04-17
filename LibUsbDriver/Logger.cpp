/*
 * Logger.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Logger.h"
#include <iostream>

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Log(const std::string& message)
{
	std::cout << message << std::endl;
}

void Logger::LogError(const std::string& message)
{
	std::cout << "Error: " << message << std::endl;
}

