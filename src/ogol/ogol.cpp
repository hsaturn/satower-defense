/*
 * ogol.cpp
 *
 *  Created on: 29 juil. 2010
 *      Author: saturn
 */
#include <SDL/SDL_gfxPrimitives.h>

#include <cfileparser.hpp>
#include "ogol.hpp"
#include "../vars.hpp"
#include "ogolVectoroid.hpp"

extern int getRand(int,int);	// BEUARK

inline float getValue(long l)
{
	if (l<-999999 || l>999999)
		return *(float*)l;
	else
		return l;
}
ogol::ogol(ogol* poParent)
:
		mpoParent(poParent)
{
}

float* ogol::createVar(const string &sName)
{
	if (mmapVars.size()==MAX_OGOL_VARS)
	{
		throw new CSException("ogol","too many variables declared");
	}
	mmapVars[sName]=0;
	return getVar(sName);
}

float* ogol::getVar(const string &sName)
{
	map<string, float>::iterator oit=mmapVars.find(sName);
	if (oit==mmapVars.end())
	{
		if (mpoParent)
			return mpoParent->getVar(sName);
		else
			throw new CSException("ogol","variable not declared : "+sName);
	}
	else
		return &(oit->second);
}

void ogol::readDef(CFileParser* poDef)
{
	mlstOgol.clear();
	poDef->getExpectedChar("{");
	bool bAddOgolBlock;

	while(poDef->good())
	{
		bAddOgolBlock=false;
		if (poDef->peekChar()=='}')
		{
			poDef->getExpectedChar("}");
			break;
		}
		EOGOL_CMD	iCmd=OGOL_PD;
		long		iParam1=0;	// Fixme, was an int, maybe stored in int place (but must be a long)
		long		iParam2=0;
		bool		bVar=poDef->peekChar(true)=='$';
		string		sGetValue1="";
		string		sGetValue2="";

		if (bVar)
			poDef->getNextChar(false);
		string sOgolCmd=poDef->getNextIdentifier("ogol command");

		if (bVar)
		{
			iCmd=OGOL_SETVAR;
			poDef->getExpectedChar("=");
			iParam1=(long)createVar(sOgolCmd);
			sGetValue2="value";
		}
		else if (sOgolCmd=="add")
		{
			iCmd=OGOL_ADD;
			poDef->getExpectedChar("$");
			iParam1=(long)getVar(poDef->getNextIdentifier("varname"));
			sGetValue2="add value";
			poDef->getExpectedChar(",");
		}
		else if (sOgolCmd=="pu")
		{
			iCmd=OGOL_PU;
		}
		else if (sOgolCmd=="pd")
		{
			iCmd=OGOL_PD;
		}
		else if (sOgolCmd=="fw")
		{
			iCmd=OGOL_FW;
			sGetValue1="distance";
		}
		else if (sOgolCmd=="bw")
		{
			iCmd=OGOL_BW;
			sGetValue1="distance";
		}
		else if (sOgolCmd=="fwu")
		{
			iCmd=OGOL_FWU;
			sGetValue1="distance";
		}
		else if (sOgolCmd=="bwu")
		{
			iCmd=OGOL_BWU;
			sGetValue1="distance";
		}
		else if (sOgolCmd=="tr")
		{
			iCmd=OGOL_TR;
			sGetValue1="angle";
		}
		else if (sOgolCmd=="tl")
		{
			iCmd=OGOL_TL;
			sGetValue1="angle";
		}
		else if (sOgolCmd=="angle")
		{
			iCmd=OGOL_ANGLE;
			sGetValue1="angle";
		}
		else if (sOgolCmd=="circle")
		{
			iCmd=OGOL_CIRCLE;
			sGetValue1="angle";
		}
		else if (sOgolCmd=="repeat")
		{
			iCmd=OGOL_REPEAT;
			sGetValue2="repeat number";
			bAddOgolBlock=true;
		}
		else if (sOgolCmd=="filled")
		{
			iCmd=OGOL_FILLED;
			string sColor=poDef->getNextIdentifier("color");
			iParam2=ogolPencil::stringToColor(sColor);
			if (iParam2==0)
			{
				poDef->throw_("Unknown color "+sColor);
			}
			bAddOgolBlock=true;
		}
		else if (sOgolCmd=="rotates")
		{
			iCmd=OGOL_ROTATES;
			sGetValue2="number of rotations";
			bAddOgolBlock=true;
		}
		else if (sOgolCmd=="mirror")
		{
			iCmd=OGOL_MIRROR;
			bAddOgolBlock=true;
		}
		else if (sOgolCmd=="shootpoint")
		{
			iCmd=OGOL_SHOOTPOINT;
		}
		else if (sOgolCmd=="color")
		{
			iCmd=OGOL_COLOR;
			string sColor=poDef->getNextIdentifier("color");
			iParam1=ogolPencil::stringToColor(sColor);
			if (iParam1==0)
			{
				poDef->throw_("Unknown color "+sColor);
			}
		}
		else if (sOgolCmd=="opacity")
		{
			iCmd=OGOL_OPACITY;
			sGetValue1="transparency 1..100";
		}
		else
		{
			poDef->throw_("Unknown ogol command ["+sOgolCmd+"]");
		}
		if (sGetValue1.length())
		{
			if (poDef->peekChar(true)=='$')
			{
				poDef->getNextChar(false);
				iParam1=(long)getVar(poDef->getNextIdentifier("varname ("+sGetValue1));
			}
			else
				iParam1=poDef->getNextLong(sGetValue1);
		}
		if (sGetValue2.length())
		{
			if (poDef->peekChar(true)=='$')
			{
				poDef->getNextChar(false);
				iParam2=(long)getVar(poDef->getNextIdentifier("varname ("+sGetValue2));
			}
			else
				iParam2=poDef->getNextLong(sGetValue2);
		}
		if (bAddOgolBlock)
		{
			ogol* poBlock=new ogol(this);
			iParam1=(long)poBlock;
			poBlock->readDef(poDef);
		}
		ogolCommand* poCmd=new ogolCommand(iCmd,iParam1,iParam2);
		mlstOgol.push_back(poCmd);
	}
}

