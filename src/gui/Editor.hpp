#pragma once

#include <cfileparser.hpp>
#include <rect.hpp>
#include <coord.hpp>
#include <Widget.hpp>
#include <game_extension.hpp>
#include <map>
#include "Caret.hpp"
#include <text.hpp>

using namespace std;

class Editor : public Widget
{
	public:
		Editor(CFileParser*);
		virtual ~Editor() {};

		void highlight(SDL_Surface *dest) const override {}
		void render(SDL_Surface*, Uint32 ellapsed) override;

		inline void renderText(SDL_Surface* surface, int line, const string&);

	private:
		int window_rows;	// number of rows in the area
		int window_cols;	// number of lines in the area
		int top_row;		// first row displayed
		int left_col;		// first col displayed
		int font_w, font_h;
		map<int, string> lines;
		Caret caret;
		text* mpFont;
};
