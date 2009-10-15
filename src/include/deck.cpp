#include "deck.hpp"
#include <string>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

#include "libs/sqlite3.h"

// ----------------- DeckInfo ------------------------
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

std::string DeckInfo::GetName()
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
		std::cout<< "Can't open database: " <<  sqlite3_errmsg(dbDeck)<<std::endl;
		sqlite3_close(dbDeck);
		//throw exception here?
	}
	std::cout<< "reading sqlite  "<< fullPath <<" " <<retCode<<std::endl;
	std::string query="select COUNT(*) from cards";
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

// ----------------- Deck ----------------------------

DeckInfoList Deck::getDeckList()
{
	DeckInfoList decks;

//	decks.push_back(DeckInfo("test1")); 
//	decks.push_back(DeckInfo("test2"));
//	decks.push_back(DeckInfo("test3"));

#ifdef WIN32
	std::wstring strPattern = L".\\decks\\*.anki"; //будь я проклят! 
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
		std::cout<<"coult not find decks"<<std::endl;
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
				decks.push_back(DeckInfo(temps));
			std::cout<<temps<<std::endl;
		}
		free(filelist);
		std::cout<<n<<" decks loaded"<<std::endl;
	}
#endif
	std::string sss;


	return decks;
}

Deck * Deck::loadDeck(DeckId id)
{
	Deck * newdeck = new Deck();
	return newdeck;
}


//------------------ QA - card field -----------------
std::string QA::ToString() const
{
	return std::string("QA stub value 日本語");
}

