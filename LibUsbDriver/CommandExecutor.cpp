/*
 * CommandExecutor.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#include "CommandExecutor.h"
#include "Logger.h"

CommandExecutor::CommandExecutor()
{
}

CommandExecutor::~CommandExecutor()
{
}

void CommandExecutor::Execute(const std::string& cmd)
{
	Logger::Log("Executing command: " + cmd + "...");
	if (!system(NULL)) {
		Logger::Log("Processor is not available");
		return;
	}

	int result = system(cmd.c_str());
	Logger::Log("Command has been executed: " + std::to_string(result));
}
