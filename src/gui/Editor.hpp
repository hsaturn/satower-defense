#pragma once

#include <cfileparser.hpp>
#include <rect.hpp>
#include <coord.hpp>
#include <Widget.hpp>

using namespace std;

class Editor : public Widget
{
	public:
		Editor(CFileParser*);
		virtual ~Editor() {};

		void highlight(SDL_Surface *dest) const override {}
};
