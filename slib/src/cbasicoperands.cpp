/*
 * cbasicoperands.cpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#include <cbasicoperands.hpp>
#include <ctype.hpp>
#include <cexpressionparser.hpp>

using namespace std;

COperandInt* COperandInt::mpoInstance=0;

const COperandInt* COperandInt::getInstance()
{
	if (mpoInstance==0)
		mpoInstance=new COperandInt;

	return mpoInstance;
}

COperandInt::COperandInt()
	:
	COperand(CType::factory("int"))
{

}

bool COperandInt::parse(CExpressionParser* poParser,string &sValue) const
{
	bool bOk;
	char c;

	c=poParser->peekChar(true);
	if (c=='-')
	{
		poParser->getNextChar(true);	// Remove the - char
		sValue="-";
		c=poParser->peekChar(false);
	}
	while((c>='0')&&(c<='9')&&(!poParser->eol()))
	{
		bOk=true;
		poParser->getNextChar(false);
		sValue+=c;
		c=poParser->peekChar(false);
	}

	return bOk;
}

/****************************************/
COperandFloat* COperandFloat::mpoInstance=0;

COperandFloat::COperandFloat()
	:
	COperand(CType::factory("float"))
{

}

const COperandFloat* COperandFloat::getInstance()
{
	if (mpoInstance==0)
		mpoInstance=new COperandFloat;

	return mpoInstance;
}

bool COperandFloat::parse(CExpressionParser* poParser, string &sParsed) const
{
	bool bOk=false;
	string sMantiss;
	string sExponent;
	char c;

	if (parseOne(poParser,sMantiss))
	{
		bOk=true;
		c=poParser->tryChar('e');
		if (c==0)
			c=poParser->tryChar('E');

		if (c)
		{
			if (parseOne(poParser,sExponent))
			{
				sMantiss+=c;
				sMantiss+=sExponent;
			}
		}
	}

	sParsed=sMantiss+sExponent;
	return bOk;
}

bool COperandFloat::parseOne(CExpressionParser* poParser, string &sValue) const
{
	bool bOk=false;
	bool bDot=false;
	char c;

	c=poParser->peekChar(true);
	if (c=='-')
	{
		poParser->getNextChar(true);
		sValue="-";
		c=poParser->peekChar(false);
	}
	while((((c>='0')&&(c<='9')) || (bDot==false && c=='.')) && (!poParser->eol()))
	{
		bOk=bOk || ((c>='0')&&(c<='9'));

		poParser->getNextChar(false);
		if (c=='-')
			bDot=true;
		sValue+=c;
		c=poParser->peekChar(true);
	}

	return bOk;
}

/************************************/
COperandString* COperandString::mpoInstance=0;


COperandString::COperandString()
	:
	COperand(CType::factory("string"))
{

}

const COperandString* COperandString::getInstance()
{
	if (mpoInstance==0)
		mpoInstance=new COperandString;

	return mpoInstance;
}

bool COperandString::parse(CExpressionParser* poParser, string &sValue) const
{
	bool bOk=false;
	char cQuote=0;
	char c;

	c=poParser->getNextChar(true);

	if ((c=='\'') || (c=='"'))
	{
		cQuote=c;
		c=poParser->getNextChar(true);
	}
	while((cQuote!=0) && (c!=cQuote) && (!poParser->eol()))
	{
		sValue+=c;
		c=poParser->getNextChar(false);
	}
	if (c==cQuote)
		bOk=true;

	return bOk;
}