void ogol::draw(SDL_Surface* img, coord* poCurrent, coord* poAngle, ogolPencil &oPencil) const
{
	if (oPencil.getMirror())
	{
		list<ogolCommand*>::const_reverse_iterator	oit=mlstOgol.rbegin();
		while(oit!=mlstOgol.rend())
		{
			ogolCommand*	pogol=(*oit++);
			pogol->draw(img,poCurrent,poAngle,oPencil);
		}
	}
	else
	{
		list<ogolCommand*>::const_iterator	oit=mlstOgol.begin();
		while(oit!=mlstOgol.end())
		{
			ogolCommand*	pogol=(*oit++);
			pogol->draw(img,poCurrent,poAngle,oPencil);
		}
	}
}

void ogolCommand::draw(SDL_Surface* img, coord* poCurrent, coord* poAngle, ogolPencil &oPencil) const
{
	coord oCurrent(*poCurrent);
	coord* poDistance=0;		// !=0 if we must move
	bool bLine=false;
//	bool bAngle=false;
	int iAngle=0;

	switch(miOgolCommand)
	{
	case OGOL_PU:
		oPencil.up();
		break;

	case OGOL_PD:
		oPencil.down();
		break;

	case OGOL_FW:
		poDistance=new coord(poAngle->x(),poAngle->y());
		poDistance->scalarMultiply(getValue(miParam1));
		bLine=true;
		break;

	case OGOL_BW:
		poDistance=new coord(poAngle->x(),poAngle->y());
		poDistance->scalarMultiply(-getValue(miParam1));
		bLine=true;
		break;

	case OGOL_FWU:
		poDistance=new coord(poAngle->x(),poAngle->y());
		poDistance->scalarMultiply(getValue(miParam1));
		break;

	case OGOL_BWU:
		poDistance=new coord(poAngle->x(),poAngle->y());
		poDistance->scalarMultiply(-getValue(miParam1));
		break;

	case OGOL_TR:
		iAngle=getValue(miParam1);
//		bAngle=true;
		break;

	case OGOL_TL:
		iAngle=-getValue(miParam1);
//		bAngle=true;
		break;

	case OGOL_ANGLE:
	{
		float fAngle=(float)getValue(miParam1)*2*PI/360;
		coord oAngle(fAngle);
		poAngle->setX(oAngle.x());
		poAngle->setY(oAngle.y());
		break;
	}

	case OGOL_CIRCLE:
		if(oPencil.isDown())
		{
			aacircleColor(img,oCurrent.x(),oCurrent.y(),getValue(miParam1)*oPencil.getScale()/2,oPencil.getColor());
		}
		break;

	case OGOL_COLOR:
		oPencil.setColor(miParam1);
		break;

	case OGOL_OPACITY:
		oPencil.setOpacity(getValue(miParam1));
		break;

	case OGOL_REPEAT:
		{
			ogol* poOgol=(ogol*)miParam1;
			long iRepeat=getValue(miParam2);
			while(iRepeat-->0)
				poOgol->draw(img,poCurrent,poAngle,oPencil);
			break;
		}

	case OGOL_ADD:
		*(float*)miParam1+=getValue(miParam2);
		break;
	case OGOL_SETVAR:
		*((float*)miParam1)=getValue(miParam2);
		break;

	case OGOL_ROTATES:
		{
			float f2=getValue(miParam2);
			if (f2)
			{
				float fAngle=-6.28318530718/f2;
				coord oAngle(fAngle);
				coord oNextAngle(*poAngle);
				ogol* poOgol=(ogol*)miParam1;

				int iRepeat=(int)(f2+0.5);
				while(iRepeat-->0)
				{
					oNextAngle.rotateBy(oAngle);
					oNextAngle.normalize();
					poAngle->setX(oNextAngle.x());
					poAngle->setY(oNextAngle.y());
					poOgol->draw(img,poCurrent,poAngle,oPencil);
				}
				break;
			}
		}

	case OGOL_MIRROR:
		{
			ogol* poOgol=(ogol*)miParam1;
			coord oFirstPos(*poCurrent);
			coord oFirstAngle(*poAngle);

			float fFirstAngle=poAngle->argument();

			poOgol->draw(img,poCurrent,poAngle,oPencil);
			// Compute the angle of the drawn picture
			// Add 90 to the angle to start drawing
			// the mirrored part.
			coord oLastPos(*poCurrent);
			oLastPos.substract(oFirstPos);
			float fCurAngle=oLastPos.argument();
			fCurAngle=poAngle->argument();
			float fAngle=(fCurAngle-fFirstAngle)*2+PI;
			//coord oAngle(fAngle-6.28318530718/4-fCurAngle);
			coord oAngle(fAngle);
			poAngle->multiplyBy(oAngle);
			//poAngle->setX(oAngle.x());
			//poAngle->setY(oAngle.y());
			//coord oPi(PI);

			oPencil.toggleMirror();
			poOgol->draw(img,poCurrent,poAngle,oPencil);
			oPencil.toggleMirror();
			break;
		}

	case OGOL_FILLED:
		{
			int iOpacity=oPencil.getOpacityFF();
			ogol* poOgol=(ogol*)miParam1;
			ogolPolygon* poly=oPencil.recordPolygon();
			poOgol->draw(img,poCurrent,poAngle,oPencil);
			oPencil.endRecord(poly,img, (miParam2 & 0xFFFFFF00) | iOpacity);
			break;
		}
	case OGOL_SHOOTPOINT:
		oPencil.setShootPoint(*poCurrent);
		break;
	}

	if (poDistance)
	{

		poDistance->scalarMultiply(oPencil.getScale());
		poDistance->add(*poCurrent);
	}
	if (oPencil.isDown())
	{
		if (bLine && poDistance)
		{
			if (oPencil.getExplosion())
			{
				coord oSpeed(*poCurrent);
				oSpeed.substract(oPencil.getShootPoint());
				oSpeed.normalize();
				oSpeed.multiplyBy(getRand(50,200)/20);
				int iRotationSpeed(getRand(0,180)-getRand(0,180));
				ogolVectoroid::create(*poCurrent,*poDistance,oSpeed,oPencil.getColor(),iRotationSpeed);
				//oPencil.addExplosion(p);
			}
			else if (!oPencil.isRecordingPoly())
			{
				aalineColor(img,poCurrent->x(),poCurrent->y(),poDistance->x(),poDistance->y(),oPencil.getColor());
			}
		}
	}

	if (oPencil.isRecordingPoly() && !oPencil.getExplosion())
	{
		oPencil.recordPoint(poCurrent, bLine);
	}
	if (poDistance)
	{
		poCurrent->setX(poDistance->x());
		poCurrent->setY(poDistance->y());
		delete poDistance;
	}

	if (iAngle)
	{
		float fAngle=(float)iAngle*6.28318530718/360;
		coord oAngle(fAngle);
		poAngle->multiplyBy(oAngle);
		poAngle->normalize();
	}
}
