#ifndef __IDECK_H_INCLUDED__
#define __IDECK_H_INCLUDED__

#include <string>

namespace Model
{

/**
@brief Interface for different types of decks.
@descr One possible reason for decks of different types
	   is that decks can contain different types of cards
	   (for example ieroglyphs or not) and so their internal
	   structure may differ. But it should be encapsulated.
*/
class IDeck
{
 public:
	virtual void LoadFull(const std::string& strInit) = 0;
	virtual std::string GetName() const = 0;
};

} //namespace Model

#endif //__IDECK_H_INCLUDED__