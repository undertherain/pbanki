#ifndef __LOADER_H_INCLUDED__
#define __LOADER_H_INCLUDED__

#include <vector>
#include <string>
#include <memory>

namespace Model
{
	class IDeck;
	class IUser;
}

namespace Utils
{
	class IOError;

	class CDeckLoader;
}

typedef std::vector<Utils::CDeckLoader> DeckList;
typedef std::auto_ptr<Model::IUser> UserInfo;

namespace Utils
{

/**
@descr Deck loader class contains all needed logic to display short
	   info about deck inside of it and to make full load of selected
	   by user deck.
*/
class CDeckLoader
{
private:
	Model::IDeck* m_ptrDeck;
	int* m_ptrDeckPtrCnt;

public:
	CDeckLoader(Model::IDeck* ptrDeck);
	CDeckLoader(const CDeckLoader& other);
	~CDeckLoader();

	std::string GetName() const;
	void Load();
};

/**
@brief This class should encapsulate loading and saving of data.
@descr Every action which is connected with file work should be
	   provided through this class.
*/
class CLoader
{
public:
	/**
	@descr Create loading information using description file.
	*/
	CLoader() throw(IOError);	//TODO: This constructor is hard code and should be removed!
	CLoader(const std::string& strFilePath) throw(IOError);

	DeckList GetDeckList()const throw(IOError); //TODO: This function is hardcoded. Please fix it.
	UserInfo GetUserInfo()const throw(IOError); //TODO: This function is hardcoded. Please fix it.
};

} //namespace Utils

#endif //__LOADER_H_INCLUDED__