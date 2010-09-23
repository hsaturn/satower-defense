/*
 * ogolPolygon.cpp
 *
 *  Created on: 9 ao�t 2010
 *      Author: saturn
 */

#include <SDL/SDL_gfxPrimitives.h>
#include "ogolPolygon.hpp"
#include <iostream>

using namespace std;

ogolPolygon::ogolPolygon()
:
	mx(0),
	my(0),
	mc(0),
	miSize(0),
	miArraySpace(0)
{

}

ogolPolygon::~ogolPolygon()
{
	if (mx)
	{
		free(mx);
		free(my);
		free(mc);
	}
}

void ogolPolygon::draw(SDL_Surface *p, Uint32 iFillColor) const
{
	filledPolygonColor(p,mx,my,miSize,iFillColor);
	for(int i=0;i<miSize-1;i++)
	{
		if (mc[i] & 0x1)
		{
			aalineColor(p,mx[i],my[i],mx[i+1],my[i+1],mc[i]);
		}
	}
}
void ogolPolygon::addPoint(Sint16 x,Sint16 y, Uint32 iColor)
{
	if (miSize)
	{
		if (x==mx[miSize-1] && y==my[miSize-1])
		{
			mc[miSize-1]=iColor;
			return;
		}
	}
	if (mx==0)
	{
		miArraySpace=64;
		mx=(Sint16*)malloc(miArraySpace*sizeof(Sint16));
		my=(Sint16*)malloc(miArraySpace*sizeof(Sint16));
		mc=(Uint32*)malloc(miArraySpace*sizeof(Uint32));
	}
	else if (miSize>=miArraySpace)
	{
		miArraySpace<<=2;
		mx=(Sint16*)realloc(mx,miArraySpace*sizeof(Sint16));
		my=(Sint16*)realloc(my,miArraySpace*sizeof(Sint16));
		mc=(Uint32*)realloc(mc,miArraySpace*sizeof(Uint32));
	}
	mx[miSize]=x;
	my[miSize]=y;
	mc[miSize]=iColor;
	miSize++;
}
