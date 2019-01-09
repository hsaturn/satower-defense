#pragma once

#include <map>
#include <game_extension.hpp>


class OgolEditor : public GameExtension
{
	public:

		bool readTheme(string &sItem, CFileParser *oDef) override;


};
