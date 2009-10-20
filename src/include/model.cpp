#include "model.hpp"

DeckInfoList Model::getDeckList()
{
	return IDeck::getDeckList();
}

void Model::loadDeck(DeckId id)
{
	currentDeck=IDeck::loadDeck(id);
}

Card Model::getNextCard()
{
	return currentDeck->getNextCard();
}

