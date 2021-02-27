/**@<parser.h>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
void T(void);
void F(void);
void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern FILE *source;
