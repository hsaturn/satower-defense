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

#include "vars.hpp"

using namespace std;


template <class T>
class coord_base
{
	public:

		coord_base(SDL_Event &);
		//coord_base(int);	// Voir constantes COORD_XXX
		coord_base() : mx(0),my(0){};
		coord_base(T x,T y) : mx(x),my(y){};
		// Angle : rad
		coord_base(T angle){ mx=cos(angle); my=sin(angle);}

		void snapToGrid();

		inline void substract(const coord_base &p){ mx-=p.mx; my-=p.my; };
		inline void add	(const coord_base &p){ mx+=p.mx; my+=p.my; };

		void operator -=(const coord_base &p){ substract(p); };
		void operator +=(const coord_base &p){ add(p); };

		inline void scalarMultiply(T k){ mx*=k; my*=k; };
		void scalarDivide(T k)  { mx/=k; my/=k; };

		inline void multiplyBy(const coord_base &p);
		inline void multiplyBy(T k) { scalarMultiply(k);}
		inline void rotateBy(float fAngle){ multiplyBy(coord_base<T>(fAngle));}
		inline void rotateBy(const coord_base &p) { multiplyBy(p); };

		inline T x() const { return mx; }
		inline T y() const { return my; }

		coord_base<int> getTilesCoord() const;

		void setX(T x){mx=x;}
		void setY(T y){my=y;}
		void setXY(T x,T y){mx=x; my=y;}

		void normalize();
		T norm	()	const;
		T module()	const;

		T	distanceTo(const coord_base &p) const;
		bool inSquare(const coord_base &TopLeft, const coord_base &BottomRight) const;

		friend ostream& operator << (ostream&, const coord_base<float>);

		operator SDL_Rect() const {
			SDL_Rect rect;
			rect.x=mx;
			rect.y=my;
			return rect; }
		void readDef(CFileParser*);

	private:
		T mx;
		T my;

};

typedef coord_base<float>	coord;
typedef coord_base<int>		coordi;

// FIXME
extern const coord mapTL;
// extern const coord mapBR;


template <class T>
coordi coord_base<T>::getTilesCoord() const
{
	coordi t(
			(int)((mx-mapTL.x())/tileSizeX+0.5),
			(int)((my-mapTL.y())/tileSizeY+0.5));
	return t;
}

template <class T>
void coord_base<T>::normalize()
{
	T fNorm=norm();
	if (fNorm)
	{
		mx/=fNorm;
		my/=fNorm;
	}
}

template <class T>
T coord_base<T>::norm() const
{
	return sqrt(mx*mx+my*my);
}

template <class T>
T coord_base<T>::module() const
{
	return atan2(my,mx);
}

template <class T>
coord_base<T>::coord_base(SDL_Event &e)
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
		cerr << "Error : Trying to use coord_base(SDL_Event) for unimplemented event : " << e.type << endl;
		break;
	}

}

/*
template <class T>
coord_base<T>::coord_base(int i)
{
	switch(i)
	{
		case COORD_MOUSE:
			int x,y;
			SDL_GetMouseState(&x,&y);
			mx=x; my=y;
			break;
		default:
			cerr << "coord_base<T> bad argument : " << i << endl;
			break;
	}
}
*/
template <class T>
T coord_base<T>::distanceTo(const coord_base &p) const
{
	return sqrt((mx-p.mx)*(mx-p.mx)+(my-p.my)*(my-p.my));
}

template <class T>
void coord_base<T>::snapToGrid()
{
	this->operator-=(mapTL);
	mx=int(mx/tileSizeX)*tileSizeX;
	my=int(my/tileSizeY)*tileSizeY;
	this->operator+=(mapTL);
}

template<class T>
bool coord_base<T>::inSquare(const coord_base &mapTL, const coord_base &mapBR) const
{
	return (mx>=mapTL.mx) && (my>=mapTL.my) && (mx<mapBR.mx) && (my<mapBR.my);
}

template<class T>
void coord_base<T>::readDef(CFileParser* poDef)
{
	poDef->getExpectedChar("(");
	mx=poDef->getNextLong("x coord");
	poDef->getExpectedChar(",");
	my=poDef->getNextLong("y coord");
	poDef->getExpectedChar(")");
}
template<class T>
void coord_base<T>::multiplyBy(const coord_base &p)
{
	T x=mx*p.x()-my*p.y();
	my=mx*p.y()+my*p.x();
	mx=x;
}

#endif /* COORD_HPP_ */
