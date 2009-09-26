#ifndef __ISESSION_H_INCLUDED__
#define __ISESSION_H_INCLUDED__

#include <string>
#include <vector>

namespace Logic
{

/**
@brief General interface for different kind of sessions.
@note I am not sure that different kinds of controllers are
	  needed but flexibility is never enough. :)
*/
class ISession
{
public:
	virtual std::vector<std::string> ShowDeckList() const = 0;
	virtual void Start() = 0;
};

} //namespace Logic

#endif //__ISESSION_H_INCLUDED__