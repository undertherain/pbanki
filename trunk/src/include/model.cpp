#include "model.hpp"
#include "utils.hpp"

DeckInfoList Model::getDeckList()
{
	return IDeck::getDeckList();
}

void Model::LoadDeck(DeckId id)
{
	currentDeck=IDeck::LoadDeck(id);
	currentDeck->LoadData();
}

ICard Model::getNextCard()
{
	return currentDeck->GetNextCard();
}

void Model::LoadStats()
{
	currentDeck->LoadStats();
}



void Model::AnswerCard(Answer answer)
{
	currentDeck->AnswerCard(answer);
}

std::string Model::GetStatus()
{
	return currentDeck->GetStatus();
}

Model::~Model()
{
	if (currentDeck!=NULL)
		delete currentDeck;
}
