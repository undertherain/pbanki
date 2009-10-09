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
		std::cout<<i->GetName()+" \n";	
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


void ViewConsole::ShowFront(const Card & card)
{
	std::cout<<"front: "<<card.front.ToString()<<std::endl;
	return ;
}

void ViewConsole::ShowBack(const Card & card)
{
	std::cout<<"back: "<<card.front.ToString()<<std::endl;
	return ;
}



void ViewConsole::run()
{
	std::cout<<"entering main loop\n";
	Card card;
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
				//view. show front
				
				card=model.getNextCard();
				ShowFront(card);
				
				//view. wait for answer
				
				std::cout<<"\tpress enter to view answer\n";
				
				_getch();

				status=Status::stShowBack;	
				break;
			case Status::stShowBack:
				{
					
					ShowBack(card);
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
