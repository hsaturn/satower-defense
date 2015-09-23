/*
 * coperand.hpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#ifndef COPERAND_HPP_
#define COPERAND_HPP_

#include <string>
#include <cexprelement.hpp>

using namespace::std;

class CType;
class CExpressionParser;

// the class COperand is designed to handle
// any kind of operand.
// The COperand class must be able to use a CExpressionParser in order
// to return the longest string that matches the operand type.
// An operand has mandatory a type.
//
// The expression parser provides

// abstract
class COperand : public CExprElement
{
public:
	COperand(const CType *poType);

	/**
	 * parse uses the parser given and then
	 * return the longest string that is a valid
	 * operand.
	 * parse returns an empty string if nothing matches
	 * the type of the operand in the parser.
	 *
	 * When the expression parser finds a valid operand,
	 * it then builds a COperandValue instance. TODO
	 *
	 * @param CExpressionParser* An expression parser
	 * @param string    &sParsed	The return value if parse is successful
	 * @return bool    true in case of valid operand
	 */
	virtual bool parse(CExpressionParser *po, string &sParsed) const=0;

	/**
	 * Compare the types of this and the argument.
	 *
	 * @return bool true if this and the value have the same type.
	 */
	bool isTypeOf(const COperand*) const;

};

#endif /* COPERAND_HPP_ */
