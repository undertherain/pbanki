//  Console interface

#include <iostream>
#include <algorithm>

#include <conio.h>

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
			case Status::stSelectDeck:
				displayDeckList(model.getDeckList());
				status=Status::stLoadDeck;
				break;
			case Status::stLoadDeck:
				{
					DeckId id;
					id=selectDeck();
					if (id.length()==0)
					{
						status=Status::stExit;	
					}
					else
					{
						model.loadDeck(id);
						status=Status::stShowFront;
					}
				}
				break;
			case Status::stShowFront:
				std::cout<<"front: sample front\n";
				std::cout<<"\t enter to view answer\n";
				
				getch();

				status=Status::stShowBack;	
				break;
			case Status::stShowBack:
				{
					Card card=model.getNextCard();
					std::cout<<"back: sample back\n ";
					std::cout<<"\tevaluate you answer [1-4, 0 to exit]\n";
					int answer=0;
					std::cin>>answer;

					if (answer==0) 	status=Status::stExit;	
					else
					{
						status=Status::stShowFront;
					}

				}
				break;
			case Status::stExit:
				_exit(0);
				break;
			default:
				//make error unexepected status
				_exit(0);
				break;

			}
		}
		catch(...)
		{
			std::cout<<"exc\n";
		}

	}


}
