/*
 * ogolPolygon.hpp
 *
 *  Created on: 9 ao�t 2010
 *      Author: saturn
 */

#ifndef OGOLPOLYGON_HPP_
#define OGOLPOLYGON_HPP_

#include <SDL/SDL.h>

class ogolPolygon
{
public:
	ogolPolygon();
	~ogolPolygon();
	void addPoint(Sint16 x,Sint16 y, Uint32 uColor);

	Sint16*	getXArray() { return mx; };
	Sint16* getYArray() { return my;	};
	int		getSize() const { return miSize; }
	void draw(SDL_Surface*, Uint32 iFillColor) const;

private:
	Sint16*	mx;
	Sint16* my;
	Uint32*	mc;
	int		miSize;
	int		miArraySpace;
};

#endif /* OGOLPOLYGON_HPP_ */
