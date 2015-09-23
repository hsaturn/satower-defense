/*
 * towerBase.cpp
 *
 *  Created on: 17 juil. 2010
 *      Author: saturn
 */
#include <cfileparser.hpp>

#include "towerBase.hpp"
#include "walkerBase.hpp"
#include "missileFactory.hpp"
#include "missileBase.hpp"
#include "game.hpp"

extern	list<walkerBase*>	lstWalkers;

int gaiRanges[16]={70,66,77,88,99,100,110,120,130,140,150,160,170,180,190,200};

map<int,towerBase*>	towerBase::mapTowers;


towerBase::towerBase(int iNumber, coord pos)
	:
	miNumber(iNumber),
	miLevel(1),
	miMaxMissiles(1),
	miMaxMissilePerTarget(1),
	mpMissileFactory(0),
	miCoolDown		(0),
	miCoolDownTime	(0)
{
	miRange=gaiRanges[iNumber];
	//imageCoords.setX(towers_x+(iNumber%4)*39);
	//imageCoords.setY(towers_y+(iNumber/4)*36);
	setCoord(pos);
}

void towerBase::setCoord(coord pos)
{
	mapelt::setCoord(pos);
	mCenterCoords=coord(pos.x()+tileSizeX/2,pos.y()+tileSizeY/2);
}

float towerBase::getRange() const
{
	return (float)miRange*gpGame->getOgolScale();
}

towerBase* towerBase::buildTowerAs(coord pos)
{
	for(auto oit: mapTowers)
	{
		if (oit.second->getCenterCoord().xyMaxDistanceTo(pos)<=12.5)
			return new towerBase(*(oit.second));
	}

	return 0;
}


int towerBase::update(int iTimeEllapsedms)
{
	bool bDelete;
	mapelt::update(iTimeEllapsedms);
	if (miCoolDown>0)
		miCoolDown-=iTimeEllapsedms;

	list<missileBase*>::iterator oit=mlstMissiles.begin();
	while (oit!=mlstMissiles.end())
	{
		bDelete=false;
		missileBase* pMissile=*oit;

		if (pMissile->getTarget()->isDead() | (pMissile->getTarget()->getVisible()==false))
		{
			//cout << endl;
			//cout << "target is dead" << endl;
			// Try to find another target
			// or delete it
			if (checkTargets(pMissile)==false)
			{
				bDelete=true;
				//cout << "tower no target for " << pMissile << endl;
			}
		}
		else if (pMissile->update(iTimeEllapsedms))
		{
			//cout << "missile " << pMissile << " asks for die" << endl;
			// The missile cannot do more damage.
			bDelete=true;
		}

		if (bDelete)
		{
			delete pMissile;
			oit=mlstMissiles.erase(oit);
		}
		else
			oit++;
	}
	return false;
}

void towerBase::drawAt(const coord &cdest, SDL_Surface* dest) const
{
	coord pos(cdest.x()+tileSizeX/2,cdest.y()+tileSizeY/2);

	if (mbWithRange)
	{
		aacircleColor(dest,pos.x(),pos.y(),getRange(),0xffffffff);
		SDL_SetAlpha(img,SDL_SRCALPHA,128);
	}
	gpGame->drawTileCentered(dest,miNumber, cdest,"towers");
	SDL_SetAlpha(img,SDL_SRCALPHA,SDL_ALPHA_OPAQUE);
}

/*
void towerBase::drawAt(const coord &cdest, SDL_Surface* dest) const
{
	SDL_Rect srce,odest;
	srce.x=imageCoords.x();
	srce.y=imageCoords.y();
	srce.w=26;	// FIXME
	srce.h=26;	// FIXME
	odest.x=cdest.x()+(tileSizeX-26)/2;
	odest.y=cdest.y()+(tileSizeY-26)/2;
	odest.w=srce.w;
	odest.h=srce.h;
	if (mbWithRange)
	{
		aacircleColor(dest,cdest.x()+tileSizeX/2,cdest.y()+tileSizeY/2,getRange(),0xffffffff);
		SDL_SetAlpha(img,SDL_SRCALPHA,128);
	}
	else
	{
		SDL_SetAlpha(img,SDL_SRCALPHA,255);
	}
	// FIXME, img is still to remove (global)
	SDL_BlitSurface(img, &srce, dest, &odest);
	SDL_SetAlpha(img,SDL_SRCALPHA,SDL_ALPHA_OPAQUE);

}
*/

void towerBase::drawMissiles(SDL_Surface* dest)
{
	list<missileBase*>::iterator oit=mlstMissiles.begin();

	while (oit!=mlstMissiles.end())
	{
		(*oit++)->draw(dest);
	}
}

