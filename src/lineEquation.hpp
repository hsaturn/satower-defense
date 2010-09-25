/*
 * line.hpp
 *
 *  Created on: 25 sept. 2010
 *      Author: saturn
 */

#ifndef LINE_HPP_
#define LINE_HPP_

class coord;

class lineEquation
{
public:
	float a;
	float b;
	float c;

	/**
	 * Find the intersection with another line
	 * @return null if no intersection or the coord of the intersection
	 */
	coord* intersection(const lineEquation &) const;

	float distanceTo(const coord &p) const;
};

#endif /* LINE_HPP_ */
