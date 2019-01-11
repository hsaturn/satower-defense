/*
 * walkers.cpp
 *
 *  Created on: 29 juil. 2010
 *      Author: saturn
 */

#include <cfileparser.hpp>

#include "coord.hpp"
#include "walkers.hpp"
#include "game.hpp"
#include "vars.hpp"
#include "walkerBase.hpp"

map<string,walkerBase*>	walkers::gmapWalkers;

walkerBase* walkers::buildWalker(const string &sName)
{
	walkers::const_iterator oit=walkers::find(sName);
	if (oit==walkers::end())
	{
		throw string("Impossible de trouber le walker ("+sName+")");
	}
	return new walkerBase(oit->second);
}

void walkers::registerWalker(const string& sName, walkerBase* pWalker)
{
	walkerBase* pOldWalker = gmapWalkers[sName];
	if (pOldWalker) delete pOldWalker;
	gmapWalkers[sName]=pWalker;
}

