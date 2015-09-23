/*
 * button.cpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */
#include "button.hpp"
#include <SDL/SDL_gfxPrimitives.h>

list<Button*>	Button::mlstButtons;

Button::Button(CFileParser* poDef)
{
	try
	{
		msName=poDef->getNextIdentifier("button name");
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
		mlstButtons.push_front(this);
	}
	catch (CSException *p)
	{
		cerr << "Error while reading button " << msName << endl;
		cerr << p->getCompleteError() << endl;
	}
}

Button::~Button()
{
	list<Button*>::iterator oit=mlstButtons.begin();
	while (oit!=mlstButtons.end())
	{
		if (this==*oit)
		{
			mlstButtons.erase(oit);
			break;
		}
		oit++;
	}
}

Button* Button::searchButton(coord c)
{
	list<Button*>::const_iterator oit=mlstButtons.begin();
	while(oit!=mlstButtons.end())
	{
		Button* p=*oit++;
		if (p->isInside(c))
			return p;
	}
	return 0;
}

void Button::highlight(SDL_Surface* dest) const
{
	boxColor(dest,mRect.x1(),mRect.y1(),mRect.x2(),mRect.y2(),0x00000080);
}
