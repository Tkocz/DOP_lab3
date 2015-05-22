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


/* static variables */
static int numberOfRecursion = 0;


/* Private function prototypes */

static valueADT EvalCompound(expADT exp, environmentADT env);
static valueADT EvalIfExp(expADT exp, environmentADT env);
static valueADT evalFunc(expADT exp, environmentADT env);
static valueADT evalCall(expADT exp, environmentADT env);

/* Exported entries */


valueADT Evaluating(expADT exp, environmentADT env){
	valueADT result;

	numberOfRecursion = 0;

	return (Eval(exp, env));
}

valueADT Eval(expADT exp, environmentADT env){

	if (numberOfRecursion > 1000){
		Error("Stack overflow.Too deep recursion.");
	}
	numberOfRecursion = numberOfRecursion + 1;

	switch (ExpType(exp)) {
	case ConstExp: {
		return (NewIntegerValue(ExpInteger(exp)));
	}
	case IdentifierExp: {
		return (GetIdValue(ExpIdentifier(exp),env));
	}
	case CompoundExp:
		return (EvalCompound(exp, env));
	case FuncExp:
		return (evalFunc(exp,NewClosure(env)));
	case IfExp:
		return (EvalIfExp(exp, NewClosure(env)));
	case CallExp:
		return (evalCall(exp,NewClosure(env)));
	default:
		Error("Unidentified Evaluation");
	}
}

void InitVariableTable(void)
{
	variableTable = NewSymbolTable();
}

valueADT GetIdValue(string name,environmentADT env) {
	valueADT ip;

	ip = GetIdentifierValue(env, name);
	return (Eval(GetFuncValueBody(ip),(environmentADT)GetFuncValueClosure(ip)));
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

static valueADT evalCall(expADT exp, environmentADT env){
	string body;
	expADT arg ,func;
	valueADT funcValue;

	func = GetCallExp(exp);
	arg = GetCallActualArg(exp);
	funcValue = Eval(func, env);
	/*
	if (ValueType(funcValue) != funcValue){
		Error("Illigal type");
	}*/
	body = GetFuncValueFormalArg(funcValue);
	DefineIdentifier(env, body, arg, env);

	return Eval(GetFuncValueBody(funcValue), env);
}

static valueADT evalFunc(expADT exp, environmentADT env){
	string arg;
	expADT body;

	arg = GetFuncFormalArg(exp);
	body = GetFuncBody(exp);

	return NewFuncValue(arg, body, env);
}