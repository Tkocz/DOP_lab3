
/*Main.c*/
#include "env.h"
#include "eval.h"
#include "exp.h"
#include "parser.h"
#include "print.h"
#include "value.h"
#include "symtab.h"
#include "scanadt.h"
#include "exception.h"
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(){
	scannerADT scanner;
	expADT exp;
	string line;
	valueADT value;

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
			printExp(exp);
			value = Eval(exp, NewEnvironment());
			printf("%d\n", value);
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}