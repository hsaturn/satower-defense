/*
 * CWindow.cpp
 *
 *  Created on: 10 ao�t 2010
 *      Author: saturn
 */

#include "CWindow.hpp"
#include <SDL/SDL_gfxPrimitives.h>

#include <iostream>

using namespace std;

void CWindow::draw(SDL_Surface* pDest)
{
	boxColor(
			pDest,
			moRegion.x,moRegion.y,
			moRegion.x+moRegion.w-1,moRegion.y+moRegion.h-1,miBgColor);

}

SDL_Rect CWindowBorder::getOuterRegion() const
{
	SDL_Rect oRect(getInnerRegion());

	oRect.x-=miBorderSize;
	oRect.y-=miBorderSize;
	oRect.w+=2*miBorderSize;
	oRect.h+=2*miBorderSize;

	return oRect;
}

void CWindowBorder::draw(SDL_Surface* pDest) const
{
	SDL_Rect oBorder=getOuterRegion();
	// top
	boxColor(
			pDest,
			oBorder.x,oBorder.y,
			oBorder.x+oBorder.w-1,oBorder.y+miBorderSize-1,miColor);

	// bottom
	boxColor(
			pDest,
			oBorder.x,oBorder.y+oBorder.h-miBorderSize,
			oBorder.x+oBorder.w-1,oBorder.y+oBorder.h-1,miColor);

	// left
	boxColor(
			pDest,
			oBorder.x,oBorder.y+miBorderSize,
			oBorder.x+miBorderSize-1,oBorder.y+oBorder.h-miBorderSize-1,miColor);

	// right
	boxColor(
			pDest,
			oBorder.x+oBorder.w-miBorderSize,oBorder.y+miBorderSize,
			oBorder.x+oBorder.w-1,oBorder.y+oBorder.h-miBorderSize-1,miColor);

	mpoWindow->draw(pDest);
}

void CWindowTransparent::draw(SDL_Surface* pDest) const
{
	SDL_SetAlpha( pDest, SDL_SRCALPHA | SDL_RLEACCEL, 64);
	mpoWindow->draw(pDest);
}
