#include "dic.hpp"
#include "utils.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <algorithm>

void Dictionary::Load(std::string filname)
{
	std::ifstream fin(filname.c_str());
	//FILE* idFileIn=fopen(filname.c_str(),"r");
	//char buff[2048];	

	std::string strTemp;
	int count=0;
	//char c;
	//int posBuf=0;
		//dic.insert(std::pair<std::string, std::string>("last", "stub"));
/*	while (fread(&c,1,1,idFileIn))
	{
		std::cout<<c;
		if (posBuf>2000) 
		{
			std::cout<<"buffer aaaaaaaaaaaaa"<<std::endl;
			break;
		}
		buff[posBuf++]=c;
		if (c=='\n')
		{
			buff[posBuf++]='\0';
			strTemp = std::string(buff);
			dic.insert(std::pair<std::string, std::string>("二", strTemp));
			posBuf=0;
		}
	}
	*/
	
	while( std::getline( fin, strTemp ) )
	{

		int pos = strTemp.find('\t'); // Split at tab.
		try {
			std::string left = strTemp.substr(0, pos);
			std::string right = strTemp.substr(pos);

						
			dic.insert(std::pair<std::string, std::string>(left, right));
			//dic.insert(std::pair<std::string, std::string>("二", strTemp));
			count++;
			
		}
		catch(...){}


	}

	std::cout<<count <<" items load in dictionary"<<std::endl;

}

std::string Dictionary::LookUp(std::string key)
	{
		std::cout<<"searching dictionoary for "<<key<<std::endl;
		std::string strResult="";
		//iterate all dic,
		 std::map<std::string,std::string>::iterator it;
		 for ( it=dic.begin() ; it != dic.end(); it++ )
		 {
			 if (key.find((*it).first )!=std::string::npos)
				strResult+= (*it).first + " - " + (*it).second +"\n";

		
		 }
		// it=dic.begin() ;
		//strResult+= (*it).first + " => " + (*it).second +"\n";
		std::cout<<strResult<<std::endl;
		std::cout<<"dic done "<<key<<std::endl;
		if (strResult=="") return "empty";
		return strResult;
	}	
