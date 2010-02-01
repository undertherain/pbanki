
#include "deck.hpp"
#include <string>
#include <iostream>
#include <stdlib.h>

#if  defined(ARCH_PB) || defined(ARCH_PBEMU) || defined(ARCH_LINUX)
	#include <dirent.h>
#else
	#include <windows.h>
#endif

#ifdef ARCH_WIN
	//DOES NOT WORK ON WIN32
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

DeckInfoList IDeck::getDeckList(std::string directory)
{
	DeckInfoList decks;
	Logger logger;

	logger.WriteLog("current dir = " + directory);

	if (directory.length()==0)
	{
		directory="./decks/";
	}

	//	decks.push_back(DeckInfo("test1")); 
	//	decks.push_back(DeckInfo("test2"));
	//	decks.push_back(DeckInfo("test3"));

#if defined(ARCH_PB) || defined(ARCH_PBEMU) || defined(ARCH_LINUX)
	//linux-specific directory listing
	struct dirent **filelist;
	std::string temps;
	int n;
	n=scandir(directory.c_str(),&filelist,0,alphasort);
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

	//std::wstring strPattern = L"./decks/*.anki"; //будь я проклят! 
	std::string searchPattern=directory+"*.anki";
	std::wstring wSearchPattern(searchPattern.length(), ' ');
	std::copy(searchPattern.begin(), searchPattern.end(), wSearchPattern.begin());
	
	
	logger.WriteLog("search path = " + searchPattern +"\n");
	HANDLE hFile;							 // Handle to file
	WIN32_FIND_DATA FileInformation;         // File information
	hFile = ::FindFirstFile(wSearchPattern.c_str(), &FileInformation);
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
					///std::string strFilePath( strFilePathW.begin(), strFilePathW.end() ); //ох ад!  а кстати с кириллией не работает!
					std::string strFilePath;
					FormatHelper::wstrToUtf8(strFilePath,strFilePathW);  //а так работает!

					//std::cout<<strFilePath<<std::endl;
					logger.WriteLog(" f: "+strFilePath+"\n");
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
int IDeck::GetNumCardsDueToday() const
{
	return numCardsDueToday;
}

//-------------------- CardField --------------------------
std::string CardField::ToString() const
{
	std::string strResult;    //very not effective
	bool isSpan=false;
	int odd=1;
	//std::cout<<"inner text=" <<innerText<<std::endl;
	for (unsigned int i=0;i<innerText.length();i++)
	{
		if (innerText[i]=='<') isSpan = true;

		if (!isSpan)
		{
			strResult=strResult+innerText[i];
		}
		if (innerText[i]=='>') 
		{       odd++;
			isSpan = false;
			if (odd%2) strResult=strResult+"\n";
		}
	}
	return strResult;
}

int DeckStats::GetNumCardsTotal() const
{
	return numCardsTotal;
}
//-------------------oth -------------------
SessionStats::SessionStats()
{
	numNewEase0=0;
	numNewEase1=0;
	numNewEase2=0;
	numNewEase3=0;
	numNewEase4=0;

}
