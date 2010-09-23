/*
 * walkerGeneratorBase.cpp
 *
 *  Created on: 20 juil. 2010
 *      Author: saturn
 */

#include "walkerGeneratorBase.hpp"
#include "walkerBase.hpp"
#include "game.hpp"

extern coord coordToPixel(coord);

walkerGeneratorBase::walkerGeneratorBase(
		walkerBase* pWalkerToClone,
		int iSalveCount,
		int iDelayMs,
		int iInitialDelaySec)
:
	mpWalkerToClone	(pWalkerToClone),
	miSalveCount	(iSalveCount),
	miDelayMs		(iDelayMs*gpGame->startsCount()),
	miNextWalker	(iDelayMs*gpGame->startsCount()+1000*iInitialDelaySec),
	moitStart		(gpGame->beginStarts()),
	moitEnd			(gpGame->beginEnds()),
	miStart			(1),
	mbStarted		(false)
	{
		if (pWalkerToClone->isBoss())
		{
			miSalveCount=pWalkerToClone->getMaxCount();
		}
	}

walkerBase* walkerGeneratorBase::update(int iTimeEllapsedMs)
{
	walkerBase* p=0;

	if (miSalveCount>0)
	{
		miNextWalker-=iTimeEllapsedMs;
		if (miNextWalker<0)
		{
			mbStarted=true;
			miSalveCount--;
			if (miStart==gpGame->startsCount())
			{
				miNextWalker+=miDelayMs;
				miStart=1;
			}
			else
			{
				miStart++;
			}
			p=new walkerBase(mpWalkerToClone);
			if (p->isBoss())
			{
				gpGame->bossAdd();
				gpGame->changeMusic(p->getMusic());
			}
			p->setHealth(p->getHealth()*gpGame->getHealthFactor());
			// Compute a new path

			if (moitStart==gpGame->endStarts())
				moitStart=gpGame->beginStarts();
	//		cout << ":" << *moitStart << " --> " << coordToPixel(*moitStart) << endl;
			p->setCoord(coordToPixel(*moitStart++));
			int i=gpGame->endsCount();
			bool bOk=false;
			while(i>0)
			{
				if (moitEnd==gpGame->endEnds())
					moitEnd=gpGame->beginEnds();
				p->setDestination(*moitEnd++,"walkerGeneratorBaseSet");
				if (p->computePath("walkerGeneratorBase"))
				{
					bOk=true;
					break;
				}
				i--;
			}
			p->setVisible(true);
			if (!bOk)
			{
				delete p;
				p=0;
				cerr << "walkerGeneratorBase : unable to generate walker (bad end point or no path)" << endl;
			}
		}
	}
	//if (p)
	//	cout << "walker out ! " << p->getCoord() << " -> " << p->getDestination() << endl;
	return p;
}

bool walkerGeneratorBase::sendNow(Uint32 iTimeMs)
{
	if (!mbStarted)
	{
		if (miNextWalker>(int)iTimeMs)
			miNextWalker=iTimeMs;
	}
	return !mbStarted;
}

