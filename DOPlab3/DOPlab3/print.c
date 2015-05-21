#include "print.h"

void printExp(expADT expression) {
	switch (ExpType(expression)) {
	case FuncExp: 
		printf("FuncExp (%s) {\n", GetFuncFormalArg(expression));
		printExp(GetFuncBody(expression));
		printf("}\n");
		break;
	case IfExp:
		printf("if \n");
		printExp(GetIfLHSExpression(expression));
		printf("RelOp: %c\n", GetIfRelOp(expression));
		printExp(GetIfRHSExpression(expression));
		printf("then \n");
		printExp(GetIfThenPart(expression));
		printf("else \n");
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
		printf("CompoundExp: (\n");
		printExp(ExpLHS(expression));
		printf("Operator: %c\n", ExpOperator(expression));
		printExp(ExpRHS(expression));
		printf(")\n");
		break;
	}
};