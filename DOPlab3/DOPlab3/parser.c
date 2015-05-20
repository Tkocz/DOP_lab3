#include "parser.h"

static expADT ReadE(scannerADT scanner);
static expADT ReadT(scannerADT scanner);
static expADT ReadC(scannerADT scanner);
static expADT ReadF(scannerADT scanner);

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

static expADT ReadE(scannerADT scanner)
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
		SaveToken(scanner, token);
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

static expADT ReadT(scannerADT scanner){
	expADT exp;
	string token;
	expADT comp;

	if (!MoreTokensExist(scanner)){
		Error("Tried to read empty scanner");
	}
	exp = ReadC(scanner);
	if (MoreTokensExist(scanner)){
		token = ReadToken(scanner);
		if (token[0] == '*' || token[0] == '/'){
			comp = NewCompoundExp(token[0], exp, ReadT(scanner));
			return (comp);
		}
		SaveToken(scanner, token);
		return (exp);
	}
	return(exp);
}


static expADT ReadC(scannerADT scanner) {
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

static expADT ReadF(scannerADT scanner) {
	expADT exp, exp1, exp2, thenE, elseE;
	string token, parenthesisCheck;
	char relOp;

	if (!MoreTokensExist(scanner))
		Error("Tried to read empty scanner");

	token = ReadToken(scanner);
	if (token[0] == '(') {
		exp = ReadE(scanner);
		if (MoreTokensExist) {
			token = ReadToken(scanner);
			if (token[0] == ')')
				return exp;
		}
		Error("Unbalanced parenthesis");
	}
	else if (StringEqual(token, "if")) {
		if (MoreTokensExist) {
			parenthesisCheck = ReadToken(scanner);
			if (!StringEqual(parenthesisCheck, "(")) {
				SaveToken(scanner, parenthesisCheck);
			}
			exp1 = ReadE(scanner);
			token = ReadToken(scanner);
			// TODO: felkontroll!
			relOp = token[0];
			exp2 = ReadE(scanner);
			if (StringEqual(parenthesisCheck, "(")) {
				parenthesisCheck = ReadToken(scanner);
				if (!StringEqual(parenthesisCheck, ")"))
					Error("Unbalanced parenthesis in If-statement");
			}
			if (MoreTokensExist(scanner) && StringEqual(ReadToken(scanner), "then")) {
				thenE = ReadE(scanner);
				if (MoreTokensExist(scanner) && StringEqual(ReadToken(scanner), "else")) {
					elseE = ReadE(scanner);
					exp = NewIfExp(exp1, relOp, exp2, thenE, elseE);
					return exp;
				}
				else {
					Error("No Else statement found");
				}
			}
			else
				Error("No Then statement found");

		}
	}
	else if (StringEqual(token, "func")) {
		if (StringEqual(ReadToken(scanner), "(")) {
			token = ReadToken(scanner);
			if (StringEqual(ReadToken(scanner), ")") && StringEqual(ReadToken(scanner), "{")) {
				exp2 = ReadE(scanner);
				if (!StringEqual(ReadToken(scanner), "}"))
					Error("Unbalanced expression");
				exp = NewFuncExp(token, exp2);
				return exp;
			}
		}
		else
		Error("Missing parameter parenthesis");
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