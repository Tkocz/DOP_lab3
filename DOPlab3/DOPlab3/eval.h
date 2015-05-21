/*
 * File: eval.h
 * ------------
 * This file is the interface to the evaluator module, which
 * computes the value represented by an expression.  This module
 * also maintains the state of the variables, which are only
 * updated or inspected during the evaluation phase.
 */

#ifndef _eval_h
#define _eval_h

#include "exp.h"
#include "value.h"
#include "env.h"

/*
 * Function: Eval
 * Usage: n = Eval(exp);
 * ------------------------
 * This function evaluates an expression and returns its value.
 */

valueADT Eval(expADT exp, environmentADT env);

/*
 * Function: InitVariableTable
 * Usage: InitVariableTable();
 * ---------------------------
 * This function initializes the table used to hold the variables.
 * All variables are initially undefined.
 */

void InitVariableTable(void);

/*
 * Function: GetIdValue
 * Usage: value = GetIdValue(name);
 * ----------------------------------------
 * This function returns the value of an identifier.
 */

valueADT GetIdValue(string name);

/*
 * Function: SetIdValue
 * Usage: SetIdValue(name, value);
 * ---------------------------------------
 * This function sets the identifier to have the specified value.
 */

void SetIdValue(string name, valueADT value);

#endif
