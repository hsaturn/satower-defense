/*
 * CurrentNextArea.hpp
 *
 *  Created on: 22 august 2010
 *      Author: saturn
 */

#ifndef CURRENTNEXTAREA_HPP_
#define CURRENTNEXTAREA_HPP_

#include "rect.hpp"
#include "walkers.hpp"

class text;

class CFileParser;
// Current / Next walker place holder.

class CurrentNextArea
{
public:
	CurrentNextArea() : mpFont(0){};

	void readDef(CFileParser*);
	void displayCurrentNextWalkers(int iTimeEllapsedMs,SDL_Surface*,const walkers::const_iterator* poit) const;

private:
	text*	mpFont;
	rect	mArea;
	int		miRows;
	int		miCellSize;
	int		miMarginY;
	int		miTextX;
};

#endif /* CURRENTNEXTAREA_HPP_ */
