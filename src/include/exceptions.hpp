#ifndef MINDCRAFT_EXCEPTIONS
#define MINDCRAFT_EXCEPTIONS

#include <string>
class Exception
{
private:
	std::string message;
public:
	Exception(std::string _message):message(_message){}
	std::string GetMessage(){return message;}
};
#endif // MINDCRAFT_EXCEPTIONS
