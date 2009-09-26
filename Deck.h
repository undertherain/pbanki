#ifndef __DECK_H_INCLUDED__
#define __DECK_H_INCLUDED__

#include "IDeck.h"
#include <string>

namespace Utils
{
	class ParseError;
}

namespace Model
{
namespace Implementation
{

/**
@brief Default (base) deck class.
@descr On construction this class is not fully loaded.
	   It contains only information which should be
	   requested in the moment when deck is chosen by
	   the user.
*/
class CBaseDeck : public IDeck
{
private:
	std::string m_strDeckName;

public:
	CBaseDeck(const std::string& strInit);

	virtual void LoadFull(const std::string& strInit);
	virtual std::string GetName() const;
};

} //namespace Implementation
} //namespace Model

#endif //__DECK_H_INCLUDED__