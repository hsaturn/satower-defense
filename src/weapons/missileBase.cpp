/*
 * missileBase.cpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#include "missileBase.hpp"

#include "../towerBase.hpp"
#include "../walkerBase.hpp"
#include "../sound.hpp"

// FIXME BAD DESIGN, The base class has to know each of its descendants
#include "laserMissile.hpp"
#include "coldLaser.hpp"
#include "bulletMissile.hpp"

#include "missileFactory.hpp"

extern int getRand(int iMin,int iMax);	// FIXME

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

missileBase::missileBase()
	:
	mapelt			(coord(0,0)),
	mpTarget		(0),
	mfSpeed			(0),
	miColor			(0),
	miDamage		(0),
	mbJobFinished	(false),
	mpogol			(0),
	mpoSound		(0)
{
	setAlpha	(0x80);
};

void missileBase::fillBaseClone(missileBase* pClone)
{
	pClone->mpos=mpos;
	pClone->mfSpeed=mfSpeed;
	pClone->mpTarget=mpTarget;
	pClone->miColor=miColor;
	pClone->miDamage=miDamage;
	pClone->mpogol=mpogol;
	pClone->mpoSound=mpoSound;
	pClone->miMaxTimems=miMaxTimems;
}

void missileBase::sound()
{
	if (mpoSound)
		mpoSound->play();
}
void missileBase::setColor(int iColor)
{
	miColor=(miColor&0xff) | (iColor<<8);
}

void missileBase::setAlpha(int iAlpha)
{
	miColor=(miColor&0xffffff00) | (iAlpha&0xff);
}

missileBase::~missileBase()
{
	// if (mogol) delete mogol;	// NOT TO BE DONE BECAUSE OF CLONED OBJECT
	setTarget(0);
}

void missileBase::setTarget(walkerBase* pTarget)
{
	if (mpTarget)	mpTarget->decRef();
	mpTarget=pTarget;
	mbJobFinished=false;	// FIXME should be a method resetState if any other things would also change
	if (mpTarget) mpTarget->incRef();
}

// Does not work yet
void missileBase::readFromDef(CFileParser* poDef)
{

	while(poDef->good())
	{
		poDef->checkExpectedString("weapon");
		readOneFromDef(poDef);
	}
}

int missileBase::update(int iTimeEllapsedms)
{
	bool bRet=false;
	if (miMaxTimems)
	{
		if (miMaxTimems<=iTimeEllapsedms)
			bRet=true;
		else
			miMaxTimems-=iTimeEllapsedms;
	}
	return bRet;
}

void missileBase::readOneFromDef(CFileParser* poDef)
{
	missileBase*	pMissile=0;
	bool bWeapon=true;
	string sWeaponName=poDef->getNextIdentifier("weapon name");
	poDef->getExpectedChar("{");

	// FIXME detect doublons

	try
	{
		while(bWeapon)
		{
			if (poDef->peekChar()=='}')	// Check the end of weapon definition
			{
				if (pMissile)
				{
					if (pMissile->mfSpeed==0)
						poDef->throw_("Speed must be not null");
				}
				poDef->getExpectedChar("}");
				break;
			}
			poDef->trim();
			string sItem=poDef->getNextIdentifier("weapon item");
			if (pMissile==0)
			{
				if (sItem!="type")
					poDef->throw_("missileBase","type expected, ["+sItem+"] got instead !");

				string sType=poDef->getNextIdentifier("type of weapon");
				if (sType=="laser")
				{
					pMissile=new laserMissile();
				}
				else if (sType=="missile")
				{
					pMissile=new bulletMissile();
				}
				else if (sType=="cold_laser")
				{
					pMissile=new coldLaser();
				}
				else
					poDef->throw_("Unknown weapon type ["+sType+"]");
			}
			else
			{
				if (sItem=="comments")
				{
					pMissile->msComments=poDef->getNextString("comments");
				}
				else if (sItem=="damage")
				{
					static bool bMsg=true;
					if (bMsg)
					{
						bMsg=false;
						cout << "damage NYI" << endl;
					}
					poDef->getNextIdentifier("damage type");
				}
				else if (sItem=="sound")
				{
					pMissile->mpoSound=new Sound(poDef->getNextWord());
				}
				else if (sItem=="speed")
				{
					pMissile->mfSpeed=poDef->getNextLong("speed value");
				}
				else if (sItem=="color")
				{
					string sColor=poDef->getNextIdentifier("color");
					pMissile->miColor=ogolPencil::stringToColor(sColor);
					if (pMissile->miColor==0)
						poDef->throw_("missileBase","Bad color (" +sColor+")");
				}
				else if (sItem=="max_time")
				{
					pMissile->miMaxTimems=poDef->getNextLong("Max time to live (ms)");
				}
				else if (sItem=="ogol")
				{
					if (pMissile->mpogol)
					{
						poDef->throw_("missileBase","ogol defined twice");
					}
					pMissile->mpogol=new ogol();
					pMissile->mpogol->readDef(poDef);
				}
				else if (sItem=="type")
				{
					poDef->throw_("type defined twice");
				}
				else if (!pMissile->readSpecificDefItem(sItem,poDef))
					poDef->throw_("missileBase","Unknown weapon item : '"+sItem+"'");
			}
		}

		if (pMissile)
			missileFactory::registerFactory(sWeaponName,pMissile);
		else
			poDef->throw_("missileBase","Bad missile definition");
	}
	catch(CSException *p)
	{
		throw new CSException("missileBase","In weapon '"+sWeaponName+"': "+p->getCompleteError());
	}
}

