#pragma once
/**@<mypas.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
int gettoken(FILE *);
void mypas(void);
extern int lookahead;