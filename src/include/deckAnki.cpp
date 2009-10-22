// deckAnki
// Copyright (c) 2009 Alexander A. Drozd 

#include "deckAnki.hpp"
#include "libs/sqlite3.h"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#define DECK_BUFFER_SIZE 3
#define MAX_NEW_CARDS 10



//--------------------------- Fetching Card Queue (buffer) -------------------
void DeckAnki::Fetch()
{
	//start fetching failed cards until we reach buffer limit
	sqlite3 *dbDeck;
	int retCode;

	//open sqlite
	//select random card

	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	if( retCode )
	{
		std::cout<< "getNextCard. Can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);
		return ;
		//throw exception here?
	}
	std::cout<< "reading sqlite  "<< fileName <<" " <<retCode<<std::endl;
	std::string query="SELECT * FROM cards where type=0 ORDER BY combinedDue LIMIT " + FormatHelper::ConvertToStr(DECK_BUFFER_SIZE);
	try
	{
		SQLiteHelper::ExecuteQuery(dbDeck,query);
	}
		catch(Exception ex)
	{
		std::cout<<"DeckAnki::Fetch: "<<ex.GetMessage()<<std::endl;
	}

	//close sqlite
	sqlite3_close(dbDeck);

	std::cout<<"size = " << SQLiteHelper::values.size() << std::endl;
	for (unsigned int i=0;i<SQLiteHelper::values.size();i++)
	{
		StringMap row = SQLiteHelper::values[i];
		CardAnki card= CardFromDBRow(row);
		cardsDueBuffer.push_back(card);
	}
	//if no mo failed cards - start loading review cards, until we reach biffer limit
	//if no more review cards - load new cards
}

//--------------------------- Next Card ------------------------------
ICard DeckAnki::GetNextCard()
{

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
	sqlite3 *dbDeck;
	int retCode;

	//open sqlite
	//select random card
	std::cout<< "Loading deck stats " <<std::endl;

	try
	{
		retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

		if( retCode )
		{
			std::cout<< "DeckAnki:LoadStats. can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
			sqlite3_close(dbDeck);

			return ;
			//throw exception here?
		}
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
	//close sqlite
	sqlite3_close(dbDeck);
}

void DeckAnki::LoadData()
{
	std::cout<<"Loading deck data " <<std::endl;
	//update databse entriies
	Fetch();
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
	std::cout<<"DeckAnki destructor:\n\t" << cardsAnsweredBuffer.size()<< " answered cards not saved "<<std::endl;
}

CardAnki DeckAnki::CardFromDBRow(StringMap row)
{
		std::string strFront = row["question"];
		std::string strBack = row["answer"];
		CardField fldFront(strFront);
		CardField fldBack(strBack);
		CardAnki card(fldFront,fldBack);
		card.id=FormatHelper::StrToInt(row["id"]);
		card.due=FormatHelper::StrToFloat(row["due"]);

	return card;
}
