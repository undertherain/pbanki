#include "deck.hpp"
#include <string>
#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

// ----------------- DeckInfo ------------------------

std::string DeckInfo::GetName()
{
	return name;
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
					std::string strFilePath( strFilePathW.begin(), strFilePathW.end() );
				
					std::cout<<strFilePath<<std::endl;
				}
			}
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);

	}
	else 
	{
		std::cout<<"coult not finsd decks"<<std::endl;
	}

#else
	//linux-specifi
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
			//user *tu= user::load(temps);
			//pair<userlist::iterator,bool>p=users.insert(make_pair(temps,tu));
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

