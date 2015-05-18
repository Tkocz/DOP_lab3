#include "parser.h"

/*
* Implementation notes: ParseExp
* ------------------------------
* This function just calls ReadE to read an expression and then
* checks to make sure no tokens are left over.
*/

expADT ParseExp(scannerADT scanner)
{
	expADT exp;

	exp = ReadE(scanner, 0);
	if (MoreTokensExist(scanner)) {
		Error("ParseExp: %s unexpected", ReadToken(scanner));
	}
	return (exp);
}

/*
* Implementation notes: ReadE
* Usage: exp = ReadE(scanner, prec);
* ----------------------------------
* This function reads an expression from the scanner stream,
* stopping when it encounters an operator whose precedence is
* less that or equal to prec.
*/

expADT ReadE(scannerADT scanner, int prec)
{
	expADT exp, rhs;
	string token;
	int newPrec;

	exp = ReadT(scanner);
	while (TRUE) {
		token = ReadToken(scanner);
		newPrec = Precedence(token);
		if (newPrec <= prec) break;
		rhs = ReadE(scanner, newPrec);
		exp = NewCompoundExp(token[0], exp, rhs);
	}
	SaveToken(scanner, token);
	return (exp);
}

/*
* Function: ReadT
* Usage: exp = ReadT(scanner);
* ----------------------------
* This function reads a single term from the scanner by matching
* the input to one of the following grammatical rules:
*
*       T  ->  integer
*       T  ->  identifier
*       T  ->  ( E )
*
* In each case, the first token identifies the appropriate rule.
*/

expADT ReadT(scannerADT scanner)
{
	expADT exp;
	string token;

	token = ReadToken(scanner);
	if (StringEqual(token, "(")) {
		exp = ReadE(scanner, 0);
		if (!StringEqual(ReadToken(scanner), ")")) {
			Error("Unbalanced parentheses");
		}
	}
	else if (isdigit(token[0])) {
		exp = NewIntegerExp(StringToInteger(token));
	}
	else if (isalpha(token[0])) {
		exp = NewIdentifierExp(token);
	}
	else {
		Error("Illegal term in expression");
	}
	return (exp);
}