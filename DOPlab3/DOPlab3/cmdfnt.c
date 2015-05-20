#include "cmdfnt.h"

/* Local function declarations */
void DefineCommand(string cmd, commandFnT fn);
void ExecuteCommand(string cmd);

static symtabADT commandTable;

void DefineCommand(string cmd, commandFnT fn)
{
	commandEntryT entry;
	entry = New(commandEntryT);
	entry->fn = fn;
	Enter(commandTable, cmd, entry);
}
void ExecuteCommand(string cmd)
{
	commandEntryT entry;
	entry = Lookup(commandTable, cmd);
	if (entry == UNDEFINED) {
		//printf("Undefined command: %s\n", cmd);
		return;
	}
	entry->fn();
}
void InitCommandTable(void)
{
	commandTable = NewSymbolTable();
}
