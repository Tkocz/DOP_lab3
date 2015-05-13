/*Main.c*/
#include "env.h"
#include "eval.h"
#include "exp.h"
#include "parser.h"
#include "print.h"
#include "value.h"
#include "symtab.h"
#include "scanadt.h"
#include <stdio.h>
#include <stdlib.h>

main(){
	scannerADT scanner;
	expressionADT exp;
	string line;
	InitVariableTable();
	scanner = NewScanner();
	SetScannerSpaceOption(scanner, IgnoreSpaces);
	while (TRUE) {
		try {
			printf("=> ");
			line = GetLine();
			if (StringEqual(line, "quit")) exit(0);
			SetScannerString(scanner, line);
			exp = ParseExp(scanner);
			value = EvalExp(exp);
			printf("%d\n", value);
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}