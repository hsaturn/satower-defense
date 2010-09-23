/*
 * laserMissile.hpp
 *
 *  Created on: 8 august 2010
 *      Author: saturn
 */

#ifndef LASERMISSILE_HPP_
#define LASERMISSILE_HPP_

#include "missileBase.hpp"

class laserMissile : public missileBase
{
public:

	laserMissile();
	virtual ~laserMissile();

	virtual void drawAt(const coord &p, SDL_Surface *) const;
	virtual int update(int iTimeEllapsedms);
	virtual missileBase* clone();
	virtual const string getDamageInfo(const towerBase* pFromTower) const;
	virtual bool isTargetable(walkerBase* pTarget)  { return true; }
	void readFromDef(CFileParser*){};
};

#endif /* LASERMISSILE_HPP_ */
