#include "Exceptions.h"

namespace Utils
{

///////////////////////////////////////////////////////////////////////
GeneralError::GeneralError(const char* szErrorMessage)
: runtime_error(szErrorMessage)
{
}

const char* GeneralError::what() const throw()
{
	return runtime_error::what();
}

///////////////////////////////////////////////////////////////////////
IOError::IOError(const char* szErrorMessage)
: GeneralError(szErrorMessage)
{
}

const char* IOError::what() const throw()
{
	return GeneralError::what();
}

///////////////////////////////////////////////////////////////////////
ParseError::ParseError(const char* szErrorMessage)
: GeneralError(szErrorMessage)
{
}

const char* ParseError::what() const throw()
{
	return GeneralError::what();
}

} //namespace Utils