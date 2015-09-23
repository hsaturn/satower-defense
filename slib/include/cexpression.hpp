/*
 * cexpression.hpp
 *
 *  Created on: 25 oct. 2009
 *      Author: saturn
 */

#ifndef CEXPRESSION_HPP_
#define CEXPRESSION_HPP_

#include <list>

using namespace std;

class CExprElement;

/**
 * The CExpression class coontains a whole expression.
 * This class consists of any number of CExprElement*.
 * Yet, we have two CExprElement classes : operands and operator.
 *
 * The expression is a human readable expression. Thus
 * 12-15 is splitted in two operand separated by the + operator.
 * COperandInt(12) COperator(+) COperandInt(15)
 *
 * This class is filled by an instance of CExpressionParser.
 */
class CExpression
{
public:
	CExpression();

	typedef list<const CExprElement*>	T_CONTAINER;
	typedef T_CONTAINER::const_iterator	const_iterator;
	typedef T_CONTAINER::iterator		iterator;
	const_iterator	begin() const;
	const_iterator	end() const;
	iterator	begin();
	iterator	end();

	/**
	 * Adds an expression element to this.
	 * the element added should be either an operand or an operator.
	 * Also, the CExpression yet does not check if the expression is
	 * valid by itself. So it is yet possible (but for what goal) to
	 * create an expression and adds two operand with no operators
	 * between them // TODO
	 *
	 * @param const CExprElement*
	 * @return void
	 */
	void addElement(const CExprElement*);

private:
	list<const CExprElement*>	mlstExpression;
};

#endif /* CEXPRESSION_HPP_ */
