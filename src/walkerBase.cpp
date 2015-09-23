/*
 * walkerBase.cpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#include <SDL/SDL_gfxPrimitives.h>
#include <cfileparser.hpp>

#include "vars.hpp"
#include "walkerBase.hpp"
#include "aStarLibrary.h"
#include "sound.hpp"
#include "ogol.hpp"
#include "game.hpp"
#include "walkers.hpp"
#include "ogolVectoroid.hpp"

extern int getRand(int iMin,int iMax);	// FIXME Beuuuark

coord coordToPixels(coord o)
{
	o.add(coord(0.5,0.5));
	o.scalarMultiply(gpGame->getTileSizeX());	// FIXME

	o+=gpGame->getMapArea().getTopLeft();
	return o;
}

walkerBase::walkerBase()
	:
	mfHealth(0),
	mfSpeed(0),
	miRefCount(0),
	mbVisible(true),
	moAngle(0,1),
	mfRotationSpeed(DEFAULT_ROTATION_SPEED),
	mfScale(1),
	mbFollowDir(false),
	miExplosion(0),
	mlBank(0),
	mlLifeTime(0),
	mlReleaseSlow(0),
	mbAutoRotateSpin(true),
	msShortDesc("short_desc missing"),
	miMaxCount(9999),
	mbBoss(false),
	msMusic(""),
	miKills(1)
{
}

// BUG moPencil n'est pas dupliqu�
// (contient le shoot point partag� par des walkers)
walkerBase::walkerBase(const walkerBase* p)
	:
		mapelt(*p),	// FIXME BAD WRITTEN
		mfHealth		(p->mfHealth),
		mfInitialHealth	(p->mfInitialHealth),
		mDest			(p->mDest),
		mfSpeed			(p->mfSpeed),
		miRefCount		(0),
		mbVisible		(p->mbVisible),
		miPathId		(0),
		mDirection		(p->mDirection),
		moAngle			(p->moAngle),
		mfRotationSpeed	(p->mfRotationSpeed),
		mpoSoundDie		(p->mpoSoundDie),
		mogol			(p->mogol),
		moPencil		(p->moPencil),
		mfScale			(p->mfScale),
		mbFollowDir		(p->mbFollowDir),
		miExplosion		(p->miExplosion),
		mlBank			(p->mlBank),
		mlLifeTime		(0),
		mfInitialSpeed	(p->mfSpeed),
		mlReleaseSlow	(p->mlReleaseSlow),
		msShortDesc		(p->msShortDesc),
		miMaxCount		(p->miMaxCount),
		mbBoss			(p->mbBoss),
		msMusic			(p->msMusic),
		miKills			(p->miKills)
{
	moShootPoint=getCoord();
}

walkerBase::walkerBase(
	float fHealth,
	coord pos,
	float speed,
	coord dest,
	float fRotationSpeed)
	:
	mapelt(pos),
	mfHealth(fHealth),
	mfInitialHealth(fHealth),
	mfSpeed(0),
	miRefCount(0),
	mbVisible(true),
	miPathId(0),
	moAngle(0,1),
	mfRotationSpeed(fRotationSpeed),
	mpoSoundDie(0),
	mfScale(1),
	mbFollowDir(false),
	mlBank(0),
	mlLifeTime(0),
	moShootPoint(pos),
	msShortDesc("short_desc missing"),
	miMaxCount(9999),
	mbBoss(false),
	msMusic(""),
	miKills(1)
{
	setDirection(coord(1,0));
	setDestination(dest,"walkerBase-1");
}

void walkerBase::resetClosedList()
{
	::resetClosedList();
}

void walkerBase::readOneFromDef(CFileParser* poDef)
{
	bool bDisabled(false);
	walkerBase* poWalker=new walkerBase;
	bool bReadWalker=true;
	string sSoundDie="explode_1.wav";
	string sName=poDef->getNextIdentifier("name of the walker");

	poWalker->msShortDesc=sName+" has no short desc";
	poDef->getExpectedChar("{");

	while(poDef->good() && bReadWalker)
	{
		if (poDef->peekChar()=='}')
		{
			bReadWalker=false;
			poDef->getExpectedChar("}");

			if (poWalker->mfHealth<=0)
			{
				poDef->throw_("missing or bad health");
			}
			else if (poWalker->mfSpeed<=0)
			{
				poDef->throw_("missing speed");
			}
		}
		else
		{
			string sKeyword=poDef->getNextIdentifier("walker data");
			if (sKeyword=="disabled")
			{
				bDisabled=true;
			}
			else if (sKeyword=="boss_music")
			{
				if (poWalker->isBoss())
				{
					poWalker->msMusic=poDef->getNextString("Music file");
					// FIXME, check if the music exists
				}
				else
					poDef->throw_("walkerBase","Only bosses can change the music");
			}
			else if (sKeyword=="boss")
			{
				poWalker->mbBoss=true;
			}
			else if (sKeyword=="die_sound")
			{
				sSoundDie=poDef->getNextString("Sound file");
			}
			else if (sKeyword=="boss_kills")
			{
				poWalker->miKills=poDef->getNextLong("kills value");
			}
			else if (sKeyword=="max_count")
			{
				poWalker->miMaxCount=poDef->getNextLong("max count value");
			}
			else if (sKeyword=="health")
			{
				poWalker->mfHealth=poDef->getNextLong("health");
				poWalker->mfInitialHealth=poWalker->mfHealth;
			}
			else if (sKeyword=="fixed_spin")
			{
				poWalker->mbAutoRotateSpin=false;
			}
			else if (sKeyword=="short_desc")
			{
				poWalker->msShortDesc=poDef->getNextString("short description");
			}
			else if (sKeyword=="speed")
			{
				poWalker->mfSpeed=poDef->getNextLong("speed");
			}
			else if (sKeyword=="ogol")
			{
				poWalker->mogol.readDef(poDef);
			}
			else if (sKeyword=="rotation_speed")
			{
				poWalker->mfRotationSpeed=(float)poDef->getNextLong("angle")*6.28318530718/360;
			}
			else if (sKeyword=="follow_direction")
			{
				poWalker->mbFollowDir=true;
			}
			else if (sKeyword=="scale")
			{
				poWalker->mfScale=poDef->getNextLong("scale")/100.0;
			}
			else if (sKeyword=="bank")
			{
				poWalker->mlBank=poDef->getNextLong("bank value");
			}
			else
				poDef->throw_("walker","Unknown walker item ["+sKeyword+"]");
		}
	}

	if (poWalker->isBoss() && poWalker->miMaxCount>20)
	{
		poDef->throw_("walkerBase","A boss must have max_count less than 20");
	}
	if (poWalker)
		poWalker->mpoSoundDie=new Sound(sSoundDie);

	if (poWalker && !bDisabled)
		walkers::registerWalker(sName,poWalker);
}

void walkerBase::setDestination(coord dest,const string sFrom)
{
	mDest=dest;
	computePath("setDest from "+sFrom);
}

float walkerBase::getSpeed() const
{
	return mfSpeed*gpGame->getOgolScale();
}

bool walkerBase::computePath(const string sFrom)
{
	static bool bInitPath=true;

	if (bInitPath)
	{
		bInitPath=false;
	}
	if (miPathId==0)
	{
		miPathId=allocatePathId();
	}
	else
	{
		resetPath(miPathId);
	}
	pathStatus[miPathId]=FindPath(miPathId,
			getCoord().x()-mapTL.x(),
			getCoord().y()-mapTL.y(),
			mDest.x()*tileSizeX,mDest.y()*tileSizeY);
	if (pathStatus[miPathId]!=found)
	{
		cerr << sFrom << ": Path " << miPathId << " from " << getCoord() << " to " << mDest << " not found " << endl;
	}
	return pathStatus[miPathId]==found;
}
	
walkerBase::~walkerBase()
{
	if (miRefCount>0)
	{
		cerr << "ERROR : Attempting to delete a linked walker !!!!" << endl;
	}
	freePathId(miPathId);
	if (mbBoss)
		gpGame->bossDie();
}

void walkerBase::drawNative(const coord &pos, SDL_Surface* dest,const coord& angle, float fScale) const
{
	coord oPos(pos);
	coord oAngle(angle);
	moPencil.down();
	moPencil.setScale(mfScale*fScale);
	moPencil.setOpacityFactor(100);
	mogol.draw(dest,&oPos,&oAngle,moPencil);
}

void walkerBase::drawAt(const coord &p, SDL_Surface *dest) const
{
	// moPencil.drawExplosion(back);
	if ((mbVisible) && miExplosion<=0)
	{
		float fScale;
		const float scale_min=0.5;
		if (mfHealth>0)
		{
			fScale=scale_min+(mfHealth*(1.0-scale_min))/mfInitialHealth;
		}
		else
			fScale=scale_min;


		moPencil.down();
		moPencil.setScale(mfScale*fScale);
		coord oAngle(moAngle);
		coord cray(p);
		if (mlLifeTime>1000)
			moPencil.setOpacityFactor(100);
		else
			moPencil.setOpacityFactor(mlLifeTime/10);
		moPencil.setShootPoint(p);
		if (miExplosion==-1)
		{
			// Used the 1st time, only once to compute explosion particles (vectoroids)
			moPencil.setExplosion(true);
			miExplosion=giOgolVectoroidTimeMs;	// drawing explosion
		}
		mogol.draw(dest,&cray,&oAngle,moPencil);
		moShootPoint=moPencil.getShootPoint();
		moPencil.setExplosion(false);
	}
}

bool walkerBase::damageSpeed(float fSlowPercent, float fMinPercent)
{
	bool bRet=false;
	mlReleaseSlow = 5000;	// 5 sec to re-accelerate
	if (mfInitialSpeed>0)
	{
		float fLowestSpeed=mfInitialSpeed*fMinPercent/100.0;
		mfSpeed=mfSpeed*(100.0-fSlowPercent)/100.0;
		if (mfSpeed<fLowestSpeed)
		{
			if (fSlowPercent>0)
				mfSpeed=fLowestSpeed;
			bRet=true;
		}
	}
	else
		bRet=true;
	return bRet;
}

void walkerBase::damage(float fHealth)
{
	if (mfHealth>0)
		mfHealth-=fHealth;
	if (mfHealth<=0)
	{
		mfHealth=0;
		miExplosion=-1;
		if (mlBank)
			gpGame->mlBank+=mlBank;
		else
			gpGame->mlBank+=mfInitialHealth;
		if (mpoSoundDie)
			mpoSoundDie->play();
		gpGame->bossDieEndMusic();
	}

	coord oVertical(mDirection);
	coord oRotate(0,1);
	oVertical.multiplyBy(oRotate);
	oVertical.normalize();
	oVertical.scalarMultiply(2);
	coord oV1(getShootPoint());
	oV1.add(oVertical);
	coord oSpeed(mDirection);
	coord oAngle(getRand(-20,20));
	oSpeed.multiplyBy(oAngle);
	oSpeed.normalize();

	ogolVectoroid::create(getShootPoint(),oV1,oSpeed,0xFF000080,getRand(-720,720));
	// moPencil.addExplosion(p);
}

int walkerBase::update(int iTimerEllapsedms)
{
	if (mfSpeed<mfInitialSpeed)
	{
		mlReleaseSlow-=iTimerEllapsedms;
		if (mlReleaseSlow<0)
		{
			mlReleaseSlow=0;
			mfSpeed=mfSpeed+10*mfInitialSpeed*(float)iTimerEllapsedms/100.0;
			if (mfSpeed>mfInitialSpeed)
				mfSpeed=mfInitialSpeed;
		}
	}
	// moPencil.update(iTimerEllapsedms);
	mlLifeTime+=iTimerEllapsedms;
	if (miExplosion>0)
	{
		miExplosion-=iTimerEllapsedms;
		if (miExplosion<=0)
		{
			mbVisible=false;
		}
	}

	if (mbVisible)
	{
		coord pos(mDirection);
		pos.scalarMultiply(iTimerEllapsedms*mfSpeed/1000);
		if (mfHealth>0)
		{
			mpos+=pos;
		}
		if (pathStatus[miPathId]==found)
		{
			ReadPath(miPathId,mpos.x()-mapTL.x(),mpos.y()-mapTL.y(),10);

			coord dest(xPath[miPathId],yPath[miPathId]);
			dest+=mapTL;
			dest-=mpos;
			setDirection(dest);
		}
		if (iTimerEllapsedms)
		{

			if (mbFollowDir)
			{
				float fFilter=1-0.3*(float)iTimerEllapsedms/100.0;
				moAngle.setX(moAngle.x()*fFilter+(mDirection.x()*(1.0-fFilter)));
				moAngle.setY(moAngle.y()*fFilter+(mDirection.y()*(1.0-fFilter)));
			}
			else
			{
				if (mbAutoRotateSpin)
				{
					if (pos.x()>0)
					{
						mfSpin+=(float)iTimerEllapsedms/250.0;
						if (mfSpin>1)
							mfSpin=1;
					}
					else if (pos.x()<0)
					{
						mfSpin-=(float)iTimerEllapsedms/250.0;
						if (mfSpin<-1)
							mfSpin=-1;
					}
					else
						mfSpin=0;
				}

				float fAngle=mfSpin*mfRotationSpeed*(float)iTimerEllapsedms/1000.0*mfSpeed/mfInitialSpeed;
				coord oRotate(fAngle);
				moAngle.rotateBy(fAngle);
			}
			moAngle.normalize();
		}
		if ((getShootPoint().distanceTo(getDestinationPixels())<10))	// FIXME hard coded
		{
			if (gpGame->state()==STATE_PLAY)
			{
				gpGame->burnLives(miKills);
				miExplosion=-1;
				miKills=0;
			}
			// mbVisible=false;
		}
	}
	return false;
}

void walkerBase::setDirection(coord direction)
{
	direction.normalize();
	mDirection=direction;
}

