#ifndef __EXCEPTIONS_H_INCLUDED__
#define __EXCEPTIONS_H_INCLUDED__

#include <stdexcept>

namespace Utils
{

/**
@brief The most general exception class.
*/
class GeneralError : public std::runtime_error
{
 public:
	GeneralError(const char* szErrorMessage);

	virtual const char* what() const throw();
};

/**
@brief IO exception.
*/
class IOError : public GeneralError
{
 public:
	IOError(const char* szErrorMessage);

	virtual const char* what() const throw();
};

/**
@brief The most common deserialization error class.
@descr IOError is provided to represent only IO errors!
*/
class ParseError : public GeneralError
{
 public:
	ParseError(const char* szErrorMessage);

	virtual const char* what() const throw();
};

} //namespace Utils

#endif //__EXCEPTIONS_H_INCLUDED__