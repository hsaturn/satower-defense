/*
 * ogolVectoroid.cpp
 *
 *  Created on: 2 ao�t 2010
 *      Author: saturn
 */


#include <SDL/SDL_gfxPrimitives.h>

#include <cfileparser.hpp>
#include "ogol.hpp"

int giVectoroids=0;

ogolVectoroid::ogolVectoroid(
		coord start,
		coord last,
		coord speed,
		Uint32 iColor,
		int iRotationSpeed)
:
	mStart(start),
	mSpeed(speed),
	miColor(iColor),
	mfRotationSpeed(iRotationSpeed*PI/180),
	miTimeEllapsed(0)
{
	last.substract(start);
	mfLength=last.norm();
	mfInitialLength=mfLength;
	mAngle=last.module();

}

void ogolVectoroid::update(int iTimerEllapsedms)
{
	miTimeEllapsed+=iTimerEllapsedms;
	mfLength=mfInitialLength+mfInitialLength*miTimeEllapsed/giOgolVectoroidTime;
	float fMul(mfRotationSpeed*iTimerEllapsedms/1000);
	coord delta(mSpeed);
	delta.multiplyBy(fMul);
	mStart.add(delta);
	float fRotate=fMul;
	mAngle.rotateBy(fRotate);
	mAngle.normalize();
	if (miTimeEllapsed<giOgolVectoroidTime-100)
	{
		int iRed=miColor & 0xFF000000>>24;
		if (iRed<245)
		{
			iRed+=10;
		}
		miColor=iRed<<24 | (miColor & 0x00FFFF00);
	}
	int iOpacity=255-((miTimeEllapsed<<8)/giOgolVectoroidTime);
	if (iOpacity<0)
		iOpacity=0;
	miColor=(miColor & 0xFFFFFF00)|iOpacity;
}

void ogolVectoroid::draw(SDL_Surface* p) const
{
	giVectoroids++;
	coord oEnd(mAngle);
	oEnd.multiplyBy(mfLength);
	oEnd.add(mStart);
	aalineColor(back,mStart.x(),mStart.y(),oEnd.x(),oEnd.y(),miColor);
}
