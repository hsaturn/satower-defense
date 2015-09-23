/*
 * coperator.cpp
 *
 *  Created on: 27 oct. 2009
 *      Author: saturn
 */
#include <coperator.hpp>

COperator::COperator(int iPrecedence, const CType* poType)
	:
	CExprElement(poType),
	miPrecedence(iPrecedence)
{

}
