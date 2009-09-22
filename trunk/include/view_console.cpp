#include <iostream>
#include <algorithm>

#include "view_console.hpp"
#include "model.hpp"

void ViewConsole::refresh()
{
	std::cout<<"refreshing console view";
}

void ViewConsole::displayDeckList(DeckInfoList decks)
{
	std::cout<<"displaying deck list\n";
	for (DeckInfoList::iterator i=decks.begin();i!=decks.end();i++)
	{
		std::cout<<i->name+" \n";	
	}
}

DeckId ViewConsole::selectDeck()
{
	std::cout<<"Select deck:";
	DeckId id;
	std::cin>>id;
	std::cout<<std::endl;
	return id;
}




void ViewConsole::run()
{
	std::cout<<"entering main loop\n";

	for(;;)  //main loop
	{
		try
		{
			switch (status)
			{
			case 1:// selecting deck
				displayDeckList(model.getDeckList());
				status=2;
				break;
			case 2://selecting deck list
				{
					DeckId id;
					id=selectDeck();
					if (id.length()==0)
					{
						status=0;	
					}
					else
					{
						model.loadDeck(id);
						status=4;
					}
				}
				break;
			case 4://show front
				std::cout<<"front:\n";
	
				status=5;	
				break;
			case 5://show front
				{
					Card card=model.getNextCard();
					std::cout<<"back\n ";
					int answer=0;
					std::cin>>answer;
				
					if (answer==0) 	status=0;	
					else
					{
						status=4;
					}

				}
				break;
			case 0: _exit(0);

			}
		}
		catch(...)
		{
			std::cout<<"exc\n";
		}

	}

}
