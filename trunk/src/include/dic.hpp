#include <map>
#include <string>
#include <iostream>


class Dictionary
{
private:
	std::map<std::string,std::string> dic;
	
public:
	void Load(std::string filname);
	std::string LookUp(std::string key);
	

};
