/* Local function declarations */
static void InitCommandTable(void);
static void DefineCommand(string cmd, commandFnT fn);
static void ExecuteCommand(string cmd);
static void ClearCmd(void);
static void RunCmd(void);
static void HelpCmd(void);
static void QuitCmd(void);

18
/* Local function declarations */
static void InitCommandTable(void);
static void DefineCommand(string cmd, commandFnT fn);
static void ExecuteCommand(string cmd);
static void ClearCmd(void);
static void RunCmd(void);
static void HelpCmd(void);
static void QuitCmd(void);
/* Main test program */
main()
{
	string line;
	InitCommandTable();
	while (TRUE) {
		printf("-> ");
		line = GetLine();
		ExecuteCommand(line);
		if (StringEqual(line, "quit")) break;
	}
}
Här är våra fyra funktioner
som skall kopplas till
kommandon
static void InitCommandTable(void)
{
	commandTable = NewSymbolTable();
	DefineCommand("clear", ClearCmd);
	DefineCommand("run", RunCmd);
	DefineCommand("help", HelpCmd);
	DefineCommand("quit", QuitCmd);
}
static void DefineCommand(string cmd, commandFnT fn)
{
	commandEntryT entry;
	entry = New(commandEntryT);
	entry->fn = fn;
	Enter(commandTable, cmd, entry);
}
static void ExecuteCommand(string cmd)
{
	commandEntryT entry;
	entry = Lookup(commandTable, cmd);
	if (entry == UNDEFINED) {
		printf("Undefined command: %s\n", cmd);
		return;
	}
	entry->fn();
}

/* Command dispatch functions */
static void ClearCmd(void)
{
	printf("Command = clear\n");
}
static void RunCmd(void)
{
	printf("Command = run\n");
}
static void HelpCmd(void)
{
	printf("Command = help\n");
}
static void QuitCmd(void)
{
	printf("Command = quit\n");
}