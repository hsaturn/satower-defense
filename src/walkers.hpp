/*
 * walkers.hpp
 *
 *  Created on: 29 juil. 2010
 *      Author: saturn
 */

#ifndef WALKERS_HPP_
#define WALKERS_HPP_

/**
 * This class is a walkerBase list holder/builder.
 * Also this class parse a walker block (not inside, which is for walkerBase)
 * definition file.
 */
#include <map>
#include <string>

using namespace std;

class walkerBase;

class walkers
{
public:
	static walkerBase*	buildWalker(const string &sName);

	typedef map<string,walkerBase*>	T_CONTAINER;
	typedef T_CONTAINER::iterator iterator;
	typedef T_CONTAINER::const_iterator const_iterator;

	static const_iterator begin() { return gmapWalkers.begin(); }
	static const_iterator end() { return gmapWalkers.end(); }
	static const_iterator find(const string &sName) { return gmapWalkers.find(sName); }

	static void registerWalker(const string &sName, walkerBase* pWalker);
private:
	static map<string, walkerBase*>	gmapWalkers;
};

#endif /* WALKERS_HPP_ */
