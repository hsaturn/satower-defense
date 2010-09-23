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
}

bool rect::isInside(coord c) const
{
	return !(c.x()<mTopLeft.x() || c.y()<mTopLeft.y() || c.x()>mBottomRight.x() || c.y()>mBottomRight.y());
}
