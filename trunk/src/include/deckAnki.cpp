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
//#define MAX_NEW_CARDS 10
#define MAX_FAILED_CARDS 10
#define FAILED_CARDS_SPACING 20



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
			query="select * from cards where priority in (1,2,3,4) and type = 2 and (not (id in ("+ GetFetchedCardIds() +"))) ORDER BY due desc limit 1";
			FetchCardsByQuery(query);
			//std::cout<<"finally fetched by query"<<std::endl;
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
		//std::cout<<"fetched cards by row "<<std::endl;
		cardsDueBuffer.push_front(card);
		fetchedCardIds.insert(card.id);
	}
	//std::cout<<"fetched cards by query "<<std::endl;
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

		//------ load deck settings -----------
		//new cards per day
		numNewCardsPerDay=20;
		std::string query="select newCardsPerDay from decks";
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numNewCardsPerDay=FormatHelper::StrToInt(SQLiteHelper::values[0]["newCardsPerDay"]);
		std::cout<<"NEW cards per day = "<< numNewCardsPerDay <<std::endl;


		//-------loading cards statistics--------
		//cards total
		query="select count(id) from cards";
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"cards total = "<< numCardsTotal <<std::endl;

		//new cards total
		query="select count(id) from cards where type = 2 and priority in (1,2,3,4)";
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"NEW cards total = "<< numCardsNewTotal <<std::endl;

		//new cards done today
		std::cout<<"Cards done today. date= " << FormatHelper::GetTodayDateStr() <<std::endl;
		query="select * from stats where day = '"+FormatHelper::GetTodayDateStr()+"'";
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsNewDoneToday=0;
		for (int i=0;i<SQLiteHelper::values.size();i++)
		{
			numCardsNewDoneToday+=FormatHelper::StrToInt(SQLiteHelper::values[i]["newEase0"]);
			numCardsNewDoneToday+=FormatHelper::StrToInt(SQLiteHelper::values[i]["newEase1"]);
			numCardsNewDoneToday+=FormatHelper::StrToInt(SQLiteHelper::values[i]["newEase2"]);
			numCardsNewDoneToday+=FormatHelper::StrToInt(SQLiteHelper::values[i]["newEase3"]);
			numCardsNewDoneToday+=FormatHelper::StrToInt(SQLiteHelper::values[i]["newEase4"]);
		}
		

		//numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"NEW cards DONE today = "<< numCardsNewDoneToday <<std::endl;

		//cards new today
		if (isLearnMoreModeOn)
		{
			numCardsNewToday=numCardsNewTotal;
		}
		else
		{
			numCardsNewToday=std::min(numCardsNewTotal,numNewCardsPerDay);
			numCardsNewToday=numCardsNewToday-numCardsNewDoneToday;
			if (numCardsNewToday<0) numCardsNewToday=0;
		}
		std::cout<<"NEW cards today = "<< numCardsNewToday <<std::endl;

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

		//cards due today
		numCardsDueToday=numCardsReviewToday+numCardsNewToday+numCardsFailedToday;
		std::cout<<"DUE cards today = "<< numCardsDueToday <<std::endl;
	}
	catch(Exception ex)
	{
		std::cout<<"DeckAnki::LoadStats: "<<ex.GetMessage()<<std::endl;
	}
}

void DeckAnki::LoadStatsForTomorrow()
{
	//cards review tomorrow
	std::string query="select count(id) from cards where combinedDue < " + FormatHelper::GetTomorrowTimeStr() + " and priority in (1,2,3,4) and type = 1"; 
	SQLiteHelper::ExecuteQuery(dbDeck,query);
	numCardsReviewTomorrow=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
	//std::cout<<"REVIEW cards today = "<< numCardsReviewToday <<std::endl;
	
}

std::string DeckAnki::GetStatsForTomorrowStr()
{
	LoadStatsForTomorrow();
	std::string str="Congratulations!\nYou have finished for now.\nAt this time tomorrow:\n";
	str = str + "There will be "+FormatHelper::ConvertToStr(numCardsReviewTomorrow) + " reviews \n";


	return str;
}

