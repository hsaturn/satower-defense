/*
 * coldMissile.cpp
 *
 *  Created on: 12 august 2010
 *      Author: saturn
 */

#include "coldLaser.hpp"
extern int getRand(int,int);	// BEUARK

coldLaser::coldLaser()
:
	missileBase()
{
	mfSpeed=1;	// fake speed
	miChangeLaser=0;
	mfMaxSlow=50;
	miMaxTimems=999999999;
}

coldLaser::~coldLaser()
{

}

void coldLaser::drawAt(const coord &p, SDL_Surface* dest) const
{
	coord	pDest(mpTarget->getShootPoint());
	pDest.substract(mpos);
	float fLength=pDest.norm();
	if (fLength==0)
		return;	// 0 length is not computable.
	if (miChangeLaser<0)
	{
		miChangeLaser=1;	// FIXME parameter
		mfAlpha=pDest.module();
		mfAlpha0=getRand(20,60)*PI/180;
		mfAlpha1=getRand(60,150)*PI/180;
		mfRatioLength0=getRand(40,60)/100.0;
		mfRatioLength1=getRand(40,60)/100.0;
		// Do until last segment is at least 10% of the distance to the target
		// (else artifacts effect occurs)
		if (getRand(-10,10)>0)
			mbLrl=true;
	}

	float fAngleL0=mfAlpha;
	float fAngleL1=mfAlpha;
	if (mbLrl)
	{
		fAngleL0-=mfAlpha0;
		fAngleL1+=mfAlpha1;
	}
	else
	{
		fAngleL0+=mfAlpha0;
		fAngleL1-=mfAlpha1;
	}
	coord I1(mpos);
	coord L0(fAngleL0);
	coord L1(fAngleL1);
	L0.scalarMultiply(fLength*mfRatioLength0);
	I1.add(L0);

	L1.scalarMultiply(fLength*mfRatioLength1);
	coord I2(I1);
	I2.add(L1);

	coord I3(mpTarget->getShootPoint());
	aalineColor(dest,mpos.x(),mpos.y(),I1.x(),I1.y(),0xFFFFFFFF);
	aalineColor(dest,I1.x(),I1.y(),I2.x(),I2.y(),0xFFFFFF80);
	aalineColor(dest,I2.x(),I2.y(),I3.x(),I3.y(),0xFFFFFF40);
}

int coldLaser::update(int iTimeEllapsedms)
{
	bool bRet=missileBase::update(iTimeEllapsedms);
	if (!bRet)
	{
		miChangeLaser-=iTimeEllapsedms;
		// FIXME miMaxSlow to be implemented
		float fSlow=(float)100*(float)iTimeEllapsedms/1000.0;

		bRet=mpTarget->damageSpeed(fSlow,mfMaxSlow);
	}
	return bRet;
}

const string coldLaser::getDamageInfo(const towerBase* pFromTower) const
{
	return "Freeze target\nSlow up to "+to_string((int)(mfMaxSlow))+" %";
}

missileBase* coldLaser::clone()
{
	coldLaser* p=new coldLaser();

	fillBaseClone(p);

	return p;
}

bool coldLaser::isTargetable(walkerBase* pTarget)
{
	// The target is considered too slow and not attackable
	// if its speed is too low.
	if (pTarget->damageSpeed(0,mfMaxSlow*1.1))
		return false;
	else
		return true;
}

