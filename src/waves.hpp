/*
 * waves.hpp
 *
 *  Created on: 31 août 2010
 *      Author: saturn
 */

#ifndef WAVES_HPP_
#define WAVES_HPP_

#include "walkerGeneratorBase.hpp"
#include "walkers.hpp"

class Waves
{
public:
	Waves();
	~Waves();

	walkerBase* update(Uint32 lEllapsed);

	const walkers::const_iterator* getCurWalker() const { return &moCurWalker; }

	/**
	 * Prepare the next wave if possible.
	 * return true if next wave has been prepared.
	 * @param Uint32 iTimeMs, option set the time before wave is launched
	 *               this parameter is ignore if time left is less.
	 * @see walkerGeneratorBase::sendNow()
	 */
	bool nextWave(Uint32 iTimeMs=0);

	/**
	 * Get the time before next wave is launched.
	 * @return number of ms
	 */
	Uint32	timeLeftBeforeNextWave() const;

private:
	walkerGeneratorBase*		mpoGenerator;
	walkers::const_iterator		moitWalkersNames;
	walkers::const_iterator 	moCurWalker;
};

#endif /* WAVES_HPP_ */
