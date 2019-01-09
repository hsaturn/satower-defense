#pragma once

#include <cfileparser.hpp>
#include <rect.hpp>
#include <coord.hpp>
#include <Widget.hpp>
#include <game_extension.hpp>
#include <map>
#include "Caret.hpp"

using namespace std;

class Editor : public Widget
{
	public:
		Editor(CFileParser*);
		virtual ~Editor() {};

		void highlight(SDL_Surface *dest) const override {}
		void render(SDL_Surface*, Uint32 ellapsed) override;

	private:
		map<int, string> lines;
		Caret caret;
};
