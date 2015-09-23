/*
 * coperator.hpp
 *
 *  Created on: 25 oct. 2009
 *      Author: saturn
 */

#ifndef COPERATOR_HPP_
#define COPERATOR_HPP_

#include <cexprelement.hpp>

class CExpressionParser;
class CExpression;

class CType;

/**
 *
 * This class is abstract since we don't know here how
 * many operand the operator must have.
 **/
class COperator : public CExprElement
{
public:
	/**
	 * Any kind of operator has at least two properties:
	 * its precedence and the type of the operator.
	 * For example the binary operator +(int,int) has a low precedence
	 * and returns an int.
	 */
	COperator(int iPrecedence, const CType* poReturnedType);

	/**
	 * The operator must provide its own parse method.
	 * This allow more flexibility for special kind of operators.
	 * @see Basic operators for an example
	 * If the parsing is successfull, the current expression will contains all expressions
	 * that will further use in order to evaluate the result of the whole expression
	 * /!\ If not, poCurrentExpression must be left unchanged
	 *
	 * @param CExpression* poCurrentExpression
	 * @param CExpressionParser* poParser
	 * @return bool true if the parsing is successful.
	 * @abstract
	 *
	 */
	virtual bool parse(CExpression* poCurrentExpression, CExpressionParser*) const=0;

	int getPrecedence() const { return miPrecedence; }

private:
	int miPrecedence;
};

#endif /* COPERATOR_HPP_ */
