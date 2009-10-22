#include "deck.hpp"
#include "dbSQLite.hpp"

#include <list>

class CardAnki;

typedef std::list<CardAnki> CardList;

class CardAnki: public ICard
{
private:
	//"cardModelId","created","modified","tags","ordinal","question","answer","priority","firstAnswered","successive","averageTime","reviewTime","youngEase0","youngEase1","youngEase2","youngEase3","youngEase4","matureEase0","matureEase1","matureEase2","matureEase3","matureEase4""spaceUntil","relativeDelay","",

public:
	CardAnki(CardField _front,CardField _back):ICard(_front,_back){};
	CardAnki():ICard(CardField("ankiempty"),CardField("ankiempty")){};
	int id;
	int factId;
	float modified;
	float interval;
	float lastInterval;
	float due;
	float lastDue;
	float factor;
	float lastFactor;
	int combinedDue;
	int reps;
	int yesCount;
	int noCount;
	bool isDue;
};

class DeckAnki: public IDeck
{
private:
		CardList cardsDueBuffer;
		CardList cardsAnsweredBuffer;
		CardAnki * lastCard;
		CardAnki CardFromDBRow(StringMap row);

public:
	ICard GetNextCard();						//next card from queue
	void LoadStats();						//
	void LoadData();						//
	void AnswerCard(Answer answer);
	void Fetch();
	DeckAnki():lastCard(NULL){}
	~DeckAnki();
};
