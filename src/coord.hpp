/*
 * coord.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef COORD_HPP_
#define COORD_HPP_

#define COORD_MOUSE 1

#include <SDL/SDL.h>
#include <iostream>
#include <cfileparser.hpp>
#include <math.h>
#include "lineEquation.hpp"

#include "vars.hpp"

using namespace std;

class coord
{
	public:

		coord(const SDL_Event &);
		//coord(int);	// Voir constantes COORD_XXX
		coord() : mx(0),my(0){};
		coord(float x,float y) : mx(x),my(y){};
		// Angle : rad
		coord(float angle){ mx=cos(angle); my=sin(angle);}

		void snapToGrid();

		inline void substract(const coord &p){ mx-=p.mx; my-=p.my; };
		inline void add	(const coord &p){ mx+=p.mx; my+=p.my; };

		void operator -=(const coord &p){ substract(p); };
		void operator +=(const coord &p){ add(p); };

		inline void scalarMultiply(float k){ mx*=k; my*=k; };
		void scalarDivide(float k)  { mx/=k; my/=k; };

		inline void multiplyBy(const coord &p)
		{	float x=mx*p.x()-my*p.y();
			my=mx*p.y()+my*p.x();
			mx=x;
		}
		inline void multiplyBy(float k) { scalarMultiply(k);}
		inline void rotateBy(float fAngle){ multiplyBy(coord(fAngle));}
		inline void rotateBy(const coord &p) { multiplyBy(p); };

		inline float x() const { return mx; }
		inline float y() const { return my; }

		Uint32 getTilesCoordX() const;
		Uint32 getTilesCoordY() const;

		void setX(float x){mx=x;}
		void setY(float y){my=y;}
		void setXY(float x,float y){mx=x; my=y;}

		void normalize();

		float invnorm() const;	// Fast Inverse norm (1/norm) faster than norm()
		float module	()	const;	// (aka length()
		float argument()	const;	// aka angle

		// Find the equation of a line with p included in the line and this as a vector
		// equation is returned into oLine
		// Yet, a line equation is ax+by+c=0
		void buildLineEquation(lineEquation &oLine,const coord &oP);

		/**
		 * Usual distance
		 */
		float	distanceTo(const coord &p) const;

		/**
		 * Max abs(x1-x2),abs(y1-y2) distance (is it really a distance ...)
		 */
		float 	xyMaxDistanceTo(const coord &p) const;

		bool inSquare(const coord &TopLeft, const coord &BottomRight) const;

		friend ostream& operator << (ostream&, const coord);

		operator SDL_Rect() const {
			SDL_Rect rect;
			rect.x=mx;
			rect.y=my;
			return rect; }
		void readDef(CFileParser*);

	private:
		float mx;
		float my;

};


coord coordToPixel(coord o);

// FIXME
extern const coord mapTL;
// extern const coord mapBR;


#endif /* COORD_HPP_ */
