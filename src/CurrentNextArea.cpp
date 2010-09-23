/*
 * CurrentNext.cpp
 *
 *  Created on: 22 august 2010
 *      Author: saturn
 */

#include "CurrentNextArea.hpp"
#include "text.hpp"
#include <cfileparser.hpp>
#include "walkerBase.hpp"
#include "game.hpp"
#include "vars.hpp"

void CurrentNextArea::readDef(CFileParser* poDef)
{
	poDef->getExpectedChar("{");
	while(poDef->good())
	{
		if (poDef->peekChar()=='}')
		{
			poDef->getExpectedChar("}");
			break;
		}
		string s=poDef->getNextIdentifier("current_area item");

		if (s=="area")
			mArea.readDef(poDef);
		else if (s=="font")
			mpFont=new text(poDef);
		else if (s=="rows")
			miRows=poDef->getNextLong("rows value");
		else if (s=="cell_size")
			miCellSize=poDef->getNextLong("cell_size value");
		else if (s=="margin_y")
			miMarginY=poDef->getNextLong("margin_y value");
		else if (s=="text_x")
			miTextX=poDef->getNextLong("text offset value");
		else
			poDef->throw_("current_next area","Unknown data ["+s+"]");
	}
	if (mpFont==0)
		poDef->throw_("current_next area","font must be defined !");
}

void CurrentNextArea::displayCurrentNextWalkers(int iTimeEllapsedMs,SDL_Surface* dest,const walkers::const_iterator* poit) const
{
	static coord oAngle(1,0);
	static float fScale(0.95);
	static float fScaleInc(0.01);
	if (mpFont==0)
	{
		cerr << "Warning : CurrentNextArea has no font. Probably missing current_next section in theme definition" << endl;
	}
	else
	{
		walkers::const_iterator oit=(*poit);
		coord pos(miTextX,mArea.getTopLeft().y());
		coord row(0,miCellSize+miMarginY);
		coord posW(mArea.getTopLeft().x()+miCellSize/2,mArea.getTopLeft().y()+miCellSize/2);
		int i=1;

		while(i<=miRows && oit!=walkers::end())
		{
			walkerBase* pWalker=(oit->second);
			string s=pWalker->getShortDesc()+"\n";
			s+="HP: "+to_string(pWalker->getHealth()*gpGame->getHealthFactor())+", "+to_string(pWalker->getBank())+"$\n";
			s+="Speed : "+to_string(pWalker->getSpeed())+" m/s";
			mpFont->drawSolid(dest,pos,s);
			float fScale2(fScale);
			pWalker->drawNative(posW,dest, oAngle,fScale2);
			pos.add(row);
			posW.add(row);
			oit++;
			i++;
		}
		fScale+=fScaleInc;
		if (fScale>1.0)
		{
			fScaleInc=-fScaleInc;
			fScale=1.0;
		}
		else if (fScale<0.7)
		{
			fScaleInc=-fScaleInc;
			fScale=0.7;
		}
		oAngle.multiplyBy(coord(0.01));

	}
}
