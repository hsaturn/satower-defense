/*
 * translate.cpp
 *
 *  Created on: 3 août 2010
 *      Author: saturn
 */
#include "translate.hpp"

 map<string, string>*	translate::mapTranslation=0;

void translate::init()
{
	if (mapTranslation==0)
	{
		mapTranslation=new map<string,string>;

		(*mapTranslation)["test"]="test";
	}
}

const string& translate::_T(const string& s)
{
	if (mapTranslation==0)
		translate::init();

	map<string,string>::const_iterator oit=mapTranslation->find(s);

	if (oit!=mapTranslation->end())
		return oit->second;
	else
		return s;
}
