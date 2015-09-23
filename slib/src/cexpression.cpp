/*
 * cexpresion.cpp
 *
 *  Created on: 25 oct. 2009
 *      Author: saturn
 */

#include <cexpression.hpp>

CExpression::CExpression()
{

}

CExpression::const_iterator	CExpression::begin() const
{
	return mlstExpression.begin();
}

CExpression::const_iterator CExpression::end() const
{
	return mlstExpression.end();
}

CExpression::iterator CExpression::begin()
{
	return mlstExpression.begin();
}

CExpression::iterator CExpression::end()
{
	return mlstExpression.end();
}

void CExpression::addElement(const CExprElement* poElement)
{
	mlstExpression.push_back(poElement);
}
