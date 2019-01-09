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

	private:
		SDL_Rect caret;
		Uint32	color;

		Uint32 mblink;
};
