/*
 * Exceptions.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

class LibUsbException : public std::exception
{
private:
	const std::string message_;

public:
	LibUsbException(const std::string &message);
	virtual ~LibUsbException() throw();
	virtual const char *what() const throw();
};

#endif /* EXCEPTIONS_H_ */
