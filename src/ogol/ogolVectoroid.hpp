/*
 * ogolVectoroid.hpp
 *
 *  Created on: 25 sept. 2010
 *      Author: saturn
 */

#ifndef OGOLVECTOROID_HPP_
#define OGOLVECTOROID_HPP_

#include <list>
#include "../coord.hpp"

using namespace std;

class SDL_Surface;

extern int giOgolVectoroidTimeMs;

class ogolVectoroid
{
public:

	// This method may, or may not
	// create the vectoroid depending on the
	// system parameters (max vectoroids count for example)
	static void create(
			const coord &start,			// start of line
			const coord &last,			// end of line
			const coord &speed,
			Uint32 iColor,			// ...
			int iRotationSpeed);

	static void update(int iTimerEllapsedms);
	static void draw(SDL_Surface*);

	static long count() { return mlstVectoroids->size(); }

private:
	ogolVectoroid(
			const coord &start,			// start of line
			      coord last,				// end of line
			const coord &speed,			// Speed
			Uint32 iColor,			// ...
			int iRotationSpeed);		// rotation speed (degrees/sec)

	void _draw(SDL_Surface*);
	void _update(int iTimerEllapsedms);

private:
	coord	mStart;
	coord	mSpeed;
	coord	mAngle;
	Uint32	miColor;
	float	mfRotationSpeed;	// rad/sec
	float	mfLength;
	float	mfInitialLength;
	int		miTimeEllapsed;

	static list<ogolVectoroid*>*	mlstVectoroids;
};

#endif /* OGOLVECTOROID_HPP_ */
