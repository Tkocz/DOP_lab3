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
#include "env.h"

/*
* Private variable: variableTable
* -------------------------------
* This table keeps track of the values for each variable.
*/

static symtabADT variableTable;

/* Private function prototypes */

static valueADT EvalCompound(expADT exp, environmentADT env);
static valueADT EvalIfExp(expADT exp, environmentADT env);

/* Exported entries */

valueADT Eval(expADT exp, environmentADT env)
{
	switch (ExpType(exp)) {
	case ConstExp: {
		return (NewIntegerValue(ExpInteger(exp)));
	}
	case IdentifierExp: {
		return (GetIdValue(ExpIdentifier(exp)));
	}
	case CompoundExp:
		return (EvalCompound(exp, env));
	case FuncExp:
		return (GetFuncBody(exp));
	case IfExp:
		return (EvalIfExp(exp, env));
	case CallExp:
		return (GetCallActualArg(exp));
	default:
		Error("Unidentified Evaluation");
	}
}

void InitVariableTable(void)
{
	variableTable = NewSymbolTable();
}

valueADT GetIdValue(string name) {
	valueADT ip;

	ip = Lookup(variableTable, name);
	if (ip == UNDEFINED)  Error("Unknown identifier %s", name);
	return (ip);
}

void SetIdValue(string name, valueADT value) {
	Enter(variableTable, name, value);
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

static valueADT EvalIfExp(expADT exp, environmentADT env) {
	char op;
	valueADT  thenSum, elseSum;
	int lhs, rhs;

	op = GetIfRelOp(exp);
	lhs = GetIntValue(Eval(GetIfLHSExpression(exp), env));
	rhs = GetIntValue(Eval(GetIfRHSExpression(exp), env));
	thenSum = (Eval(GetIfThenPart(exp), env));
	elseSum = (Eval(GetIfElsePart(exp), env));

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