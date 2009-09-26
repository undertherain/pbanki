#include "Deck.h"
#include "Exceptions.h"

namespace Model
{
namespace Implementation
{

CBaseDeck::CBaseDeck(const std::string& strInit)
{
	m_strDeckName = strInit;	//TODO: Hardcode must be eliminated.
}

void CBaseDeck::LoadFull(const std::string& strInit)
{
}

std::string CBaseDeck::GetName() const
{
	return m_strDeckName;
}

} //namespace Implementation
} //namespace Model