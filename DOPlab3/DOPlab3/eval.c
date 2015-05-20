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

static valueADT EvalCompound(expADT exp);

/* Exported entries */

valueADT Eval(expADT exp, environmentADT env)
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

valueADT GetIdValue(string name)
{
	int *ip;

	ip = Lookup(variableTable, name);
	if (ip == UNDEFINED)  Error("%s is undefined", name);
	return (*ip);
}

void SetIdValue(string name, valueADT value)
{
	int *ip;

	ip = New(int *);
	*ip = value;
	Enter(variableTable, name, ip);
}

/* Private functions */

static valueADT EvalCompound(expADT exp, environmentADT parent)
{
	char op;
	valueADT value;
	int lhs, rhs;

	environmentADT current;
	current = NewClosure(parent);

	op = ExpOperator(exp);
	if (op == '=') {
		SetIdValue(ExpIdentifier(ExpLHS(exp)), Eval(ExpRHS(exp), current));
		return (rhs);
	}
	lhs = GetIntValue(Eval(ExpLHS(exp), current));
	rhs = GetIntValue(Eval(ExpRHS(exp), current));

	switch (op) {
	case '+': return NewIntegerValue(lhs + rhs);
	case '-': return NewIntegerValue(lhs - rhs);
	case '*': return NewIntegerValue(lhs * rhs);
	case '/': return NewIntegerValue(lhs / rhs);
	default:  Error("Illegal operator");
	}
}

static int EvalIfExp(expADT exp) {
	char op;
	int lhs, rhs, thenSum, elseSum;

	op = GetIfRelOp(exp);
	lhs = (Eval(GetIfLHSExpression(exp)));
	rhs = (Eval(GetIfRHSExpression(exp)));
	thenSum = (Eval(GetIfThenPart(exp)));
	elseSum = (Eval(GetIfElsePart(exp)));

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