/*
 * CommandExecutor.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include <string>

class CommandExecutor {
public:
	CommandExecutor();
	virtual ~CommandExecutor();

	void Execute(const std::string& cmd);
};

#endif /* COMMANDEXECUTOR_H_ */
