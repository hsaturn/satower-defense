#include "Caret.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_gfxBlitFunc.h>

Caret::Caret(Uint32 w, Uint32 h)
:
Widget("caret"),
mblink(0)
{
	setPos(0,0);
	setSize(w,h);
}

void Caret::render(SDL_Surface* surface, Uint32 ellapsed)
{
	mblink += ellapsed;
	if (mblink > blink)
	{
		SDL_FillRect(surface, &caret, color);
		if (mblink > 2*blink)
		{
			mblink -= 2*blink;
		}
	}
}

void Caret::setPos(Uint32 x, Uint32 y)
{
	caret.x=x;
	caret.y=y;
}

void Caret::setSize(Uint32 w, Uint32 h)
{
	caret.w=w;
	caret.h=h;
}
