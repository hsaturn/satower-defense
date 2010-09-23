/*
 * walkerGenerator.hpp
 *
 *  Created on: 20 juil. 2010
 *      Author: saturn
 */

#ifndef WALKERGENERATORBASE_HPP_
#define WALKERGENERATORBASE_HPP_

#include <list>

using namespace std;

class walkerBase;
#include "coord.hpp"

class walkerGeneratorBase
{
public:

	/**
	 * @param pWalkerToClone  The walker to generate
	 * @param iSalveCount	  Number of walker to generate
	 * @param iDelayMs		  Delay between two walker
	 * @param dest            Destination of the walkers
	 * @param iInitialWaitSec Initial delay in seconds
	 */
	walkerGeneratorBase(
			walkerBase* pWalkerToClone,
			int iSalveCount,
			int iDelayMs,
			int iInitialWaitSec);

	/**
	 * @param iTimeEllapsedMs Time ellapsed since last call.
	 * @return 0 if no walker created.
	 * else a valid walkerBase*
	 */
	walkerBase*	update(int iTimeEllapsedMs);

	/**
	 * @return int number of walker left to be generated.
	 */
	int	walkersLeft() const { return miSalveCount; }


	/**
	 * Prepare the next wave if possible.
	 * return true if next wave has been prepared.
	 * @param Uint32 iTimeMs, option set the time before wave is launched
	 *               this parameter is ignore if time left is less.
	 * @see Wave::nextWave()
	 */
	bool sendNow(Uint32 iTimeMs=0);

	/**
	 * Get the time before wave is launched.
	 * @return number of ms or 0 (already launched)
	 */
	Uint32	timeLeftBeforeWave() const { if (mbStarted) return 0; return miNextWalker; }

private:
	walkerBase*	mpWalkerToClone;
	int miSalveCount;
	int	miDelayMs;
	int miNextWalker;
	list<coord>::const_iterator	moitStart;
	list<coord>::const_iterator moitEnd;
	int miStart;			// Use to send groups of walkers at once for each start tile.
	bool	mbStarted;		// False if no walker has been released yet
};
#endif /* WALKERGENERATORBASE_HPP_ */
