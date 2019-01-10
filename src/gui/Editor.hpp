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
		void onKey(const SDL_KeyboardEvent& event) override;
		void onFocus() override;
		void onLeaveFocus() override;
		void onMouse(const SDL_Event& event, const coord& relative) override;

		inline void renderText(SDL_Surface* surface, int line, const string&);

	private:
		void updateCaret();
		void ensureCaretVisible();
		void virtualCol();

		int window_rows;	// number of rows in the area
		int window_cols;	// number of lines in the area
		int top_row;		// first row displayed 1 is first
		int left_col;		// first col displayed 0 is first
		int font_w, font_h;
		int row, col;		// current row / column (real)
		map<int, string> lines;
		Caret caret;
		text* mpFont;

		int delay;
		int interval;
		int unicode;
		bool autoindent;
		int virtual_col;
};
