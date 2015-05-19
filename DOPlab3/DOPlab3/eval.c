#include "eval.h"

/*
* Function: Eval
* Usage: n = Eval(exp, env);
* ------------------------
* This function evaluates an expression in the given environment and returns its result.
*/

valueADT Eval(expADT exp, environmentADT env) {
	switch (ExpType(exp)) {
	case FuncExp:
		printf("FuncExp: %s\n", GetFuncFormalArg(exp));
		printExp(GetFuncBody(exp));
		break;
	case IfExp:
		printExp(GetIfLHSExpression(exp));
		printExp(GetIfRHSExpression(exp));
		printf("Operator? %c\n", GetIfRelOp(exp));
		printExp(GetIfThenPart(exp));
		printExp(GetIfElsePart(exp));
		break;
	case CallExp:
		printExp(GetCallExp(exp));
		printExp(GetCallActualArg(exp));
		break;
	case ConstExp:
		printf("ConstExp: %d\n", ExpInteger(exp));
		break;
	case IdentifierExp:
		printf("IdentifierExp: %s\n", ExpIdentifier(exp));
		break;
	case CompoundExp:
		printExp(ExpLHS(exp));
		printf("Operator: %c\n", ExpOperator(exp));
		printExp(ExpRHS(exp));
		break;
	}
}