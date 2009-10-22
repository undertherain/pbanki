#include "deck.hpp"
#include <list>

typedef std::list<ICard> CardList;

class CardAnki: public ICard
{

};

class DeckAnki: public IDeck
{
private:
		CardList cardsDueBuffer;
		CardList cardsAnsweredBuffer;
		ICard * lastCard;

public:
	ICard GetNextCard();						//next card from queue
	void LoadStats();						//
	void LoadData();						//
	void AnswerCard(Answer answer);
	void Fetch();
	DeckAnki():lastCard(NULL){}
	~DeckAnki();
};
