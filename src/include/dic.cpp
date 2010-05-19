#include "dic.hpp"
#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
Dictionary::Dictionary(std::string filname)
{

	std::ifstream fin(filname.c_str());
	char buff[2048];

	std::string strTemp;
	int count=0;
	while( std::getline( fin, strTemp ) )
	{

		int pos = strTemp.find('\t'); // Split at tab.
		try {
			std::string left = strTemp.substr(0, pos);

			std::string right = strTemp.substr(pos);



			//dic[left]=right;
			//typedef std::pair<std::string, std::string> TStrStrPair;

			//dic.insert(std::pair<std::string, std::string>(left, right));
			dic.insert(std::pair<std::string, std::string>("二", "two"));
			count++;
		}
		catch(...){}


	}
	std::cout<<count <<" items load in dictionary"<<std::endl;

}

std::string Dictionary::LookUp(std::string key)
	{
		std::string strResult="f " + dic.begin()->first +" size " +FormatHelper::ConvertToStr(dic.begin()->second.length())+" - "+ dic.begin()->second;
		std::cout<<strResult<<std::endl;
		return strResult;
	}	
