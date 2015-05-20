#include "print.h"

void printExp(expADT expression) {
	switch (ExpType(expression)) {
	case FuncExp: 
		printf("FuncExp: %s\n", GetFuncFormalArg(expression));
		printExp(GetFuncBody(expression));
		break;
	case IfExp:
		printExp(GetIfLHSExpression(expression));
		printExp(GetIfRHSExpression(expression));
		printf("Operator? %c\n", GetIfRelOp(expression));
		printExp(GetIfThenPart(expression));
		printExp(GetIfElsePart(expression));
		break;
	case CallExp:
		printExp(GetCallExp(expression));
		printExp(GetCallActualArg(expression));
		break;
	case ConstExp:
		printf("ConstExp: %d\n", ExpInteger(expression));
		break;
	case IdentifierExp:
		printf("IdentifierExp: %s\n", ExpIdentifier(expression));
		break;
	case CompoundExp:
		printExp(ExpLHS(expression));
		printf("Operator: %c\n", ExpOperator(expression));
		printExp(ExpRHS(expression));
		break;
	}
};