bool towerBase::checkTargets(missileBase* pMissile)
{
	bool bRet=false;
	int iFreeWeapons=1;

	// Do not delete missiles when trying to change a missile's target
	if (pMissile==0)
	{
		// 1) check if any weapon has finished its job
		list<missileBase*>::iterator oitM=mlstMissiles.begin();
		while(oitM!=mlstMissiles.end())
		{
			missileBase* p=*oitM;
			if (p->jobFinished())
			{
				cout << "tower deleting " << p << endl;
				delete p;
				oitM=mlstMissiles.erase(oitM);
			}
			else
				oitM++;
		}
		iFreeWeapons=miMaxMissiles-mlstMissiles.size();
	}

	if (canFire() || pMissile!=0)
	{
		// FIXME this method should ask
		// to the missileFactory if the target is ok for the
		// missile. For example air-air missiles cannot shoot ground targets.
		// Also we should use a scoring system in order to
		// sort targets. For example, the player would like to shoot
		// strongest / closest targets. Or, a cold tower wants to slow
		// the target which has the highest speed.
		list<walkerBase*>::const_iterator	oit=lstWalkers.begin();
		while(oit!=lstWalkers.end() && iFreeWeapons)
		{
			walkerBase*	p=*oit++;
			if (!p->isDead() && p->getVisible() && countMissilesOnTarget(p)<miMaxMissilePerTarget)
			{
				//cout << "checking target" << endl;
				int iDist;

				if (pMissile!=0)
					iDist=pMissile->getCoord().distanceTo(p->getShootPoint());
				else
					iDist=mCenterCoords.distanceTo(p->getShootPoint());
				if (iDist<getRange())
				{
					if (mpMissileFactory)
					{
						if (mpMissileFactory->isTargetable(p))
						{
							if (pMissile==0)
							{
								pMissile=mpMissileFactory->build(this,p);
								mlstMissiles.push_front(pMissile);
								miCoolDown=miCoolDownTime;
								//cout << "new missile, cooldown=" << miCoolDown << endl;

								if (canFire()==false)
								{
									//cout << "cannot fire anymore" << endl;
									iFreeWeapons=0;
								}
								else
								{
									//cout << "can fire..." << endl;
									iFreeWeapons--;
								}
								pMissile=0;
							}
							else
							{
								iFreeWeapons=0;
								bRet=true;
								//cout << "changing target for " << pMissile << "!" << endl << flush;
								pMissile->setTarget(p);
							}
						}
					}
				}
			}
		}
	}
	return bRet;
}

int towerBase::countMissilesOnTarget(walkerBase* p)
{
	int iCount=0;

	list<missileBase*>::iterator	oit=mlstMissiles.begin();

	while(oit!=mlstMissiles.end())
	{
		if ((*oit++)->getTarget()==p)
			iCount++;
	}
	return iCount;
}

bool towerBase::isBuildable() const
{
	// FIXME other costs (bonuses)
	return gpGame->mlBank>=getCostMoney();
}

void towerBase::spendCosts() const
{
	gpGame->burnMoney(getCostMoney());
}

const string towerBase::getDamageInfo() const
{
	if (mpMissileFactory)
		return mpMissileFactory->getDamageInfo(this);
	else
		return "No factory !!!";
}
// Read a config file
void towerBase::readOneTower(CFileParser* poDef)
{

	int iNumber=poDef->getNextLong("tower number");
	poDef->getExpectedChar("{");

	towerBase*	pNewTower=new towerBase(iNumber,coord(0,0));

	while(true)	// break after '}'
	{
		if (poDef->peekChar()=='}')
		{
			poDef->getNextChar();
			break;
		}
		else
		{
			string sItem=poDef->getNextIdentifier("tower item");

			if (sItem=="name")
			{
				pNewTower->setName(poDef->getNextString());
			}
			else if (sItem=="coord")
			{
				coord p;
				p.readDef(poDef);
				pNewTower->mpos=p;
				p.add(coord(12.5,12.5));
				pNewTower->mCenterCoords=p;	// FIXME, using setCoord makes offset while in towers area
			}
			else if (sItem=="comments")
			{
				pNewTower->msComments=poDef->getNextString("comments");
			}
			else if (sItem=="text")
			{
				pNewTower->setText(poDef->getNextString());
			}
			else if (sItem=="weapon_count")
			{
				pNewTower->miMaxMissiles=poDef->getNextLong();
			}
			else if (sItem=="max_targets")
			{
				pNewTower->miMaxMissilePerTarget=poDef->getNextLong();
			}
			else if (sItem=="cool_down")
			{
				pNewTower->miCoolDownTime=poDef->getNextLong("cooldown value");
			}
			else if (sItem=="cost_money")
			{
				pNewTower->setCostMoney(poDef->getNextLong());
			}
			else if (sItem=="damage")
			{
				pNewTower->setDamage(poDef->getNextLong());
			}
			else if (sItem=="damage_increase")
			{
				pNewTower->setDamageIncrease(poDef->getNextLong());
			}
			else if (sItem=="range")
			{
				pNewTower->setRange(poDef->getNextLong());
			}

			else if (sItem=="range_increase")
			{
				pNewTower->setRangeIncrease(poDef->getNextLong());
			}
			else if (sItem=="weapon")
			{
				missileFactory* p=missileFactory::getFactory(poDef->getNextIdentifier("weapon"));
				//cout << "factory=" << p << endl;
				pNewTower->setMissileFactory(p);
			}
			else
			{
				cerr << "Error: Unknown item " << sItem << endl;
				//throw new CSException("ReadTower","Unknown tower item : "+sItem);
			}
		}
	}
	mapTowers[iNumber]=pNewTower;

}

