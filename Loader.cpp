#include "Loader.h"
#include "Exceptions.h"
#include "Deck.h"
#include "UserProfile.h"
#include <sstream>

namespace Utils
{
////////////////////////////////////////
//CDeckLoader
CDeckLoader::CDeckLoader(Model::IDeck* ptrDeck)
: m_ptrDeck(ptrDeck)
, m_ptrDeckPtrCnt(new int(1))
{
}

CDeckLoader::CDeckLoader(const CDeckLoader& other)
: m_ptrDeck(other.m_ptrDeck)
, m_ptrDeckPtrCnt(other.m_ptrDeckPtrCnt)
{
	++(*m_ptrDeckPtrCnt);
}

std::string CDeckLoader::GetName() const
{
	return m_ptrDeck->GetName();
}

CDeckLoader::~CDeckLoader()
{
	--(*m_ptrDeckPtrCnt);
	if (*m_ptrDeckPtrCnt == 0)
	{
		delete m_ptrDeck;
		m_ptrDeck = NULL;
	}
}

////////////////////////////////////////
//CLoader
CLoader::CLoader()
{
	
}

DeckList CLoader::GetDeckList()const throw(IOError)
{
	DeckList result;
	result.reserve(3);
	for(int i = 0; i != 3; ++i)
	{
		std::ostringstream stream;
		stream << i;
		CDeckLoader deck(new Model::Implementation::CBaseDeck("DeckName" + stream.str()));
		result.push_back(deck);
	}
	return result;
}

UserInfo CLoader::GetUserInfo() const throw(IOError)
{
	return std::auto_ptr<Model::IUser>(new Model::Implementation::CBaseUser());
}

}