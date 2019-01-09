/*
 * text.cpp
 *
 *  Created on: 3 ao�t 2010
 *      Author: saturn
 */
#include "ogol.hpp"
#include "text.hpp"
#include "translate.hpp"
#include "vars.hpp"
#include "game.hpp"

#include <iostream>

using namespace std;

bool text::mbInit=false;

void text::init()
{
	if (!mbInit)
	{
		if (TTF_Init() == -1)
		{
			cerr << _T("Unable to initialize TTF subsystem.");
		}
		else
		{
			mbInit=true;
		}
	}
}

// FIXME find a wait to call TTF_Quit()

text::text(CFileParser* poDef)
{
	string sName=poDef->getNextString("font name");
	miSize=poDef->getNextLong("font size");
	string sColor=poDef->getNextIdentifier("color");
	Uint32 iColor=ogolPencil::stringToColor(sColor);

	if (iColor==0)
		poDef->throw_("text","Unknown color ["+sColor+"]");

	mColor.r=iColor >> 24;
	mColor.g=(iColor & 0x00FF0000) >>  16;
	mColor.b=(iColor & 0x0000FF00) >> 8;

	if (!load(sName,miSize))
	{
		poDef->throw_("text","Unable to load font ["+ sName+"]");
	}
}

text::text(const string &sFont,int iSize, SDL_Color oColor)
	:
	mColor(oColor)
{
	load(sFont,iSize);
}

bool text::load(const string &sFont,int iSize)
{
	bool bRet=true;
	text::init();

	string sFontFile=gpGame->findRsrcFile("fonts/"+sFont+".TTF");
	mpFont=TTF_OpenFont(sFontFile.c_str(),iSize);
	if (mpFont==0)
	{

		bRet=false;
		cerr << _T("Unable to load font") << ' ' << sFont << ',' << sFontFile << endl;
	}
	return bRet;
}

text::~text()
{
	if (mpFont)
	{
		TTF_CloseFont(mpFont);
	}
}

void text::drawSolidShadowed(SDL_Surface*dest, coord pos, const string &sText, Uint32 iColor) const
{
	coord p;
	int iOffset=miSize/15;
	if (iOffset<1)
		iOffset=1;
	p.setX(pos.x()+iOffset);
	p.setY(pos.y()+iOffset);
	drawSolid(dest,p,sText,0xFF);
	drawSolid(dest,pos,sText,iColor);
}

void text::drawSolid(SDL_Surface *dest, const coord& pos, char c, Uint32 iColor) const
{
	SDL_Color oColor;
	if (iColor)
	{
		oColor.r=iColor >> 24;
		oColor.g=(iColor & 0x00FF0000) >>  16;
		oColor.b=(iColor & 0x0000FF00) >> 8;
	}
	else
		oColor = mColor;

	if (mpFont)
	{
		char p[2];
		p[0]=c;
		p[1]=0;

		SDL_Surface *text;

		text=TTF_RenderText_Solid(mpFont,p,oColor);
		SDL_Rect rect(pos);
		SDL_BlitSurface(text,NULL,dest, &rect);
		SDL_FreeSurface(text);
	}
}

void text::drawSolid(SDL_Surface*dest, coord pos, const string &sText, Uint32 iColor) const
{
	SDL_Color oColor;

	if (iColor)
	{
		oColor.r=iColor >> 24;
		oColor.g=(iColor & 0x00FF0000) >>  16;
		oColor.b=(iColor & 0x0000FF00) >> 8;
	}
	else
		oColor=mColor;

	if (mpFont)
	{
		string s(sText);
		coord row(0,size());
		while(s.length())
		{
			unsigned long cr=s.find("\n");
			if (cr!=string::npos)
			{
				SDL_Surface *text;

				text=TTF_RenderText_Blended(mpFont,s.substr(0,cr).c_str(),oColor);
				SDL_Rect rect(pos);
				SDL_BlitSurface(text,NULL,dest, &rect);
				SDL_FreeSurface(text);
				s=s.substr(cr+1);
			}
			else
			{
				SDL_Surface *text;

				text=TTF_RenderText_Blended(mpFont,s.c_str(),oColor);
				SDL_Rect rect(pos);
				SDL_BlitSurface(text,NULL,dest, &rect);
				SDL_FreeSurface(text);
				break;
			}
			pos.add(row);
		}
	}
}
