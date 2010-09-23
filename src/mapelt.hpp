/*
 * mapelt.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef MAPELT_HPP_
#define MAPELT_HPP_

#include "coord.hpp"

class mapelt
{
	public:
		mapelt(){};
		mapelt(coord c) : mpos(c) {};

		void draw(SDL_Surface* p) const{	drawAt(mpos,p); };
		virtual void drawAt(const coord&,SDL_Surface*) const=0;

		virtual int update(int iTimeEllapsedms){ return false;};

		const coord	getCoord() const { return mpos; };
		virtual void	setCoord(const coord pos)	{ mpos=pos; };

	protected:
		coord mpos;
};

#endif /* MAPELT_HPP_ */
