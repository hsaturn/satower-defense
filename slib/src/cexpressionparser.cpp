/*
 * cexpressionparser.cpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#include <string>
#include <iostream>
#include <cexpressionparser.hpp>
#include <coperand.hpp>
#include <coperator.hpp>
#include <ctype.hpp>
#include <cfileparser.hpp>
#include <cexpression.hpp>

using namespace std;

CExpressionParser::CExpressionParser(CFileParser* poParser)
	:
	mpoParser(poParser)
{
}

CExpression* CExpressionParser::parse(CExpression* poCurrentExpression)
{
	unsigned int iParsedCol;
	unsigned int iMaxLengthFound=0;
	unsigned int iFoundEndCol=0;
	const COperand*	 poFoundOperand=0;
	string       sFoundString="";

	// First search for a valid operand
	list<const COperand*>::const_iterator oit=mlstOperands.begin();

	iParsedCol=mpoParser->getParsedCol();

	// Try each operand and keep the longest result
	while(oit!=mlstOperands.end())
	{
		mpoParser->setParsedCol(iParsedCol);	// Rewind to the parsed section
		const COperand*	pOperand=*oit;
		cout << "  Trying " << pOperand->getType() << endl;

		string sParsed;
		if (pOperand->parse(this,sParsed))
		{
			if (sParsed.length()>iMaxLengthFound)
			{
				sFoundString=sParsed;
				poFoundOperand=pOperand;
				iMaxLengthFound=sParsed.length();
				iFoundEndCol=mpoParser->getParsedCol();
				cout << "  Parsed (operand " << pOperand->getType() << ") gives (" << sParsed << ')' << endl;
			}
		}
		oit++;
	}

	if (iMaxLengthFound)
	{
		if (poCurrentExpression==0)
			poCurrentExpression=new CExpression();

		cout << "Max length operand or first one is (" << poFoundOperand->getType() << ") and gave (" << sFoundString << ')' << endl;
		mpoParser->setParsedCol(iFoundEndCol);

		poCurrentExpression->addElement(poFoundOperand);
		// A valid operand is found.
		// Try to build an expression with this operand.
		_parseOperators(poCurrentExpression, poFoundOperand);
	}
	return poCurrentExpression;

}

CExpression*  CExpressionParser::_parseOperators(CExpression* poCurrentExpression,const COperand* poOperand)
{
	CExpression* poExpression=0;

	list<const COperator*>::const_iterator oit=mlstOperators.begin();

	while(oit!=mlstOperators.end())
	{
		const COperator* poOperator=*oit;

		if (poOperator->parse(poCurrentExpression, this))
			poExpression=poCurrentExpression;
		oit++;
	}

	return poExpression;
}

void CExpressionParser::addOperand(const COperand* poOperand)
{
	mlstOperands.push_back(poOperand);
}

void CExpressionParser::addOperator(const COperator* poOperator)
{
	mlstOperators.push_back(poOperator);
}

char   CExpressionParser::getNextChar(bool bTrim, bool bConvertTabs)
{
	return mpoParser->getNextChar(bTrim,bConvertTabs);
}

char  CExpressionParser::peekChar(bool bTrim)
{
	return mpoParser->peekChar(bTrim);
}

 bool CExpressionParser::eol()
{
	return mpoParser->eol();
}

bool CExpressionParser::tryChar(char c)
{
	return mpoParser->tryChar(c);
}


