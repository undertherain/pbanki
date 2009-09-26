// Anki.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Session.h"
#include <iostream>

int main(int argc, _TCHAR* argv[])
{
	std::auto_ptr<Logic::ISession> ptrSession(new Logic::Implementation::CBaseSession());
	const std::vector<std::string>& vecStrDeckNames = ptrSession->ShowDeckList();

	//SHIIIT, NIGGA! This code must be inside of interface classes. What have I done? :)
	std::vector<std::string>::const_iterator it = vecStrDeckNames.begin();
	for (; it != vecStrDeckNames.end(); ++it)
	{
		std::cout << "Deck name = " << *it << std::endl;
	}
	return 0;
}

