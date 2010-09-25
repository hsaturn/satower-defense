/*
 * ogolPencil.cpp
 *
 *  Created on: 2 ao�t 2010
 *      Author: saturn
 */

#include <SDL/SDL_gfxPrimitives.h>

#include <cfileparser.hpp>
#include "ogol.hpp"
#include "ogolPolygon.hpp"
#include "../vars.hpp"

#include "../game.hpp"

ogolPencil::~ogolPencil()
{
	if (mpoPolygon)
	{
		delete mpoPolygon;
	}
}

float ogolPencil::getScale() const
{
	return gpGame->getOgolScale()*tileSizeY*mfScale/100;
}

// return 0 if bad color
Uint32 ogolPencil::stringToColor(const string &sColor, Sint16 iOpacity)
{
	//bool bRet=true;
	Uint32 iColor=0;
	if (sColor=="black")
	{
		iColor=0x000000ff;
	}
	else if (sColor=="red")
	{
		iColor=0xFF0000ff;
	}
	else if (sColor=="green")
	{
		iColor=0x00FF00ff;
	}
	else if (sColor=="blue")
	{
		iColor=0x0000FFff;
	}
	else if (sColor=="cyan")
	{
		iColor=0x00FFFFff;
	}
	else if (sColor=="yellow")
	{
		iColor=0xFFFF00ff;
	}
	else if (sColor=="white")
	{
		iColor=0xFFFFFFff;
	}
	else if (sColor=="lime")
	{
		iColor=0x32CD32ff;
	}
	else if (sColor=="magenta")
	{
		iColor=0xFFFF00ff;
	}
	else if (sColor=="orange")
	{
		iColor=0xFF8000ff;
	}
	else if (sColor=="brown")
	{
		iColor=0xAA4C00ff;
	}
	else if (sColor=="gold")
	{
		iColor=0xFFD700ff;
	}
	if (iColor && iOpacity!=100)
	{
		int iPercent=(iOpacity*255/10000) &0xFF;
		iColor=(iColor & 0xFFFFFF00) | iPercent | 1; // 1 => do not return 0 (failed)
	}
	return iColor;
}

void ogolPencil::recordPoint(coord* p, bool bWithLine)
{
	if (mpoPolygon)
	{
		if (mbDown && bWithLine)
		{
			mpoPolygon->addPoint(p->x(),p->y(), miColor|0x1);
		}
		else
			mpoPolygon->addPoint(p->x(),p->y(), 0);
	}
	else
		cerr << "ogolPencil::recordPoint without polygon !";
}

ogolPolygon* ogolPencil::recordPolygon()
{
	ogolPolygon* pOldOne(mpoPolygon);
	mpoPolygon=new ogolPolygon;
	return pOldOne;
}

void ogolPencil::endRecord(ogolPolygon* p, SDL_Surface* dst, Uint32 iFillColor)
{
	mpoPolygon->draw(dst,iFillColor);
	delete mpoPolygon;
	mpoPolygon=p;
}

bool ogolPencil::setColor(const string &sColor)
{
	bool bRet=false;

	Uint32 iColor=ogolPencil::stringToColor(sColor);

	if (iColor)
	{
		setColor(iColor);
		bRet=true;
	}
	return bRet;
}

bool ogolPencil::setColor(Uint32 iColor)
{
	miColor=(miColor & 0xFF) |(iColor & 0xFFFFFF00);
	return true;
}

void ogolPencil::setOpacity(Uint16 iPercent)
{
	if (iPercent>100)
		iPercent=100;

	iPercent=(iPercent*miOpacityFactor*255/10000) &0xFF;
	miColor=(miColor & 0xFFFFFF00) | iPercent;
}
