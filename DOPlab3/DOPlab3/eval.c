/*
* File: eval.c
* ------------
* This file implements the eval.h interface.
*/

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"
#include "eval.h"
#include "symtab.h"

/*
* Private variable: variableTable
* -------------------------------
* This table keeps track of the values for each variable.
*/

static symtabADT variableTable;

/* Private function prototypes */

static int EvalCompound(expADT exp);

/* Exported entries */

int EvalExp(expADT exp)
{
	switch (ExpType(exp)) {
	case ConstExp: {
		return (ExpInteger(exp));
	}
	case IdentifierExp: {
		return (GetIdValue(ExpIdentifier(exp)));
	}
	case CompoundExp:
		return (EvalCompound(exp));
	case FuncExp:
		//return ();
		break;
	case IfExp:
		return (EvalIfExp(exp));
	case CallExp:
		//return ();
		break;
	default:
		Error("Unidentified Evaluation");
	}
}

void InitVariableTable(void)
{
	variableTable = NewSymbolTable();
}

int GetIdValue(string name)
{
	int *ip;

	ip = Lookup(variableTable, name);
	if (ip == UNDEFINED)  Error("%s is undefined", name);
	return (*ip);
}

void SetIdValue(string name, int value)
{
	int *ip;

	ip = New(int *);
	*ip = value;
	Enter(variableTable, name, ip);
}

/* Private functions */

static int EvalCompound(expADT exp)
{
	char op;
	int lhs, rhs;

	op = ExpOperator(exp);
	if (op == '=') {
		rhs = EvalExp(ExpRHS(exp));
		SetIdValue(ExpIdentifier(ExpLHS(exp)), rhs);
		return (rhs);
	}
	lhs = EvalExp(ExpLHS(exp));
	rhs = EvalExp(ExpRHS(exp));
	switch (op) {
	case '+': return (lhs + rhs);
	case '-': return (lhs - rhs);
	case '*': return (lhs * rhs);
	case '/': return (lhs / rhs);
	default:  Error("Illegal operator");
	}
}

static int EvalIfExp(expADT exp) {
	char op;
	int lhs, rhs, thenSum, elseSum;

	op = GetIfRelOp(exp);
	lhs = (EvalExp(GetIfLHSExpression(exp)));
	rhs = (EvalExp(GetIfRHSExpression(exp)));
	thenSum = (EvalExp(GetIfThenPart(exp)));
	elseSum = (EvalExp(GetIfElsePart(exp)));

	switch (op) {
	case '<':
		return (lhs < rhs ? thenSum : elseSum);
	case '=':
		return (lhs == rhs ? thenSum : elseSum);
	case '>':
		return (lhs > rhs ? thenSum : elseSum);
	default:
		Error("Unknown operator");
	}
}