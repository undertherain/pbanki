#include "deck.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <map>


#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

#include "libs/sqlite3.h"
#include "logger.hpp"

typedef std::map<std::string,std::string> StringMap;
// ------------------------- SQLite routines ------------------------
class SQLiteResults
{
public:
	static std::vector<StringMap> values;

};
std::vector<StringMap> SQLiteResults::values;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)    //who does free for these poiners???????
{
	int i;
	//printf("NotUsed = %d\n", NotUsed);
	StringMap newMap;
	for(i=0; i<argc; i++){
		newMap.insert(std::pair<std::string,std::string>( azColName[i], argv[i] ? argv[i] : "NULL") );
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	SQLiteResults::values.push_back(newMap);
	printf("\n");
	return 0;
}

// ------------------------- DeckInfo ------------------------

std::string DeckInfo::GetName() const
{
	return name;
}

DeckInfo::DeckInfo(std::string fileName)
{
	name=fileName;

	//open file with sqlite
	sqlite3 *dbDeck;
	char *zErrMsg = NULL;
	int retCode;
	std::string fullPath=".\\decks\\"+fileName;
	retCode = sqlite3_open_v2(fullPath.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);


	if( retCode )
	{
		std::cout<< "Deckinfo:: Can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);
		//throw exception here?

	}
	else
	{
		std::cout<< "reading sqlite  "<< fullPath <<" " <<retCode<<std::endl;
		std::string query="select COUNT(*) from cards where isDue=1";
		retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);
		if( retCode!=SQLITE_OK )
		{
			std::cout << "SQL error: " <<  zErrMsg << std::endl;
			sqlite3_free(zErrMsg);
		}

		//read total number of cards

		//close sqlite
		sqlite3_close(dbDeck);
	}
}

// -------------------------- Deck ----------------------------

DeckInfoList Deck::getDeckList()
{
	DeckInfoList decks;
	Logger logger;

	//	decks.push_back(DeckInfo("test1")); 
	//	decks.push_back(DeckInfo("test2"));
	//	decks.push_back(DeckInfo("test3"));

#ifdef WIN32
	std::wstring strPattern = L"./decks/*.anki"; //будь я проклят! 
	HANDLE hFile;							 // Handle to file
	WIN32_FIND_DATA FileInformation;         // File information
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{
				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{

				}
				else
				{
					std::wstring     strFilePathW  = (FileInformation.cFileName); 
					std::string strFilePath( strFilePathW.begin(), strFilePathW.end() ); //ох ад!

					//std::cout<<strFilePath<<std::endl;
					decks.push_back(DeckInfo(strFilePath));
				}
			}
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);
	}
	else 
	{
		logger.WriteLog("coult not find decks");
	}

#else
	//linux-specific directory listing
	struct dirent **filelist;
	std::string temps;
	int n;
	n=scandir("./decks/",&filelist,0,alphasort);
	if (n<0)
	{
		std::cerr<<"no decks found"<<std::endl;
		//should we exit here?
	} else
	{
		for (int ii=0;ii<n;ii++)
		{
			temps=filelist[ii]->d_name;
			free (filelist[ii]);
			if  (temps.find(".anki")!=std::string::npos)
			{
				std::cout<<"loading   "<<temps<<std::endl;
				decks.push_back(DeckInfo(temps));
			}
			
		}
		free(filelist);
		std::cout<<n<<" decks loaded"<<std::endl;
	}
#endif

	return decks;
}

Deck * Deck::loadDeck(DeckId id)
{
	std::string newDeckFileName="./decks/"+id;

	//open file in sqlite
	//if error - throw exception
	//
	Deck * newDeck = new Deck();
	newDeck->fileName=newDeckFileName;

	return newDeck;
}

Card Deck::getNextCard()
{

	sqlite3 *dbDeck;
	char *zErrMsg = NULL;
	int retCode;

	//open sqlite
	//select random card

	retCode = sqlite3_open_v2(fileName.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	if( retCode )
	{
		std::cout<< "getNextCard. Can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);
		Card cardError(CardField("Can't open database: "),CardField(sqlite3_errmsg(dbDeck)));
		return cardError;
		//throw exception here?
	}
	std::cout<< "reading sqlite  "<< fileName <<" " <<retCode<<std::endl;
	std::string query="SELECT * FROM cards ORDER BY RANDOM() LIMIT 1";
	SQLiteResults::values.clear();
	retCode = sqlite3_exec(dbDeck, query.c_str(), callback, 0, &zErrMsg);

	std::cout<<"size = "<< SQLiteResults::values.size()<<std::endl;
	if( retCode!=SQLITE_OK )
	{
		std::cout << "SQL error: " <<  zErrMsg << std::endl;
		sqlite3_free(zErrMsg);
	}
	//close sqlite
	sqlite3_close(dbDeck);

	StringMap row = SQLiteResults::values[0];
	std::string strFront = row["question"];
	std::string strBack = row["answer"];
	CardField fldFront(strFront);
	CardField fldBack(strBack);
	//Card card(CardField("stub front 日本語"),CardField("stub back 日本語"));
	Card card(fldFront,fldBack);
	return card;
}

//-------------------- CardField --------------------------
std::string CardField::ToString() const
{
	std::string strResult;    //very not effective
	bool isSpan=false;
	for (int i=0;i<innerText.length();i++)
	{
		if (innerText[i]=='<') isSpan = true;

		if (!isSpan)
		{
			strResult=strResult+innerText[i];
		}
		if (innerText[i]=='>') 
		{ 
			isSpan = false;
			strResult=strResult+"\n";
		}
	}
	return strResult;
}

