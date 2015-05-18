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

expADT ReadE(scannerADT scanner)
{
	expADT exp, comp;
	string token;
	int newPrec;


	exp = ReadT(scanner);
	if (MoreTokensExist(scanner)){
		token = ReadToken(scanner);
		if (token[0] == '+' || token[0] == '-'){
			comp = NewCompoundExp(token[0], exp, ReadE(scanner));
			return (comp);
		}
		else Error("Invalid expresion ReadE");
	}
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

expADT ReadT(scannerADT scanner){
	expADT exp;
	string token;
	expADT comp;

	if (!MoreTokensExist(scanner)){
		Error("Tried to read empty scanner");
	}
	exp = ReadC(scanner);
	if (MoreTokensExist(scanner)){
		token = ReadToken(scanner);
		if (token[0] == '+' || token[0] == '-'){
			SaveToken(scanner, token);
			return exp;
		}
		if (token[0] == '*' || token[0] == '/'){
			comp = NewCompoundExp(token[0], exp, ReadT(scanner));
			return (comp);
		}
		else Error("Invalid expression ReadT");
	}
	return(exp);
}


expADT ReadC(scannerADT scanner) {
	expADT exp;
	string token;
	string arg;
	expADT func;

	if (!MoreTokensExist(scanner)){
		Error("Tried to read empty scanner");
	}
	exp = ReadF(scanner);
	if (MoreTokensExist(scanner)){
		token = ReadToken(scanner);
		if (token[0] == '('){
			arg = ReadToken(scanner);
			if (!StringEqual(ReadToken(scanner), ")")) {
				Error("Unbalanced parentheses");
			}
			func = NewFuncExp(arg, exp);
		}
		else
			SaveToken(scanner, token);
	}
	return exp;
}

expADT ReadF(scannerADT scanner) {
	expADT exp, exp2, thenE, elseE;
	string token;
	char relOp;

	if (!MoreTokensExist(scanner))
		Error("Tried to read empty scanner");

	token = ReadToken(scanner);
	if (token[0] == '(') {
		exp = ReadE(scanner);
		return exp;
	}
	else if (StringEqual(token, "if")) {
		exp = ReadE(scanner);
		token = ReadToken(scanner);
		relOp = token[0];
		exp2 = ReadE(scanner);
		if (MoreTokensExist(scanner) && StringEqual(ReadToken(scanner), "then")) {
			thenE = ReadE(scanner);
			if (MoreTokensExist(scanner) && StringEqual(ReadToken(scanner), "else")) {
				elseE = ReadE(scanner);
				// EVAL GREJ!!
				/*
				if (relOp == '>')
				if (exp > exp2)
				return thenE;
				else
				return elseE;
				if (relOp == '=')
				if (exp == exp2)
				*/
			}
			else
				Error("No Else statement found");
		}
		else
			Error("No Then statement found");

	}
	else if (isdigit(token[0])) {
		exp = NewIntegerExp(StringToInteger(token));
		return exp;
	}
	else if (isalpha(token[0])) {
		exp = NewIdentifierExp(token);
		return exp;
	}
}