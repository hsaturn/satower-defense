/*
 * ctype.hpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#ifndef CTYPE_HPP_
#define CTYPE_HPP_

#include<map>
#include <string>

using namespace std;

class CType // singleton
{
public:
	/** Because CType is a singleton class, we use a factory
	 * in order to get a CType.
	 */
	static const CType*	factory(const string sType);

	/**
	 * Return the type
	 *
	 * @return string Type name
	 */
	//string getType() const { return msType; };

	operator string () const { return msType; };

	// Check if two types are the same.
	bool operator ==(const CType&) const;

private:
	CType(const string sTypeName);

private:
	CType();
	CType(const CType&);
	CType& operator=(const CType&);

	string msType;

	static map<string,const CType*>	mmapTypes;
};

#endif /* CTYPE_HPP_ */
