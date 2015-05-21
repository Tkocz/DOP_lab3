/*
* File: env.h
* ------------
* This file is the interface to the environment module, which
* represents the environment (set of definitions) available
* in a certain lexical scope in MFL.
*/

#ifndef _cmdfnt_h
#define _cmdfnt_h

#include "strlib.h"
#include <stdio.h>
#include "genlib.h"
#include "symtab.h"
#include "env.h"

/*
* Type: commandFnT
* ----------------
* This type defines the class of command functions.
*/
typedef void(*commandFnT)(void);
/*
* Type: commandEntryT
* -------------------
* This type consists of a structure containing only a command
* function. This extra level of structure is required because
* function pointers are not compatible with void * in ANSI C.
*/
typedef struct {
	commandFnT fn;
} *commandEntryT;
/*
* Private variable: commandTable
* ------------------------------
* The entries in this table are used to hold the commands and
* their corresponding actions.
*/
static symtabADT commandTable;

void InitCommandTable(void);
void ExecuteCommand(string cmd, environmentADT env);

#endif