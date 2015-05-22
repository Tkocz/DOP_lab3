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
 * Function: Evaluating
 * Usage: n = Evaluating(exp);
 * ------------------------
 * This function evaluates an expression and returns its value.
 */

valueADT Evaluating(expADT exp, environmentADT env);

#endif
