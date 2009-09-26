#ifndef __INTERFACE_H_INCLUDED__
#define __INTERFACE_H_INCLUDED__

namespace Interface
{

/**
@brief A class or set of classes which are platform dependent.
*/
class CInterface
{
 public:
	void StartButtonClick();
	void KnowButtonClick();
	void DontKnowButtonClick();
	void ShowNextCardButtonClick();
};

} //namespace Interface

#endif //__INTERFACE_H_INCLUDED__