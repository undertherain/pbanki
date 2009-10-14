// description here


#include "include/view_console.hpp"
#include <iostream>

int main (int argc, char * argv[])
{
	std::cout<<"hello"<<std::endl;
	
//	IView & view = *(new ViewConsole);					//create platform-dependent view
//Presenter & presenter = *(new Presenter(view));		//create presenter and assign to view
	ViewConsole view;

	view.run();
	//run application

	return 0;
}
