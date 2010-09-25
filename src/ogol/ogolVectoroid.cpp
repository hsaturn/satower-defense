/*
 * ogolVectoroid.cpp
 *
 *  Created on: 2 ao�t 2010
 *      Author: saturn
 */


#include <SDL/SDL_gfxPrimitives.h>

#include <cfileparser.hpp>
#include "ogolVectoroid.hpp"
#include "ogol.hpp"

int giOgolVectoroidTimeMs=1000;

list<ogolVectoroid*>*	ogolVectoroid::mlstVectoroids=new list<ogolVectoroid*>;

void ogolVectoroid::create(
		const coord &start,
		const coord &last,
		const coord &speed,
		Uint32 iColor,
		int iRotationSpeed)
{
	ogolVectoroid*	p=new ogolVectoroid(start,last,speed,iColor,iRotationSpeed);
	if (p)
		mlstVectoroids->push_back(p);
}

ogolVectoroid::ogolVectoroid(
		const coord &start,
		coord last,
		const coord &speed,
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

void ogolVectoroid::_update(int iTimerEllapsedms)
{
	miTimeEllapsed+=iTimerEllapsedms;
	mfLength=mfInitialLength+mfInitialLength*miTimeEllapsed/giOgolVectoroidTimeMs;
	float fMul(mfRotationSpeed*iTimerEllapsedms/1000);
	coord delta(mSpeed);
	delta.multiplyBy(fMul);
	mStart.add(delta);
	float fRotate=fMul;
	mAngle.rotateBy(fRotate);
	mAngle.normalize();
	if (miTimeEllapsed<giOgolVectoroidTimeMs-100)
	{
		int iRed=miColor & 0xFF000000>>24;
		if (iRed<245)
		{
			iRed+=10;
		}
		miColor=iRed<<24 | (miColor & 0x00FFFF00);
	}
	int iOpacity=255-((miTimeEllapsed<<8)/giOgolVectoroidTimeMs);
	if (iOpacity<0)
		iOpacity=0;
	miColor=(miColor & 0xFFFFFF00)|iOpacity;
}


void ogolVectoroid::update(int iTimerEllapsedms)
{
	list<ogolVectoroid*>::iterator oit=mlstVectoroids->begin();
	while(oit!=mlstVectoroids->end())
	{
		ogolVectoroid* p=*oit;
		if (p->miTimeEllapsed>giOgolVectoroidTimeMs)
		{
			oit=mlstVectoroids->erase(oit);
			delete p;
		}
		else
		{
			oit++;
			p->_update(iTimerEllapsedms);
		}
	}
}
void ogolVectoroid::draw(SDL_Surface* p)
{
	list<ogolVectoroid*>::iterator oit=mlstVectoroids->begin();
	while(oit!=mlstVectoroids->end())
	{
		(*oit++)->_draw(p);
	}
}

void ogolVectoroid::_draw(SDL_Surface* p)
{
	coord oEnd(mAngle);
	oEnd.multiplyBy(mfLength);
	oEnd.add(mStart);
	aalineColor(back,mStart.x(),mStart.y(),oEnd.x(),oEnd.y(),miColor);
}
