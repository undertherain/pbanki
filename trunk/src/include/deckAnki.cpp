// deckAnki
// Copyright (c) 2009 Alexander A. Drozd 

#include "deckAnki.hpp"
#include "libs/sqlite3.h"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

#include <algorithm>

#define DECK_BUFFER_SIZE 4
#define MAX_NEW_CARDS 10



//--------------------------- Fetching Card Queue (buffer) -------------------
void DeckAnki::Fetch()
{
	//start fetching failed cards until we reach buffer limit


	std::cout<< "fetching card from  "<< fileName<<std::endl;
	//std::string query="SELECT * FROM cards where type=0 ORDER BY combinedDue LIMIT " + FormatHelper::ConvertToStr(DECK_BUFFER_SIZE);
	std::string query="SELECT * FROM cards where combinedDue < " + FormatHelper::GetTimeStr() + " and type=0 and priority in (1,2,3,4) and (not (id in ("+ GetFetchedCardIds() +"))) ORDER BY combinedDue LIMIT 1";
	//query="select count(id) from cards where  and priority in (1,2,3,4) and type = 0"; 

	try
	{
		SQLiteHelper::ExecuteQuery(dbDeck,query);
	}
	catch(Exception ex)
	{
		std::cout<<"Error in DeckAnki::Fetch: "<<ex.GetMessage()<<std::endl;
	}
	std::cout<< "query:  "<< query<<std::endl;

	std::cout<<"Nmum cards fetched = " << SQLiteHelper::values.size() << std::endl;
	for (unsigned int i=0;i<SQLiteHelper::values.size();i++)
	{
		StringMap row = SQLiteHelper::values[i];
		CardAnki card= CardFromDBRow(row);
		cardsDueBuffer.push_back(card);
		fetchedCardIds.insert(card.id);
	}
	//if no mo failed cards - start loading review cards, until we reach biffer limit
	//if no more review cards - load new cards
}

//--------------------------- Next Card ------------------------------
ICard DeckAnki::GetNextCard()
{
	std::cout<<"getting next card, "<<cardsDueBuffer.size() << " cards in buffer" <<std::endl;
	if (cardsDueBuffer.size()<DECK_BUFFER_SIZE)
	{
		Fetch();
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
		query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type in (0, 1)"; 
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsNewTotal=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"DUE cards today = "<< numCardsNewTotal <<std::endl;


		//cards failed today
		query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type = 0"; 
		SQLiteHelper::ExecuteQuery(dbDeck,query);
		numCardsFailedToday=FormatHelper::StrToInt(SQLiteHelper::values[0]["count(id)"]);
		std::cout<<"FAILED cards today = "<< numCardsFailedToday <<std::endl;

		//cards revire today
		query="select count(id) from cards where combinedDue < " + FormatHelper::GetTimeStr() + " and priority in (1,2,3,4) and type = 1"; 
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
	std::cout<<"Loading deck data " <<std::endl;
	//open database
	int retCode;
	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	if( retCode )
	{
		Exception exception("Error in DeckAnki:LoadData: can't open database: " +  FormatHelper::ConvertToStr(sqlite3_errmsg(dbDeck)));
		sqlite3_close(dbDeck);

		throw (exception);
	}

	//update databse entriies
	//Fetch();
}

void DeckAnki::AnswerCard(Answer answer)
{
	std::cout<<"Registering answer" <<std::endl;
	//update card stats
	//calc new interval

	switch (answer)
	{
	case 0:
		cardsDueBuffer.push_back(*lastCard);

		break;
	default:
		numCardsFailedToday--;
		cardsAnsweredBuffer.push_back(*lastCard);
		break;
	}

}

DeckAnki::~DeckAnki()
{
	if (dbDeck!=NULL)
	{
		sqlite3_close(dbDeck);
	}
	std::cout<<"DeckAnki destructor:\n\t" << cardsAnsweredBuffer.size()<< " answered cards not saved "<<std::endl;
}

CardAnki DeckAnki::CardFromDBRow(StringMap row)
{
	std::string strFront = row["question"];
	std::string strBack = row["answer"];
	CardField fldFront(strFront);
	CardField fldBack(strBack);
	CardAnki card(fldFront,fldBack);
	card.type=FormatHelper::StrToInt(row["type"]);
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

float DeckAnki::CalcNextInterval(CardAnki card,int ease)
{
	return 0;
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
//save card
//remove id from fetchedCardIds
//
