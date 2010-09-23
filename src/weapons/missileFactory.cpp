/*
 * missileFactory.cpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */

#include "missileFactory.hpp"
#include "missileBase.hpp"
#include <csexception.hpp>
#include "../towerBase.hpp"
map<const string,missileFactory*> missileFactory::lstFactories;

missileBase* missileFactory::build(towerBase* pTower, walkerBase* pTarget)
{
	missileBase* p=pMissile->clone();
	p->sound();
	p->setCoord(pTower->getCenterCoord());
	p->setTower(pTower);
	p->setTarget(pTarget);
	p->setDamage(pTower->getDamage());
	return p;
}

void missileFactory::registerFactory(const string sName, missileBase* pMissile)
{
	missileFactory* p=new missileFactory(pMissile);

	lstFactories[sName]=p;
}
missileFactory::missileFactory(missileBase* pBase)
{
	pMissile=pBase;
}

const string missileFactory::getDamageInfo(const towerBase* pFromTower) const
{
	if (pMissile)
	{
		return pMissile->getDamageInfo(pFromTower);
	}
	else
		return "No weapon !";
}

missileFactory* missileFactory::getFactory(const string sName)
{
	map<const string,missileFactory*>::iterator oit=lstFactories.find(sName);

	if (oit!=lstFactories.end())
	{
		// cout << "factory : " << sName << " found !" << endl;
		return oit->second;
	}
	else
		throw new CSException("missileFactory","Unable to find factory <"+sName+">");
}

bool missileFactory::isTargetable(walkerBase* pTarget)
{
	return pMissile->isTargetable(pTarget);
}
