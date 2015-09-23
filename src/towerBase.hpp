/*
 * towerBase.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef TOWERBASE_HPP_
#define TOWERBASE_HPP_

#include <SDL/SDL_gfxPrimitives.h>
#include <list>
#include <map>

#include "mapelt.hpp"
#include "vars.hpp"
#include "coord.hpp"

using namespace std;

class CFileParser;
class walkerBase;
class missileFactory;
class missileBase;

extern int gaiRanges[16]; //={70,66,77,88,99,100,110,120,130,140,150,160,170,180,190,200};

class towerBase : public mapelt
{

	static map<int,towerBase*>	mapTowers;

	public:

		static towerBase*	buildTowerAs(coord p);
		virtual ~towerBase() {}
		/**
		 * @return range of the tower (pixels)
		 */
		float getRange() const;
		void	setRange(int iRange){miRange=iRange;}

		const string getDamageInfo() const;

		int getSpeed() const;

		virtual void drawAt(const coord& pos, SDL_Surface*) const;

		void	drawMissiles(SDL_Surface* dest);

		void	setName(const string sName){msName=sName;}
		string	getName() const { return msName; }

		void	setCostMoney(int iCost){miCostMoney=iCost;}
		int		getCostMoney() const { return miCostMoney; }

		int		getUpgradeCost() const { return 40; }

		void	setDamage(int iDamage){miDamage=iDamage;}
		int		getDamage() const { return miDamage; }

		void	setDamageIncrease(int iDamageIncrease){miDamageIncrease=iDamageIncrease;}
		int		getDamageIncrease() const { return miDamageIncrease; }

		void	setRangeIncrease(int iRangeIncrease){miRangeIncrease=iRangeIncrease;}
		int		getRangeIncrease() const { return miRangeIncrease; }

		void			setText(const string sText){msText=sText;}
		const string	getText() const { return msText; }

		const string infos()
		{
			cout << miMaxMissiles << " " << miMaxMissilePerTarget << endl;
			return "";
		}
		coord	getCenterCoord() const { return mCenterCoords; }

		// from mapelt
		virtual int update(int iTimeEllapsedms);
		virtual void setCoord(const coord pos);

		// Missile methods
		void	setMissileFactory(missileFactory*p) { mpMissileFactory=p; }

		virtual int countMissilesOnTarget(walkerBase*);

		virtual coord getMissileStartPoint() { return mCenterCoords; }
		// Level methods
		int		getLevel() const { return miLevel; }
		void	nextLevel()
		{
			miRange+=miRange*1000/miRangeIncrease;
			miDamage+=miDamage*1000/miDamageIncrease;
			miLevel++;
		}
		bool canFire() const { return miCoolDown<=0; }

		void	setViewRange(bool bWithRange)	{	mbWithRange=bWithRange;}

		/**
		 * If pMissile, then this method tries to find a valuable
		 * target for the missile. Else the method create missiles
		 * for valuables targets.
		 * The method returns true if it has found a target for the missile.
		 * false instead.
		 * When pMissile = 0, returns is undefined.
		 */
		virtual bool checkTargets(missileBase* pMissile=0);

		static towerBase* build(int,coord);

		/**
		 * Read a tower and register it from a def file.
		 * @param poDef a file parser starting by the tower's name
		 *        (keyword 'tower' has been parsed before).
		 * once the tower read, the file parser position is after the }
		 */
		static void readOneTower(CFileParser* poDef);

		bool isBuildable() const;
		void spendCosts() const;

		float getSellValue() const { return getCostMoney()*0.8; }

	protected:
		coord	mCenterCoords;	// Coordinate of the center of this
		//coord	imageCoords;
		int		miNumber;		// Num�ro de la tour 0..15
		int		miLevel;
		string	sName;
		int		iCostMoney;
		int		iDamage;
		int		iDamageIncrease;
		int		iRange;
		int		iRangeIncrease;
		string	msName;
		int		miCostMoney;
		int		miDamage;
		int		miDamageIncrease;
		int		miRange;
		int		miRangeIncrease;
		string	msText;
		bool	mbWithRange;

		int		miMaxMissiles;
		int		miMaxMissilePerTarget;
		missileFactory	*mpMissileFactory;
		list<missileBase*>	mlstMissiles;

		int		miCoolDown;			// >0 cannot fire
		int		miCoolDownTime;		// Time ms between two fires
		string	msComments;

	private:
		towerBase(int iNumber, coord pos);
		//towerBase(const towerBase*);

};

#endif /* TOWERBASE_HPP_ */
