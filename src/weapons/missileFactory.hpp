/*
 * missileFactory.hpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#ifndef MISSILEFACTORY_HPP_
#define MISSILEFACTORY_HPP_
#include <map>
#include <string>

using namespace std;

class missileBase;
class towerBase;
class walkerBase;

// A missile factory is a weapon that
// can equip a tower.
class missileFactory
{
	static map<const string,missileFactory*> lstFactories;
public:

	missileBase* build(towerBase*,walkerBase*);

	static void registerFactory(const string sName, missileBase*);

	static missileFactory* getFactory(const string sName);

	const string getDamageInfo(const towerBase* pFromTower) const;

	/**
	 * Yet, this method manages the cooldown time
	 * of the weapon.
	 */
	int update(int iTimeEllapsedms);

	/**
	 * When a tower tries to attack a target, it should check first if
	 * the target is ok for the weapons.
	 * The factory uses its single instance to check if the target is valuable.
	 * @param walkerBase* pTarget the target to attack
	 * @return bool True if the target is ok.
	 */
	bool isTargetable(walkerBase *pTarget);

private:
	missileFactory(missileBase* pBase);

	missileBase* pMissile;

};
#endif /* MISSILEFACTORY_HPP_ */
