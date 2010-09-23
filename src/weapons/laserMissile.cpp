/*
 * laserMissile.cpp
 *
 *  Created on: 8 ao�t 2010
 *      Author: saturn
 */

#include "laserMissile.hpp"
#include "../towerBase.hpp"

extern int getRand(int,int);	// BEUARK

laserMissile::laserMissile()
:
	missileBase()
{
	miMaxTimems=999999;
	mfSpeed=1;	// Fake speed
}

laserMissile::~laserMissile()
{

}
missileBase* laserMissile::clone()
{
	miMaxTimems=999999;
	laserMissile* p=new laserMissile();
	fillBaseClone(p);

	return p;
}


void laserMissile::drawAt(const coord &p, SDL_Surface *dest) const
{
	if (mpTarget)
	{
		coord x1(p);
		coord x2(mpTarget->getShootPoint());
		x2+=coord(getRand(-2,2),getRand(-2,2));

		aalineColor(dest,x1.x(),x1.y(),x2.x(),x2.y(),miColor);
	}
	else
		cerr << "laserMissile has no target !!!" << endl;
}
const string laserMissile::getDamageInfo(const towerBase* pFromTower) const
{
	return "laser, "+to_string(pFromTower->getDamage())+"hp/sec.";
}

int laserMissile::update(int iTimeEllapsedms)
{
	bool bRet=missileBase::update(iTimeEllapsedms);
	if (!bRet)
	{
		float iDamage=(float)miDamage*(float)iTimeEllapsedms/1000.0;
		mpTarget->damage(iDamage);
		if (
				(getCoord().distanceTo(mpTarget->getShootPoint())>mpoTower->getRange()) ||
				(getTarget()->isDead())
			)

			bRet=true;
	}
	return bRet;
}
