/*
 * waves.cpp
 *
 *  Created on: 31 août 2010
 *      Author: saturn
 */
#include "waves.hpp"
#include "game.hpp"
#include "walkerBase.hpp"

Waves::Waves()
:
mpoGenerator(0)
{
	moitWalkersNames=walkers::begin();
	moCurWalker=moitWalkersNames;
	if (moitWalkersNames==walkers::end())
	{
		cerr << "*** Warning, internal error, count(walkers==0)" << endl;
	}
}

bool Waves::nextWave(Uint32 iTimeMs)
{
	if (mpoGenerator)
		return mpoGenerator->sendNow();
	else
		return false;
}
walkerBase* Waves::update(Uint32 lEllapsed)
{
	bool bBuildGenerator=false;

	if (mpoGenerator==0)
	{
		bBuildGenerator=true;
	}
	else
	{
		bBuildGenerator=mpoGenerator->walkersLeft()==0;
	}

	if (bBuildGenerator)
	{
		cout << "main : build generator" << endl;
		bBuildGenerator=false;
		string sWalkerName;

		if (moitWalkersNames==walkers::end())
		{
			cout << "*** NEXT LEVEL ***" << endl;
			moitWalkersNames=walkers::begin();
			moCurWalker=moitWalkersNames;
			gpGame->setHealthFactor(gpGame->getHealthFactor()*2);
			gpGame->nextLevel();
		}
		if (moitWalkersNames!=walkers::end())
		{
			sWalkerName=moitWalkersNames->first;
			cout << "Changement de vague (" << sWalkerName << ")" << endl;
			walkerBase* pGenerated=walkers::buildWalker(sWalkerName);

			if (mpoGenerator)
			{
				delete mpoGenerator;
				mpoGenerator=0;
			}
			// cerr << "dest=" << pGenerated->getCoord() << endl;
			mpoGenerator=new walkerGeneratorBase
			(
					pGenerated,
					10,
					800,
					10);
			moCurWalker=moitWalkersNames;
			moitWalkersNames++;
		}
		else
		{
			cerr << "No walker to generate. Aborting" << endl;
			exit(1);
		}
	}
	if (mpoGenerator)
		return mpoGenerator->update(lEllapsed);
	else
		return 0;	// TODO this is just a security in case of...
}
