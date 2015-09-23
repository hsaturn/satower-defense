/*
 * ctype.cpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */
#include <ctype.hpp>

using namespace std;

map<string,const CType*>	CType::mmapTypes;

const CType* CType::factory(const string sType)
{
	const CType* pType=0;

	map<string,const CType*>::const_iterator oit=mmapTypes.begin();

	if (oit==mmapTypes.begin())
	{
		pType=new CType(sType);
		mmapTypes[sType]=pType;
	}
	else
		pType=oit->second;

	return pType;
}

CType::CType(const string sType)
	:
	msType(sType)
{
}

// The == operator is trivial since this class has the singleton prototype.
// == is true only if two types compared are the same object.
bool CType::operator ==(const CType& oType) const
{
	return this==&oType;
}
