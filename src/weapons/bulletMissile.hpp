/*
 * bulletMissile.hpp
 *
 *  Created on: 8 ao�t 2010
 *      Author: saturn
 */
 
#ifndef BULLETMISSILE_HPP_
#define BULLETMISSILE_HPP_

#include "missileBase.hpp"

class bulletMissile : public missileBase
{
public:
	bulletMissile();
	virtual ~bulletMissile();

	virtual missileBase* clone();
	virtual void drawAt(const coord &p, SDL_Surface *) const;
	virtual int update(int iTimeEllapsedms);
	virtual void readFromDef(CFileParser*);
	virtual const string getDamageInfo(const towerBase* pFromTower) const;

	virtual bool isTargetable(walkerBase* pTarget) { return true; }

	float getSpeed() const;
protected:
	virtual bool readSpecificDefItem(const string &sItem, CFileParser*);

	coord 		oDirection;
	mutable ogolPencil	moPencil;
	bool		mbEnhancedSystem;	// Better accuracy for computing direction in order to reach target
	float		mfSpinFactor;		// rotation speed (0.02 is good, 20 for spin_factor value)
};

#endif /* BULLETMISSILE_HPP_ */
