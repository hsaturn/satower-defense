/*
 * coord.cpp
 *
 *  Created on: 20 juil. 2010
 *      Author: saturn
 */

#include "coord.hpp"
#include "game.hpp"

coord coordToPixel(coord o)
{
	o.add(coord(0.5,0.5));
	o.scalarMultiply(gpGame->getTileSizeX());	// FIXME

	o+=gpGame->getMapArea().getTopLeft();
	return o;
}

ostream& operator << (ostream& out, const coord c)
{
	out << '(' << c.x() << ',' << c.y() << ')';
	return out;
}

Uint32 coord::getTilesCoordX() const
{
	return (int)((mx-mapTL.x())/tileSizeX+0.5);
}
float 	coord::xyMaxDistanceTo(const coord &p) const
{
	float fx;
	float fy;

	if (p.my>my)
		fy=p.my-my;
	else
		fy=my-p.my;

	if (p.mx>mx)
		fx=p.mx-mx;
	else
		fx=mx-p.mx;

	if (fx>fy)
		return fx;
	else
		return fy;
}

Uint32 coord::getTilesCoordY() const
{
	return (int)((my-mapTL.y())/tileSizeY+0.5);
}

void coord::buildLineEquation(lineEquation& oLine,const coord &p)
{
	float &a=oLine.a;
	float &b=oLine.b;
	float &c=oLine.c;

	coord mdir(*this);
	mdir.normalize();
	if (mdir.x()>-0.001 && mdir.x()<0.001)	// For 'vertical lines', ax+c=0
	{
		b=0;
		a=1;
		c=-p.x();
	}
	else
	{
		// y=Ax+B
		// <=> Ax-y+B=0 => a=A b=-1 c=B

		// A=dy/dx => a=dy/dx
		a=mdir.y()/mdir.x();

		// p belongs to (d)
		// B=c=y-Ax
		c=p.y()-(a*p.x());
		b=-1;
	}
}

float coord::invnorm() const
{
	float x=mx*mx+my*my;
	union {
		float f;
		int i;
	} tmp;
	tmp.f = x;
	tmp.i = 0x5f3759df - (tmp.i >> 1);
	float y = tmp.f;
	return  (y * (1.5f - 0.5f * x * y * y));
}

void coord::normalize()
{
	float fInvNorm=invnorm();
	mx*=fInvNorm;
	my*=fInvNorm;
}

float coord::module() const
{
	return sqrt(mx*mx+my*my);
}


float coord::argument() const
{
	return atan2(my,mx);
}


coord::coord(SDL_Event &e)
{
	switch(e.type)
	{
	case SDL_MOUSEMOTION:
		mx=e.motion.x;
		my=e.motion.y;
		break;

	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		mx=e.button.x;
		my=e.button.y;
		break;

	default:
		cerr << "Error : Trying to use coord(SDL_Event) for unimplemented event : " << e.type << endl;
		break;
	}

}

/*

coord::coord(int i)
{
	switch(i)
	{
		case COORD_MOUSE:
			int x,y;
			SDL_GetMouseState(&x,&y);
			mx=x; my=y;
			break;
		default:
			cerr << "coord bad argument : " << i << endl;
			break;
	}
}
*/

float coord::distanceTo(const coord &p) const
{
	return sqrt((mx-p.mx)*(mx-p.mx)+(my-p.my)*(my-p.my));
}


void coord::snapToGrid()
{
	this->operator-=(mapTL);
	mx=int(mx/tileSizeX)*tileSizeX;
	my=int(my/tileSizeY)*tileSizeY;
	this->operator+=(mapTL);
}


bool coord::inSquare(const coord &mapTL, const coord &mapBR) const
{
	return (mx>=mapTL.mx) && (my>=mapTL.my) && (mx<mapBR.mx) && (my<mapBR.my);
}


void coord::readDef(CFileParser* poDef)
{
	poDef->getExpectedChar("(");
	mx=poDef->getNextLong("x coord");
	poDef->getExpectedChar(",");
	my=poDef->getNextLong("y coord");
	poDef->getExpectedChar(")");
}

