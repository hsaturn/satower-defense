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
	rect() : mSdlRect(nullptr) {};

	rect(const coord &tl, const coord &br)
	:
		mTopLeft(tl),
		mBottomRight(br),
		mSdlRect(nullptr)
		{}


	void readDef(CFileParser* poDef);

	const coord&	getTopLeft()	const	{ return mTopLeft; }
	const coord&	getBottomRight()const	{ return mBottomRight; }

	float x1() const { return mTopLeft.x(); }
	float x2() const { return mBottomRight.x(); }
	float y1() const { return mTopLeft.y(); }
	float y2() const { return mBottomRight.y(); }

	bool isInside(coord c) const;

	operator SDL_Rect* () const { return mSdlRect; }

private:
	void setSdlRect();
	coord	mTopLeft;
	coord	mBottomRight;
	SDL_Rect* mSdlRect;
};

#endif /* RECT_HPP_ */
