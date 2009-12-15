// deckAnki
// Copyright (c) 2009 Alexander A. Drozd 

#include "deckAnki.hpp"
//#include "libs/sqlite3.h"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

#include <algorithm>

#define DECK_BUFFER_SIZE 4
#define MAX_NEW_CARDS 10
#define MAX_FAILED_CARDS 10
#define FAILED_CARDS_SPACING 20
#define SECONDS_PER_DAY 86400.0


//--------------------------- Fetching Card Queue (buffer) -------------------
void DeckAnki::Fetch()
{
	//if there are still failed cards to load - load failed card, return
	try
	{

		if (numCardsFailedInDeck>0)
		{
			//start fetching failed cards until we reach buffer limit
			std::cout<< "fetching failed cards from  "<< fileName<<std::endl;
			//std::string query="SELECT * FROM cards where type=0 ORDER BY combinedDue LIMIT " + FormatHelper::ConvertToStr(DECK_BUFFER_SIZE);
			std::string query="SELECT * FROM cards where type=0 and priority in (1,2,3,4) and (not (id in ("+ GetFetchedCardIds() +"))) ORDER BY combinedDue LIMIT 1";
			numCardsFailedInDeck -= FetchCardsByQuery(query);
			return ;
		}
		//if no mo failed cards - start loading review or new cards, until we reach biffer limit
		std::cout << "fetching due card" << std::endl;
		std::string query="select * from cards where combinedDue < " + FormatHelper::GetCurrentTimeStr() + " and priority in (1,2,3,4) and type = 1 and (not (id in ("+ GetFetchedCardIds() +"))) ORDER BY interval desc limit 1";
		if (FetchCardsByQuery(query)==0)
		{
			std::cout << "fetching new card" << std::endl;
			query="select * from cards where priority in (1,2,3,4) and type = 2 and (not (id in ("+ GetFetchedCardIds() +"))) ORDER BY interval desc limit 1";
			FetchCardsByQuery(query);
		}
		
		return ;

	}
	catch(Exception ex)
	{
		std::cout<<"Error in DeckAnki::Fetch: "<<ex.GetMessage()<<std::endl;
	}
}

int DeckAnki::FetchCardsByQuery(std::string query)
{
	SQLiteHelper::ExecuteQuery(dbDeck,query);
	//std::cout<< "query:  "<< query<<std::endl;
	//std::cout<<"Num cards fetched = " << SQLiteHelper::values.size() << std::endl;
	for (unsigned int i=0;i<SQLiteHelper::values.size();i++)
	{
		StringMap row = SQLiteHelper::values[i];
		CardAnki card= CardFromDBRow(row);
		cardsDueBuffer.push_front(card);
		fetchedCardIds.insert(card.id);
	}
	return SQLiteHelper::values.size();
}

//--------------------------- Next Card ------------------------------
ICard DeckAnki::GetNextCard()
{
	std::cout<<"getting next card, "<<cardsDueBuffer.size() << " cards in buffer" <<std::endl;
	if (cardsDueBuffer.size()<MAX_FAILED_CARDS)
	{
		Fetch();
	}
	else 
	{
		std::cout<<"failed cards limit reached, not fetching"<<std::endl;
	}
	if (!cardsDueBuffer.empty())
	{
		CardAnki card=cardsDueBuffer.front();
		cardsDueBuffer.pop_front();
		if (lastCard!=NULL)
		{
			delete lastCard;
		}
		lastCard = new CardAnki(card);
		return card;
	}

	ICard cardError(CardField("No more cards "),CardField("No more cards "));
	return cardError;
}

//--------------------------- Load Stats ------------------------------
void DeckAnki::LoadStats()
{
	std::cout<< "Loading deck stats " <<std::endl;
	try
	{

		//new cards total
		std::string query="select count(id) from cards where type = 2 and priority in (1,2,3,4)";
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"NEW cards total = "<< numCardsNewTotal <<std::endl;

		//cards new today
		numCardsNewToday=std::min(numCardsNewTotal,MAX_NEW_CARDS);
		std::cout<<"NEW cards today = "<< numCardsNewToday <<std::endl;

		//cards due today
		query="select count(id) from cards where combinedDue < " + FormatHelper::GetCurrentTimeStr() + " and priority in (1,2,3,4) and type in (0, 1)"; 
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsDueToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"])+numCardsNewToday;
		std::cout<<"DUE cards today = "<< numCardsDueToday <<std::endl;

		//cards failed today
///		query="select count(id) from cards where combinedDue < " + FormatHelper::GetCurrentTimeStr() + " and priority in (1,2,3,4) and type = 0"; 
		query="select count(id) from cards where priority in (1,2,3,4) and type = 0"; 
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsFailedToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"FAILED cards today = "<< numCardsFailedToday <<std::endl;
		numCardsFailedInDeck=numCardsFailedToday;

		//cards review today
		query="select count(id) from cards where combinedDue < " + FormatHelper::GetCurrentTimeStr() + " and priority in (1,2,3,4) and type = 1"; 
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsReviewToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"REVIEW cards today = "<< numCardsReviewToday <<std::endl;

	}
	catch(Exception ex)
	{
		std::cout<<"DeckAnki::LoadStats: "<<ex.GetMessage()<<std::endl;
	}
}

