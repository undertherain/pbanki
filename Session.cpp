#include "Session.h"

namespace Logic
{
namespace Implementation
{

////////////////////////////
//CBaseSession
CBaseSession::CBaseSession()
: m_userProfile(m_loader.GetUserInfo())
, m_lstDecks(m_loader.GetDeckList())
{
}

std::vector<std::string> CBaseSession::ShowDeckList() const
{
	std::vector<std::string> result;
	result.reserve(m_lstDecks.size());
	DeckList::const_iterator it = m_lstDecks.begin();
	for (; it != m_lstDecks.end(); ++it)
	{
		result.push_back(it->GetName());
	}
	return result;
}

void CBaseSession::Start()
{
	//TODO: some processes (initialization) should be performed right here.
}

} //namespace Implementation
} //namespace Logic