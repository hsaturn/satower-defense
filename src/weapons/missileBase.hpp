/*
 * missileBase.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef MISSILEBASE_HPP_
#define MISSILEBASE_HPP_

#include "../mapelt.hpp"
#include "ogol.hpp"
#include "../walkerBase.hpp"

class towerBase;
class ogol;
class Sound;

class missileBase : public mapelt
{
public:

	missileBase();
	//missileBase(const coord& pos, float fSpeed, walkerBase* pTarget, Uint32 iColor);
	virtual ~missileBase();

	virtual void drawAt(const coord &p, SDL_Surface*) const=0;

	/**
	 * The default method manages the cooldown time
	 * @return true if the weapon cannot do more damage.
	 *
	 * Derived classes should call missileBase::update and check
	 * if result is true. If true, no more action is needed else than
	 * returning the same value.
	 */
	virtual int update(int iTimeEllapsedms);
	virtual missileBase* clone()=0;
	void fillBaseClone(missileBase* pClone);
	// Once the missile has reach its target,
	bool jobFinished() { return mbJobFinished; };

	void setTarget(walkerBase* pTarget);
	void setColor(int iColor);
	void setAlpha(int iAlpha);
	walkerBase*	getTarget() const { return mpTarget; }
	void setDamage(int iDamage)  { miDamage=iDamage; }

	virtual const string getDamageInfo(const towerBase* pFromTower) const { return string("no infos"); };

	// Read a file containing only weapons
	static void readFromDef(CFileParser*);

	// Read one weapon from a def file
	static void readOneFromDef(CFileParser*);
	void setTower(towerBase* pTower) { mpoTower=pTower; }

	/**
	 * @see missileFactory::isTargetable
	 */
	virtual bool isTargetable(walkerBase*)=0;

	void sound();

protected:
	missileBase(const missileBase*);
	missileBase& operator=(const missileBase&);

	/**
	 * Called by the base class in order to
	 * extend the way a derived class reads its def file
	 * @return false if bad or unknown item
	 */
	virtual bool readSpecificDefItem(const string &sItem, CFileParser*) { return false; };

	walkerBase*	mpTarget;
	float		mfSpeed;	// Pixels per sec
	Uint32		miColor;
	int			miDamage;	// FIXME should be a class (cold damage , hp etc)
	coord		mDirection;
	bool		mbJobFinished;
	towerBase*	mpoTower;
	ogol*		mpogol;
	Sound*		mpoSound;
	int			miMaxTimems;	// Max time to live
	string		msComments;
};

#endif /* MISSILEBASE_HPP_ */
