/*
 * rect.cpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */

#include "rect.hpp"
#include <cfileparser.hpp>

void rect::readDef(CFileParser* poDef)
{
	mTopLeft.readDef(poDef);
	poDef->getExpectedChar("-");
	mBottomRight.readDef(poDef);
	setSdlRect();
}

void rect::setSdlRect()
{
   if (mSdlRect == nullptr) mSdlRect = new SDL_Rect;
   mSdlRect->x = x1();
   mSdlRect->y = y1();
   mSdlRect->w = x2()-x1();
   mSdlRect->h = y2()-y1();
}

bool rect::isInside(coord c) const
{
	return !(c.x()<mTopLeft.x() || c.y()<mTopLeft.y() || c.x()>mBottomRight.x() || c.y()>mBottomRight.y());
}