std::string DeckAnki::GetStatsStr()
{
	LoadStatsForTomorrow();
	std::string str="Statistics:\n";
	str = str + "Total number of cards: "+FormatHelper::ConvertToStr(numCardsTotal) + " \n";
	str = str + "Unseen cards: "+FormatHelper::ConvertToStr(numCardsNewTotal) + " \n";
	str = str + "\nDone in this session: "+FormatHelper::ConvertToStr(numCardsDoneThisSession) + " cards\n";
	str = str + "\nPress Ok to continue";


	return str;
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

		try
		{
			//update decks table
			std::string query="update cards   set isDue = 1 where type = 1 and isDue = 0 and  priority in (1,2,3,4) and combinedDue <= :now";
			SQLiteHelper::ExecuteQuery(dbDeck,query);

			query="update decks set revCount=(select count(id) from cards where type = 1 and priority in (1,2,3,4) and isDue = 1)";
			SQLiteHelper::ExecuteQuery(dbDeck,query);
 
			query="update decks set failedSoonCount=(select count(*) from failedCards)";
			SQLiteHelper::ExecuteQuery(dbDeck,query);

			query="update decks set failedNowCount=(select count(*) from cards where type = 0 and isDue = 1 and combinedDue <= :now)";
			SQLiteHelper::ExecuteQuery(dbDeck,query);

			//update stats table
			query="insert into stats (type, day, reps, averageTime, reviewTime, distractedTime, distractedReps, newEase0, newEase1, newEase2, newEase3, newEase4, youngEase0, youngEase1, youngEase2, youngEase3, youngEase4, matureEase0, matureEase1, matureEase2, matureEase3, matureEase4)";
			query+="values (1,'"+FormatHelper::GetTodayDateStr()+"', 0, 0, 0, 0, 0, "+FormatHelper::ConvertToStr(sessionStats.numNewEase0)+", "+FormatHelper::ConvertToStr(sessionStats.numNewEase1)+", "+FormatHelper::ConvertToStr(sessionStats.numNewEase2)+", "+FormatHelper::ConvertToStr(sessionStats.numNewEase3)+", "+FormatHelper::ConvertToStr(sessionStats.numNewEase4)+", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)";
			SQLiteHelper::ExecuteQuery(dbDeck,query);

		}
		catch(Exception ex)
		{
			std::cout<<"Error in DeckAnki::~DeckAnki() on saving deck: "<<ex.GetMessage()<<std::endl;
		}

		sqlite3_close(dbDeck);
		dbDeck=NULL;
	}
}

DeckAnki::DeckAnki()
{
	lastCard=NULL;
	dbDeck=NULL;
	numCardsDoneThisSession=0;
}


CardAnki DeckAnki::CardFromDBRow(StringMap row)
{
	//std::cout<<"loading card from db row....";
	std::string strFront = row["question"];
	std::string strBack = row["answer"];
	CardField fldFront(strFront);
	CardField fldBack(strBack);
	CardAnki card(fldFront,fldBack);
	card.type=FormatHelper::StrToInt(row["type"]);
	card.typeInitial=card.type;
	card.priority=FormatHelper::StrToInt(row["priority"]);
	card.interval=FormatHelper::StrToFloat(row["interval"]);
	card.lastInterval=FormatHelper::StrToFloat(row["lastInterval"]);
	card.factor=FormatHelper::StrToFloat(row["factor"]);
	card.yesCount=FormatHelper::StrToInt(row["yesCount"]);
	card.noCount=FormatHelper::StrToInt(row["noCount"]);
	card.id=row["id"];
	//std::cout<<"loaded card id="<<card.id<<std::endl;
	card.due=FormatHelper::StrToFloat(row["due"]);
	//std::cout<<"done"<<std::endl;
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
	if (answer==-1)
	{
		//card is suspended
		lastCard->priority=-3;
		switch (lastCard->type)
		{
			case 0: numCardsFailedToday--; break;
			case 1: numCardsReviewToday--; break;
			case 2: numCardsNewToday--; break;
			default: break;
		}
		
		numCardsDueToday--;
		SaveCard(*lastCard);
		return;
	}
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
	case 1:
	case 2:
	case 3:
		//decr cards counters
		switch (lastCard->type)
		{
			case 0: numCardsFailedToday--; break;
			case 1: numCardsReviewToday--; break;
			case 2: numCardsNewToday--;
				switch (answer)
				{
					case 0: sessionStats.numNewEase1++; break;
					case 1: sessionStats.numNewEase2++; break;
					case 2: sessionStats.numNewEase3++; break;
					case 3: sessionStats.numNewEase4++; break;
				}

				break;
			default: break;
		}
		lastCard->yesCount++;
		lastCard->reps++;
		lastCard->isDue=false;
		lastCard->type=1;

		//related card should be affected
		//cardsAnsweredBuffer.push_back(*lastCard);
		//saving card
		numCardsDueToday--;
		SaveCard(*lastCard);
		numCardsDoneThisSession++;
		break;
	default:
		throw new Exception("invalid answer in AnswerCard");
		//flag error here
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
			return 3.0;
			break;
		case 3:
			return 5.0;
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
//	std::cout<<"start getting status"<<std::endl;
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
///	std::cout<<"done getting status"<<std::endl;
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
	query+=  ", reps = " + FormatHelper::ConvertToStr(card.reps);
	query+=  ", yesCount = " + FormatHelper::ConvertToStr(card.yesCount);
	query+=  ", noCount = " + FormatHelper::ConvertToStr(card.noCount);
	query+=  ", priority = " + FormatHelper::ConvertToStr(card.priority);
	query+=  ", successive = 1"; //todo
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


//learn more

void DeckAnki::LearnMore()
{
	isLearnMoreModeOn=true;
	LoadStats();
	return;
}



