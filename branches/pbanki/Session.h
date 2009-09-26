#ifndef __SESSION_H_INCLUDED__
#define __SESSION_H_INCLUDED__

#include "Loader.h"
#include "ISession.h"
#include "IDeck.h"
#include "IUserProfile.h"

namespace Logic
{

namespace Implementation
{

/**
@brief General session type.
*/
class CBaseSession : public ISession
{
private:
	Utils::CLoader m_loader;
	DeckList m_lstDecks;
	UserInfo m_userProfile;

public:
	CBaseSession(); //TODO: this harcoded constructor MUST be removed

	virtual std::vector<std::string> ShowDeckList() const;
	virtual void Start();
	
	//virtual  ShowCardList() = 0;
};

} //namespace Implementation
} //namespace Logic

#endif //__SESSION_H_INCLUDED__