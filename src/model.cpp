#include "model.hpp"

DeckInfoList Model::getDeckList()
{
	
	return Deck::getDeckList();
	
}

void Model::loadDeck(DeckId id)
{
	currentDeck=Deck::loadDeck(id);
}

Card Model::getNextCard()
{
	Card card;
	return card;
}

