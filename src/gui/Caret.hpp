#pragma once

#include <Widget.hpp>

using namespace std;

class Caret : public Widget
{
	public:
		Caret(Uint32 width, Uint32 height);

		void render(SDL_Surface*, Uint32 ellapsed) override;

		void setPos(Uint32 x, Uint32 y);
		void setSize(Uint32 x, Uint32 y);

		const Uint32 blink=500;

		void setColor(Uint32 newColor) { color=newColor; }

		void setInsertMode(bool newMode);
		bool isInsertMode() const { return insert; }

		friend ostream& operator << (ostream& out, const Caret& caret)
		{
			out << "(" << caret.caret.x << ',' << caret.caret.y << ')';
			return out;
		}

	private:
		void recalcInsertCaret();

		SDL_Rect caret;	// bloc caret
		SDL_Rect insert_caret;  // line caret
		Uint32	color;
		bool     insert;
		Uint32 mblink;
};
