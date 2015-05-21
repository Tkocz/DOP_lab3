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
void loadfile(string filename);

static scannerADT scanner;

main(){
	expADT exp;
	string line, token, command, variable;
	valueADT value;
	environmentADT env;

	env = NewEnvironment();

	InitVariableTable();
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
				value = GetIntValue(Eval(exp, env));
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
static void LoadCmd(environmentADT env)
{
	string filename = "";
	while (MoreTokensExist(scanner))
		filename = Concat(filename, ReadToken(scanner));
	printf("Command = load\n");
	loadfile(filename);
}
static void DefineCmd(environmentADT env) {
	string token;
	expADT definition;
	printf("Command = define\n");
	if (MoreTokensExist(scanner)) {
		token = ReadToken(scanner);
		if (StringEqual(ReadToken(scanner), "=")) {
			definition = ParseExp(scanner);
			switch (ExpType(definition)) {
			case FuncExp:
				printf("function!\n");
				printf("argument: %s\n", GetFuncFormalArg(definition));
				printExp(GetFuncBody(definition));
				SetIdValue(token, NewFuncValue(GetFuncFormalArg(definition), GetFuncBody(definition), env));
				break;
			default:
				SetIdValue(token, NewFuncValue("", ExpIdentifier(definition), env));
			}
		}
		else Error("Cannot load definition: %s\n", token);
	}
	else Error("Empty definition");

}
static void HelpCmd(environmentADT env)
{
	printf("Command = help\n");
}
static void QuitCmd(environmentADT env)
{
	exit(0);
}
void loadfile(string filename){
	FILE *infile;
	string tempLine;
	infile = fopen(filename, "r");
	if (infile == NULL)
		Error("Cant open file");
	while (tempLine = ReadLine(infile)){
		if (!(tempLine[0] == '#')){
			printf("%s\n", tempLine);
			//lägg in riktig funktionalitet
		}
	}
	 
	fclose(infile);
}