/*
 * rect.hpp
 *
 *  Created on: 4 ao�t 2010
 *      Author: saturn
 */

#ifndef RECT_HPP_
#define RECT_HPP_
#include "coord.hpp"

class CFileParser;

class rect
{
public:
	rect(){};
	rect(const coord &tl, const coord &br)
	:
		mTopLeft(tl),
		mBottomRight(br){}

	void readDef(CFileParser* poDef);

	const coord&	getTopLeft()	const	{ return mTopLeft; }
	const coord&	getBottomRight()const	{ return mBottomRight; }

	float x1() const { return mTopLeft.x(); }
	float x2() const { return mBottomRight.x(); }
	float y1() const { return mTopLeft.y(); }
	float y2() const { return mBottomRight.y(); }

	bool isInside(coord c) const;
private:
	coord	mTopLeft;
	coord	mBottomRight;
};

#endif /* RECT_HPP_ */
