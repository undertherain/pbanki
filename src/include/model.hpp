#ifndef MINDCRAFT_MODEL
#define MINDCRAFT_MODEL

#include <string>
#include <list>

#include "deck.hpp"


class Model  //facade 
{
private:
	IDeck * currentDeck;
public:
	DeckInfoList getDeckList();				//returns list of available deck
	void LoadDeck(DeckId deckName);	//load selected deck
	void LoadStats();
	Card getNextCard();						//next card from queue

	//deck props?
	//learn more?

};



#endif // MINDCRAFT_MODEL
