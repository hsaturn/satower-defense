/*
 * text.cpp
 *
 *  Created on: 3 ao�t 2010
 *      Author: saturn
 */

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>

#include "coord.hpp"

class CFileParser;

using namespace std;

class text
{
public:
	// TODO il faudrait une factory pour �viter les duplicates...
	text(const string &sFont, int iSize,SDL_Color);
	text(CFileParser*);
	~text();

	void drawSolid			(SDL_Surface*, coord pos,const string &sText, Uint32 iColor=0) const;
	void drawSolidShadowed	(SDL_Surface*, coord pos,const string &sText, Uint32 iColor=0) const;

	static void init();
	static bool mbInit;

	int size() const { return miSize; }

	void readDef(CFileParser*);	// NOT IMPLEMENTED
private:
	bool	load(const string &sFont,int iSize);
	TTF_Font*	mpFont;		// FIXME Objet et factory pour ca
	SDL_Color	mColor;
	int			miSize;

};

#endif /* TEXT_HPP_ */
