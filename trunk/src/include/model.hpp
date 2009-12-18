#ifndef MINDCRAFT_MODEL
#define MINDCRAFT_MODEL

#include <string>
#include <list>

#include "deck.hpp"
#define TEMPORARY 

class Model  //facade 
{
private:
	IDeck * currentDeck;
public:
	DeckInfoList getDeckList();				//returns list of available deck
	void LoadDeck(DeckId deckName);	//load selected deck
	void LoadStats();
	void CloseDeck();
	void LearnMore();
	ICard getNextCard();						//next card from queue
	void AnswerCard(Answer answer); //also does suspend???
	void SuspendCard();
	
	TEMPORARY std::string GetStatus();
	std::string GetStatsForTomorrowStr();
	std::string currentDirectory;
	Model():currentDeck(NULL){};
	~Model();
	int GetNumCardsDueToday();
	//deck props?
	//learn more?

};



#endif // MINDCRAFT_MODEL
