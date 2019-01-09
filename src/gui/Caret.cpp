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
	setInsertMode(true);
}

void Caret::render(SDL_Surface* surface, Uint32 ellapsed)
{
	mblink += ellapsed;
	if (mblink > blink)
	{
		if (insert)
			SDL_FillRect(surface, &insert_caret, color);
		else
			SDL_FillRect(surface, &caret, color);
		if (mblink > 2*blink)
		{
			mblink -= 2*blink;
		}
	}
}

void Caret::setPos(Uint32 x, Uint32 y)
{
	if (caret.x == x && caret.y == y) return;
	caret.x=x;
	caret.y=y;
	recalcInsertCaret();
}

void Caret::setSize(Uint32 w, Uint32 h)
{
	if (caret.w == w && caret.h == h) return;
	caret.w=w;
	caret.h=h;
	recalcInsertCaret();
}

void Caret::setInsertMode(bool newMode)
{
	if (newMode == insert) return;
	insert = newMode;
	recalcInsertCaret();
}

void Caret::recalcInsertCaret()
{
	insert_caret.x=caret.x;
	insert_caret.y=caret.y;
	insert_caret.h=caret.h;
	if (insert)
		insert_caret.w=2;
	else
		insert_caret.w=caret.w;
	cout << "c=" << caret.x << ',' << caret.y << endl;
	cout << "ic=" << insert_caret.x << ',' << insert_caret.y << endl;
}