void DeckAnki::LoadData()
{
	std::cout<<"Loading deck data from  "<< fileName.c_str() <<"... ";
	//open database
	int retCode;
	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);
	std::cout<<" sqlite open... ";
	if( retCode )
	{
		Exception exception("Error in DeckAnki:LoadData: can't open database: " +  FormatHelper::ConvertToStr(sqlite3_errmsg(dbDeck)));
		sqlite3_close(dbDeck);

		throw (exception);
	}

	//update databse entriies
	//Fetch();
	std::cout<<" ok" <<std::endl;

}



DeckAnki::~DeckAnki()
{
	std::cout<<"DeckAnki destructor:\n\t" << cardsAnsweredBuffer.size()<< " answered cards not saved "<<std::endl;
	if (dbDeck!=NULL)
	{
		std::cout<<"Closing sqlite connection"<<std::endl;
		sqlite3_close(dbDeck);
	}

}

CardAnki DeckAnki::CardFromDBRow(StringMap row)
{
	std::string strFront = row["question"];
	std::string strBack = row["answer"];
	CardField fldFront(strFront);
	CardField fldBack(strBack);
	CardAnki card(fldFront,fldBack);
	card.type=FormatHelper::StrToInt(row["type"]);
	card.interval=FormatHelper::StrToFloat(row["interval"]);
	card.lastInterval=FormatHelper::StrToFloat(row["lastInterval"]);
	card.factor=FormatHelper::StrToFloat(row["factor"]);
	card.id=row["id"];
	//std::cout<<"loaded card id="<<card.id<<std::endl;
	card.due=FormatHelper::StrToFloat(row["due"]);

	return card;
}

std::string DeckAnki::GetFetchedCardIds()
{
	std::string result="0";

	for (CardIds::iterator i=fetchedCardIds.begin();i!=fetchedCardIds.end();i++)
	{
		result=result + ", "+ *i ;
	}

	return result;
}

//-------------------- answering card --------------------

void DeckAnki::AnswerCard(Answer answer)
{
	std::cout<<"Registering answer" <<std::endl;
	//update card stats
	//calc new interval
	lastCard->lastInterval=lastCard->interval;
	lastCard->interval=CalcNextInterval(*lastCard,answer);
	lastCard->due = (lastCard->interval * SECONDS_PER_DAY)+  TimeHelper::GetSeconds();
	lastCard->combinedDue = lastCard->due;

	//std::cout<<"\t next interval = " << interval<<std::endl;
	switch (answer)
	{
	case 0:
		switch (lastCard->type)
		{
			case 0: break;
			case 1: numCardsReviewToday--; numCardsFailedToday++; break;
			case 2: numCardsNewToday--;  numCardsFailedToday++;  break;
			default: break;
		}
		lastCard->type=0;
		lastCard->noCount++;
		cardsDueBuffer.push_back(*lastCard);
		break;
	default:
		//decr cards counters
		switch (lastCard->type)
		{
			case 0: numCardsFailedToday--; break;
			case 1: numCardsReviewToday--; break;
			case 2: numCardsNewToday--; break;
			default: break;
		}
		lastCard->yesCount++;
		lastCard->isDue=false;
		lastCard->type=1;

		//related card should be affected
		//cardsAnsweredBuffer.push_back(*lastCard);
		//saving card
		numCardsDueToday--;
		SaveCard(*lastCard);
		break;
	}
}

