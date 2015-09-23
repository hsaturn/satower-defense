/*
 * cbasicoperands.hpp some usual and basic operands used within the CExpressionParser class.
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#ifndef CBASICOPERANDS_HPP_
#define CBASICOPERANDS_HPP_

#include <coperand.hpp>

// List of singleton classes defined in this file
// COperandInt
// COperandFloat
// COperandString

// Singleton
class COperandInt : public COperand
{
public:
	static const COperandInt*	getInstance();

	virtual bool parse(CExpressionParser *po, string &) const;

private:
	COperandInt();
	COperandInt(const COperandInt&);
	COperandInt& operator=(const COperandInt &);

	static	COperandInt* mpoInstance;
};

// Singleton
class COperandFloat : public COperand
{
public:
	static const COperandFloat* getInstance();

	virtual bool parse(CExpressionParser *po, string &) const;


private:
	COperandFloat();
	COperandFloat(const COperandFloat &);
	COperandFloat& operator =(const COperandFloat &);

	// A float has a mantiss and and exponent that can have the same
	// format. Thus parseOne is usefull to parse both parts.
	// parseOne parses [0-9][0-9]*(.[0-9][0-9]*)?
	bool parseOne(CExpressionParser* po, string &) const;

	static COperandFloat* mpoInstance;
};

// Singleton
class COperandString : public COperand
{
public:
	static const COperandString* getInstance();

	virtual bool parse(CExpressionParser *po, string &) const;

private:
	COperandString();
	COperandString(const COperandString&);
	COperandString& operator = (const COperandString&);

	static COperandString* mpoInstance;
};

#endif /* CBASICOPERANDS_HPP_ */
