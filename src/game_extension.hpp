#pragma once

#include <cfileparser.hpp>

class GameExtension
{
	public:
		GameExtension(){}
		virtual ~GameExtension() {}

		virtual bool readTheme(string &sItem, CFileParser *oDef)=0;

		
};
