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
	walkerBase* pWalker=0;

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
			pWalker=new walkerBase(mpWalkerToClone);
			if (pWalker->isBoss())
			{
				gpGame->bossAdd();
				gpGame->changeMusic(pWalker->getMusic());
			}
			pWalker->setHealth(pWalker->getHealth()*gpGame->getHealthFactor());
			// Compute a new path

			if (moitStart==gpGame->endStarts())
				moitStart=gpGame->beginStarts();
	//		cout << ":" << *moitStart << " --> " << coordToPixel(*moitStart) << endl;
			pWalker->setCoord(coordToPixel(*moitStart++));
			int i=gpGame->endsCount();
			bool bOk=false;
			while(i>0)
			{
				if (moitEnd==gpGame->endEnds())
					moitEnd=gpGame->beginEnds();
				pWalker->setDestination(*moitEnd++,"walkerGeneratorBaseSet");
				if (pWalker->computePath("walkerGeneratorBase"))
				{
					bOk=true;
					break;
				}
				i--;
			}
			pWalker->setVisible(true);
			if (!bOk)
			{
				delete pWalker;
				pWalker=0;
				cerr << "walkerGeneratorBase : unable to generate walker (bad end point or no path)" << endl;
			}
		}
	}
	//if (pWalker)
	//	cout << "walker out ! " << pWalker->getCoord() << " -> " << pWalker->getDestination() << endl;
	return pWalker;
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

