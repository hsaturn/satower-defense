#pragma once

#include <cfileparser.hpp>
#include <rect.hpp>
#include <coord.hpp>
#include <Widget.hpp>

using namespace std;

class Canvas : public Widget
{
	public:
		Canvas(CFileParser*);
		virtual ~Canvas() {};

		void highlight(SDL_Surface *dest) const override {}
};
