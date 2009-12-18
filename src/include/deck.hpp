#ifndef MINDCRAFT_DECK
#define MINDCRAFT_DECK

#include <string>
#include <list>

class DeckInfo;
class ICard;

typedef std::list<DeckInfo> DeckInfoList ;
typedef std::string DeckId ;
typedef int Answer ;


//contains cards
class IDeck  
{
protected:
	std::string fileName;
	int numNewCardsPerDay;
	int numCardsNewTotal;
	int numCardsNewToday;
	int numCardsFailedToday;
	int numCardsDueToday;
	int numCardsReviewToday;
	int numCardsReviewTomorrow;
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
	virtual void LearnMore()=0;
	virtual void AnswerCard(Answer answer)=0;
	virtual std::string GetStatus()=0;
	IDeck():isLearnMoreModeOn(false){}
	virtual ~IDeck(){}
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
	std::string ToString() const;
	CardField(std::string _innerText):innerText(_innerText){};
};

class ICard
{
public:
	int type;
	CardField front;
	CardField back;
	
	ICard():front("emty"),back("empty"){};
	ICard(CardField _front,CardField _back):front(_front),back(_back){};

};


#endif // MINDCRAFT_DECK

