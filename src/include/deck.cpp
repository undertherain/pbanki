#include "deck.hpp"

// ----------------- DeckInfo ------------------------

std::string DeckInfo::GetName()
{
	return name;
}

// ----------------- Deck ----------------------------

DeckInfoList Deck::getDeckList()
{
	DeckInfoList decks;
	decks.push_back(DeckInfo("test1"));
	decks.push_back(DeckInfo("test2"));
	decks.push_back(DeckInfo("test3"));
	return decks;
}

Deck * Deck::loadDeck(DeckId id)
{
	Deck * newdeck = new Deck();
	return newdeck;
}


//------------------ QA - card field -----------------
std::string QA::ToString()
{
	return std::string("QA ToString stub value");
}

