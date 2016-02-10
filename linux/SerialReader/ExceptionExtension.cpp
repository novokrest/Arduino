#include "ExceptionExtension.h"
#include <cerrno>
#include <exception>
#include <sstream>
#include <cstring>

ExceptionWithMessage::ExceptionWithMessage(const std::string &message)
    : message_(message)
{

}

const char* ExceptionWithMessage::what() const throw()
{
    return message_.c_str();
}

void ExceptionExtension::ThrowExceptionIfFalse(bool b, const std::string &message)
{
    if (!b) {
        throw ExceptionWithMessage(message);
    }
}

void ExceptionExtension::ThrowExceptionIfNotEqual(char a, char b, const std::string& message)
{
    std::stringstream ssError;
    ssError << message << ". Expected: '" << (int)a << "'" <<
    ", actual: '" << (int)b << "'";
    ThrowExceptionIfFalse(a == b, ssError.str());
}

void ExceptionExtension::ThrowErrnoException()
{
    std::stringstream ssError;
    ssError << "Error '" << errno << "' has occurred: " << strerror(errno);
    throw ExceptionWithMessage(ssError.str());
}

void ExceptionExtension::ThrowErrnoException(const std::string& from)
{
    std::stringstream ssError;
    ssError << "Error '" << errno << "' has occurred: " << strerror(errno) << " from " << from;
    throw ExceptionWithMessage(ssError.str());
}

void ExceptionExtension::ThrowErrnoExceptionWithMessage(const std::string& message)
{
    std::stringstream ssError;
    ssError << message << std::endl;
    ssError << "Error '" << errno << "' has occurred: " << strerror(errno);
    throw ExceptionWithMessage(ssError.str());
}

void ExceptionExtension::ThrowErrnoExceptionWithMessage(const std::string& message, const std::string& from)
{
    std::stringstream ssError;
    ssError << message << std::endl;
    ssError << "Error '" << errno << "' has occurred: " << strerror(errno) << " from " << from;
    throw ExceptionWithMessage(ssError.str());
}
