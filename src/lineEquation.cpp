/*
 * lineEquation.cpp
 *
 *  Created on: 25 sept. 2010
 *      Author: saturn
 */

#include "coord.hpp"

coord* lineEquation::intersection(const lineEquation &d) const
{
	coord* pIntersect=0;

	if (a==0)
	{
		// if b=0, this line is not a line, but a point (0,0) !
		// => no solution (or 0,0 should be the one)
		if (d.a && b!=0)
		{
			pIntersect=new coord((-d.b*(-c/b)-d.c)/d.a,-c/b);
		}
	}
	else
	{
		float z=a*d.b-b*d.a;
		if (z)
		{
			float y=(d.a*c-a*d.c)/z;
			pIntersect=new coord((-b*y-c)/a,y);
		}
	}
	return pIntersect;
}

float lineEquation::distanceTo(const coord &p) const
{
	float d=a*p.x()+b*p.y()+c;
	d=d*d/(a*a+b*b);	// We could optimize by using * InvSqrt()
	return sqrt(d);
}
