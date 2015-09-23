/*
 * cexpressionparser.hpp
 *
 *  Created on: 22 oct. 2009
 *      Author: saturn
 */

#ifndef CEXPRESSIONPARSER_HPP_
#define CEXPRESSIONPARSER_HPP_

// Very powerfull expression parser.
// This parser is not aware of what it does parse.
//
// The only thing the parser knows is the following grammar.
//
// expr:= operand | (expr) | unop(expr) |  expr op1 expr | expr op2 expr | .... | expr opn expr;
// opn := operator (user defined) with priority
//
// values and operators are defined prior to use the parser.
//
// During the parse, the engine always try to find the longest match:
//
// Given the following expression
// the following operands of type T PI, P and I,
// the following T operator * (T,T)
// the parser may find the following expressions:
// P I * 5 that returns a T
// PI * 5
// As PI * 5 is a valid expression. The parser does not try the other one (P I * 5)
// Also, because PI is longer than P, the first expression checked is PI * 5.
//
// An operator takes to expr and returns an expr.
//
// Types are defined prior ti use the parser also.

// An expression has a type. Operators may mix types together.
//
// expr are stongly typed.
//
// Thus the following expressions could be parsed
//
// "alpha beta delta" - "beta"  (result could be "alpha delta")
// "four" * 4  (result could be "four four four four "
//
// Precedences of operator is made by using its number.
// Thus op1 as a lower priority than opn
//
// Cast operators are considered as special unary op (unop(expr)).
// These operator are 'special' because the expression parser may decide
// to automatically call such an operator to make the expression match
// a valid expression.
//
// Because expr are strongly typed, the parser will never accept
// even trivial casts (byte -> int). Such a casts have to be
// defined.
//
// The parser may want to obtain a specific type as output.
// This may allow the parser to decide proper conversion to make
// prior to return the calculation path.
//
// This is why the ::parse() method can be invoked with a type as argument.
//
// Finally, the parser take a file parser to parse an expression.
//
// The CFileParser given when building a CExpressionParser can
// only be constructed with bIgnoreEol=false (see the constructor of CFileParser).
//

#include <list>

using namespace std;

class CFileParser;
class COperand;
class CExpression;
class COperator;

class CExpressionParser
{
public:
	CExpressionParser(CFileParser *poParser);

	/**
	 * Add an operand to the expr parser.
	 * This can sounds strange, almost a bad design and/or bad name
	 *
	 * This methods just teach to the parser how to handle a kind of operand.
	 * It is usual to add at least the basicoperand::COperandInt to a parser.
	 * Without calling this method, the parser wont be able to parse any expr.
	 *
	 * @return void
	 */
	void	addOperand(const COperand*);

	/**
	 * Adds an operator to the parser.
	 *
	 * @return void
	 */
	void addOperator(const COperator*);

	/**
	 * Parse the expression given in CFileParser
	 * elements found are added to poExpression.
	 * This method search for operand prior to search for operators.
	 *
	 * @return CExpression* expression or 0 if unsuccessful
	 */
	CExpression*	parse(CExpression* poExpression=0);

	/** List of method related to CFileParser */
	char   getNextChar(bool bTrim=false, bool bConvertTabs=true);
	char  peekChar(bool bTrim=true);
	bool eol();
	bool tryChar(char c);

private:
	/**
	 * Once an operand has been found, we may want to try parsing operators.
	 *
	 * Given an operand (that has a type), this method search for operators that
	 * takes a left operand of the same type in order to build an expression.
	 * All operands of the choosen operator should be parsed when returning from this
	 * method.
	 *
	 * @param CExpression* an incomplete expression currently built.
	 * @return CExpression* a more complete expression (but not finished yet).
	 */
	CExpression* _parseOperators(CExpression* poCurrentExpression, const COperand*);

private:
	CFileParser*				mpoParser;
	list<const COperand*>		mlstOperands;			// List of possible operands
	list<const COperator*>		mlstOperators;			// List of possible operators
	/*list<const COperator*>		lstOperators;		// List of known operators
	list<const CUnoperator*>	lstUnoperatords;		// List of known unary operators
	list<const CCastors*>		lstCastors;				// List of known cast operators.
	*/

};
#endif /* CEXPRESSIONPARSER_HPP_ */
