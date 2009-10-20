#include "model.hpp"

DeckInfoList Model::getDeckList()
{
	return IDeck::getDeckList();
}

void Model::LoadDeck(DeckId id)
{
	currentDeck=IDeck::LoadDeck(id);
	currentDeck->LoadData();
}

Card Model::getNextCard()
{
	return currentDeck->GetNextCard();
}

void Model::LoadStats()
{
	currentDeck->LoadStats();
}

