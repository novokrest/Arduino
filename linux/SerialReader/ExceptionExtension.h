#ifndef EXCEPTIONEXTENSION_H
#define EXCEPTIONEXTENSION_H

#include <string>
#include <sstream>

class ExceptionWithMessage : public std::exception
{
    std::string message_;

public:
    ExceptionWithMessage(const std::string& message);
    const char* what() const throw();
};

class ExceptionExtension
{
    ExceptionExtension() = delete;
    ExceptionExtension(const ExceptionExtension&) = delete;
    ExceptionExtension(const ExceptionExtension&&) = delete;
    ExceptionExtension& operator=(const ExceptionExtension&) = delete;

public:
    static void ThrowExceptionIfFalse(bool b, const std::string& message);
    static void ThrowExceptionIfNotEqual(char a, char b, const std::string& message);

    template<typename T>
    static void ThrowExceptionIfNotEqual(T a, T b, const std::string& message)
    {
        std::stringstream ssError;
        ssError << message
                << ". Expected: '" << a << "'"
                << ", actual: '" << b << "'";
        ThrowExceptionIfFalse(a == b, ssError.str());
    }

    static void ThrowErrnoException();
    static void ThrowErrnoException(const std::string& from);
    static void ThrowErrnoExceptionWithMessage(const std::string& message);
    static void ThrowErrnoExceptionWithMessage(const std::string& message, const std::string& from);
};

#endif // EXCEPTIONEXTENSION_H
