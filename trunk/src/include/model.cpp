#include "model.hpp"
#include "utils.hpp"
#include "exceptions.hpp"


DeckInfoList Model::getDeckList()
{
	if (currentDirectory.length()==0)
	{
		return IDeck::getDeckList();
	}
	else
	{
		return IDeck::getDeckList(currentDirectory+"/decks/");
	}
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

void Model::SuspendCard()
{
	currentDeck->AnswerCard(-1);
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
	currentDeck=NULL;
}

int Model::GetNumCardsDueToday()
{
	return currentDeck->GetNumCardsDueToday();
}

void Model::CloseDeck()
{
	if (currentDeck!=NULL)
		delete currentDeck;
		currentDeck=NULL;
}


void Model::LearnMore()
{
	currentDeck->LearnMore();
}

std::string Model::GetStatsForTomorrowStr()
{
	return currentDeck->GetStatsForTomorrowStr();
}
