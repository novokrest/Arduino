/*
 * Verifiers.h
 *
 *  Created on: Apr 16, 2016
 *      Author: root
 */

#ifndef VERIFIERS_H_
#define VERIFIERS_H_

#include <string>

class Verifiers {
	Verifiers();
	~Verifiers();

public:
	static void Assert(bool b, const std::string& message);
	static void Verify(bool b, const std::string& message);
};

#endif /* VERIFIERS_H_ */
