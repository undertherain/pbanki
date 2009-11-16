#include "deck.hpp"
#include <string>
#include <iostream>


#if  defined(ARCH_PB) || defined(ARCH_PBEMU)
#include <dirent.h>
#endif

#if defined(ARCH_WM) || defined(WIN32)
#include <windows.h>
#endif

#include "logger.hpp"
#include "deckAnki.hpp"
#include "deckMnemosyne.hpp"




// ------------------------- DeckInfo ------------------------

std::string DeckInfo::GetName() const
{
	return name;
}

DeckInfo::DeckInfo(std::string fileName)
{
	name=fileName;

	//open file with sqlite
//	sqlite3 *dbDeck;
//	char *zErrMsg = NULL;
//	int retCode;
	std::string fullPath=".\\decks\\"+fileName;
//	retCode = sqlite3_open_v2(fullPath.c_str(), &dbDeck,SQLITE_OPEN_READWRITE,NULL);

	//anki-cpecific stuuf should be moved to deckAnki

/*	if( retCode )
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
	*/
}

// -------------------------- Deck ----------------------------

DeckInfoList IDeck::getDeckList()
{
	DeckInfoList decks;
	Logger logger;

	//	decks.push_back(DeckInfo("test1")); 
	//	decks.push_back(DeckInfo("test2"));
	//	decks.push_back(DeckInfo("test3"));

#if defined(ARCH_PB) || defined(ARCH_PBEMU)
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
	//	std::cout<<n<<" decks loaded"<<std::endl;
	}
#else

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
		logger.WriteLog("could not find decks");
	}
#endif

	return decks;
}

IDeck * IDeck::LoadDeck(DeckId id)
{

	std::string newDeckFileName="./decks/"+id;

	//open file in sqlite
	//if error - throw exception
	//
	IDeck * newDeck = new DeckAnki();
	newDeck->fileName=newDeckFileName;

	return newDeck;
}

int IDeck::GetNumCardsFailedToday() const
{
	return numCardsFailedToday;
}
int IDeck::GetNumCardsReviewToday() const
{
	return numCardsReviewToday;
}

int IDeck::GetNumCardsNewToday() const
{
	return numCardsNewToday;
}


//-------------------- CardField --------------------------
std::string CardField::ToString() const
{
	std::string strResult;    //very not effective
	bool isSpan=false;
	for (unsigned int i=0;i<innerText.length();i++)
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
