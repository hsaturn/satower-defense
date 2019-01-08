/*
 * button.cpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */
#include "button.hpp"
#include <SDL/SDL_gfxPrimitives.h>


Button::Button(CFileParser* poDef) : Widget(poDef)
{
	try
	{
		poDef->getExpectedChar("{");
		while(poDef->good())
		{
			if (poDef->peekChar()=='}')
			{
				poDef->getExpectedChar("}");
				break;
			}
			string s=poDef->getNextIdentifier("button data");

			if (s=="area")
				mRect.readDef(poDef);
			else if (s=="action")
				msAction=poDef->getNextIdentifier("action name");
			else
				poDef->throw_("button","Unknown button data ["+s+"]");
		}
	}
	catch (CSException *p)
	{
		cerr << "Error while reading button " << msName << endl;
		cerr << p->getCompleteError() << endl;
	}
}

void Button::highlight(SDL_Surface* dest) const
{
	boxColor(dest,mRect.x1(),mRect.y1(),mRect.x2(),mRect.y2(),0x00000080);
}
