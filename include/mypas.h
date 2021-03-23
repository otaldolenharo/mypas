/**@<mypas.h>::**/
/* 
* This file constains all libraries used in mypas.c
* It contains also gettoken() function and lookahead extern variable
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
int gettoken(FILE *);
void mypas(void);
extern int lookahead;