float DeckAnki::CalcNextInterval(const CardAnki & card,int ease)
{
	if (card.type!=1) //failed and new card starts with reseted intervals
	{
		switch (ease)
		{
		case 0: 
			return 0;
			break;
		case 1:
			return 1.0;
			break;
		case 2:
			return 4.0;
			break;
		case 3:
			return 6.0;
			break;
		default:
			break;
		}
		return 0;	
	}
	double factor=card.factor;
	double newFactor=factor+(0.1-(3-ease)*(0.08+(3-ease)*0.02));
	newFactor=std::max(1.3,newFactor);
	float interval;
	interval = (card.interval==0?1:card.interval);
	std::cout<<"old interval=" << interval << std::endl;
	float newInterval;
	newInterval=interval*newFactor;


	std::cout<<"\tfactor="<<factor<<"  new factor="<<newFactor<<"    new interval=" <<newInterval<<std::endl;
	return newInterval;
}


std::string DeckAnki::GetStatus()
{
	std::string strStatus;
	switch (lastCard->type)
	{
	case 0: strStatus="failed ";
		break;
	case 1: strStatus="review ";
		break;
	case 2: strStatus="new    ";
		break;
	default:
		strStatus="oops.. ";
		break;
	}
	strStatus = strStatus + "f:" + FormatHelper::ConvertToStr(GetNumCardsFailedToday())+" r:"+ FormatHelper::ConvertToStr(GetNumCardsReviewToday())+" n:"+ FormatHelper::ConvertToStr(GetNumCardsNewToday());
	return strStatus;
}

//---------------------- saving card ---------------------
void DeckAnki::SaveCard(const CardAnki & card)
{
	std::cout<<"saving card id= "<<card.id<<std::endl;
	std::string query="update cards set ";
	query+=  "due = " + FormatHelper::ConvertToStr(static_cast<int>(card.due));
	query+=  ", combinedDue = " + FormatHelper::ConvertToStr(card.combinedDue);
	query+=  ", modified = " + FormatHelper::GetCurrentTimeStr();
	query+=  ", interval = " + FormatHelper::ConvertToStr(card.interval);
	query+=  ", lastInterval = " + FormatHelper::ConvertToStr(card.lastInterval);
	query+=  ", type = " + FormatHelper::ConvertToStr(card.type);
	query+=  ", isDue = 0";
	query+=  " where id = "+ card.id;

/*
tags=:tags,

lastDue=:lastDue,
factor=:factor,
lastFactor=:lastFactor,
firstAnswered=:firstAnswered,
reps=:reps,
successive=:successive,
averageTime=:averageTime,
reviewTime=:reviewTime,
youngEase0=:youngEase0,
youngEase1=:youngEase1,
youngEase2=:youngEase2,
youngEase3=:youngEase3,
youngEase4=:youngEase4,
matureEase0=:matureEase0,
matureEase1=:matureEase1,
matureEase2=:matureEase2,
matureEase3=:matureEase3,
matureEase4=:matureEase4,
yesCount=:yesCount,
noCount=:noCount,
spaceUntil = :spaceUntil,
relativeDelay = 0,
priority = :priority
where id=:id""", self.__dict__)
*/

	std::cout<<query<<std::endl;
	try
	{
		SQLiteHelper::ExecuteQuery(dbDeck,query);
	}
	catch(Exception ex)
	{
		std::cout<<"Error in DeckAnki::SaveCard: "<<ex.GetMessage()<<std::endl;
	}
}

/*
"Return the next interval for CARD given EASE."

def _nextInterval(self, card, ease):
delay = self._adjustedDelay(card, ease)
interval = card.interval
factor = card.factor
# if shown early and not failed
if delay < 0 and card.successive:
interval = max(card.lastInterval, card.interval + delay)
if interval < self.midIntervalMin:
interval = 0
delay = 0
# if interval is less than mid interval, use presets
if ease == 1:
interval *= self.delay2
if interval < self.hardIntervalMin:
interval = 0
elif interval == 0:
if ease == 2:
interval = random.uniform(self.hardIntervalMin,
self.hardIntervalMax)
elif ease == 3:
interval = random.uniform(self.midIntervalMin,
self.midIntervalMax)
elif ease == 4:
interval = random.uniform(self.easyIntervalMin,
self.easyIntervalMax)
else:
# if not cramming, boost initial 2
if (interval < self.hardIntervalMax and
interval > 0.166):
mid = (self.midIntervalMin + self.midIntervalMax) / 2.0
interval *= (mid / interval / factor)
# multiply last interval by factor
if ease == 2:
interval = (interval + delay/4) * 1.2
elif ease == 3:
interval = (interval + delay/2) * factor
elif ease == 4:
interval = (interval + delay) * factor * self.factorFour
fuzz = random.uniform(0.95, 1.05)
interval *= fuzz
if self.maxScheduleTime:
interval = min(interval, self.maxScheduleTime)
return interval

*/
//save card
//remove id from fetchedCardIds
//
