
#ifndef MINDCRAFT_VIEW_PB
#define MINDCRAFT_VIEW_PB

#include "controller.hpp"

struct InkViewEvent
{
	int type;
	int par1;
	int par2;
	InkViewEvent(int _type, int _par1, int _par2):type(_type),par1(_par1),par2(_par2){};
};

int mainHandler(int type, int par1, int par2);



#endif // MINDCRAFT_VIEW_PB
