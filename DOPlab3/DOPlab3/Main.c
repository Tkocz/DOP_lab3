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
#include "cmdfnt.h"

static void defineCommandTable();
void LoadCmd(environmentADT env);
void DefineCmd(environmentADT env);
void HelpCmd(environmentADT env);
void QuitCmd(environmentADT env);
void definer(scannerADT scanner, environmentADT env);
bool charExists(string text, char c);

static scannerADT scanner;

main(){
	expADT exp;
	string line, token, command, variable;
	valueADT value;
	environmentADT env;

	system("COLOR 0B");

	env = NewEnvironment();

	InitCommandTable();
	defineCommandTable();

	scanner = NewScanner();
	SetScannerSpaceOption(scanner, IgnoreSpaces);
	while (TRUE) {
		try {
			printf("=> ");
			line = GetLine();
			SetScannerString(scanner, line);
			token = ReadToken(scanner);

			if (token[0] == ':'){
				command = ReadToken(scanner);
				ExecuteCommand(command, env);
			}
			else{
				SaveToken(scanner, token);
				exp = ParseExp(scanner);
				printExp(exp);
				value = GetIntValue(Evaluating(exp, env));
				printf("%d\n", value);
			}
			except(ErrorException)
				printf("Error: %s\n", (string)GetExceptionValue());
		} endtry
	}
}

static void defineCommandTable() {
	DefineCommand("load", LoadCmd);
	DefineCommand("l", LoadCmd);
	DefineCommand("define", DefineCmd);
	DefineCommand("d", DefineCmd);
	DefineCommand("help", HelpCmd);
	DefineCommand("h", HelpCmd);
	DefineCommand("quit", QuitCmd);
	DefineCommand("q", QuitCmd);
}

/* Command dispatch functions */
static void LoadCmd(environmentADT env) {
	FILE *infile;
	string tempLine, contents;
	string filename = "";

	while (MoreTokensExist(scanner))
		filename = Concat(filename, ReadToken(scanner));
	printf("Command = load\n");

	infile = fopen(filename, "r");
	if (infile == NULL)
		Error("Cant open file");
	while (tempLine = ReadLine(infile)){
		if (!(tempLine[0] == '#' || StringEqual(tempLine, ""))){
			if(charExists(tempLine, '{')) {
				contents = "";
				while (!charExists(tempLine, '}')) {
					tempLine = Concat(" ", tempLine);
					contents = Concat(contents, tempLine);
					tempLine = ReadLine(infile);
				}
				tempLine = Concat(" ", tempLine);
				contents = Concat(contents, tempLine);
				definer(contents, env);
			}
			else{
				definer(tempLine, env);
			}
		}
	}
	fclose(infile);
}
static void DefineCmd(environmentADT env) {
	string expression = "";
	printf("Command = define\n");
	while(MoreTokensExist(scanner)) {
		expression = Concat(expression, ReadToken(scanner));
	}
	definer(expression, env);
}


static void HelpCmd(environmentADT env) {
	printf("Available commands\n");
	printf("\t :load <file>          Load a file with definitions.\n");
	printf("\t :define <id >= <exp.> Define a identifier.\n");
	printf("\t :help                 Display this text.\n");
	printf("\t :quit                 Exit the interpreter.\n");
}

static void QuitCmd(environmentADT env) {
	exit(0);
}

void definer(string expressionLine, environmentADT env) {
	string token,id;
	expADT definition;
	scannerADT defScanner;

	defScanner = NewScanner();

	SetScannerString(defScanner, expressionLine);
	SetScannerSpaceOption(defScanner, IgnoreSpaces);

	id = ReadToken(defScanner);
	if (StringEqual(ReadToken(defScanner), "=")== TRUE) {
		token = ReadToken(defScanner);
		SaveToken(defScanner, token);
		definition = ParseExp(defScanner);
	}
	else {
		Error("Expecting '=' in definition\n");
	}
	if (MoreTokensExist(defScanner))
		Error("Token: %s in definition unexpected", ReadToken(defScanner));

	DefineIdentifier(env, id, definition, env);
}

bool charExists(string text, char c) {
	int i, textLength;
	textLength = StringLength(text);
	for(i=0; i<textLength; i++) {
		if(text[i] == c)
			return TRUE;
	}
	return FALSE;
}