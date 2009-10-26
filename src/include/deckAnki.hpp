#include "deck.hpp"
#include "dbSQLite.hpp"

#include <list>
#include <set>
#include <string>

class CardAnki;

typedef std::list<CardAnki> CardList;
typedef std::set<std::string> CardIds;

class CardAnki: public ICard
{
private:
	//"cardModelId","created","modified","tags","ordinal","question","answer","priority","firstAnswered","successive","averageTime","reviewTime","youngEase0","youngEase1","youngEase2","youngEase3","youngEase4","matureEase0","matureEase1","matureEase2","matureEase3","matureEase4""spaceUntil","relativeDelay","",
	int failedCardsCounter;
public:
	CardAnki(CardField _front,CardField _back):ICard(_front,_back){};
	CardAnki():ICard(CardField("ankiempty"),CardField("ankiempty")){};
	std::string id;
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
	sqlite3 *dbDeck;
	CardList cardsDueBuffer;
	CardList cardsAnsweredBuffer;
	CardIds fetchedCardIds;
	std::string GetFetchedCardIds();
	CardAnki * lastCard;
	CardAnki CardFromDBRow(StringMap row);
	int numCardsFailedInDeck;
	float CalcNextInterval(const CardAnki & card,int ease);
	void Fetch();
	void SaveCard(const CardAnki & card);
	int FetchCardsByQuery(std::string query);

public:
	ICard GetNextCard();						//next card from queue
	void LoadStats();						//
	void LoadData();						//
	void AnswerCard(Answer answer);
	std::string GetStatus();
	DeckAnki()
		{
			lastCard=NULL;
			dbDeck=NULL;
		}
	~DeckAnki();
};
