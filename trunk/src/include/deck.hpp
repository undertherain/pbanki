#ifndef MINDCRAFT_DECK
#define MINDCRAFT_DECK

#include <string>
#include <list>
#include <iostream>
class DeckInfo;
class ICard;

typedef std::list<DeckInfo> DeckInfoList ;
typedef std::string DeckId ;
typedef int Answer ;

class DeckStats ///not in use yet
{
	friend class IDeck;
	protected:
	int numCardsTotal;
	int GetNumCardsTotal() const;
};

class SessionStats
{
public:
	int reps;
	float reviewTime;
	float averageTime;
	int numNewEase0;
	int numNewEase1;
	int numNewEase2;
	int numNewEase3;
	int numNewEase4;
	SessionStats();
};

//contains cards
class IDeck  
{
protected:
	SessionStats sessionStats;
	std::string fileName;
	int numNewCardsPerDay;
	int numCardsTotal;
	int numCardsSuspended;
	int numCardsNewTotal;
	int numCardsNewDoneToday;
	int numCardsNewToday;
	int numCardsFailedToday;
	int numCardsDueToday;
	int numCardsReviewToday;
	int numCardsReviewTomorrow;
	int numCardsDoneThisSession;
	bool isLearnMoreModeOn;

public:
	static DeckInfoList getDeckList(std::string directory="");
	static IDeck * LoadDeck(DeckId id);
	virtual ICard GetNextCard()=0;					//next card from queue
	//virtual bool IsEmpty()=0;
	virtual void LoadData()=0;
	virtual void LoadStats()=0;
	virtual void LoadStatsForTomorrow()=0;
	virtual std::string GetStatsForTomorrowStr()=0;
	virtual std::string GetStatsStr()=0;
	virtual void LearnMore()=0;
	virtual void AnswerCard(Answer answer)=0;
	virtual std::string GetStatus()=0;
	IDeck():isLearnMoreModeOn(false){}
	virtual ~IDeck(){}
	//virtual void CloseDeck()=0;
	int GetNumCardsFailedToday() const;
	int GetNumCardsReviewToday() const;
	int GetNumCardsNewToday() const;
	int GetNumCardsDueToday() const;

};

//light-weighted class for list of available decks
class DeckInfo 
{
private:
	std::string name;
	int numCardsDueToday;
	int numCardsNewToday;
	int numCardsTotal;

public:

	DeckInfo(std::string fileName);
	
	//DeckInfo(const DeckInfo & deck):name(deck.name)
	//{}

	std::string GetName() const;
	int getNumCardsTotal() const;
};

//question or answer
class CardField	
{
private:
	std::string innerText;
public:
	std::string soundPath;
	std::string ToString() const;
	CardField(std::string _innerText, std::string _soundPath="");
};

class ICard
{
public:
	int type;
	int typeInitial;
	CardField front;
	CardField back;
	
	
	ICard():front("emty",""),back("empty",""){};
	ICard(CardField _front,CardField _back):front(_front),back(_back){};

};


#endif // MINDCRAFT_DECK

