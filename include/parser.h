/**@<parser.h>::**/
/* 
* This file constains all libraries used in parser.c
* It constains all function protopytes from parser.c
* It also contains extern variables and extern FILES
 */
#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <symtab.h>
#include <pseudocode.h>

/* Funcion prototypes */
void declarative(void);
void imperative(void);
void vardecl(void);
void sbpdecl(void);
void varlist(void);
int typemod(void);
void sbphead(void);
void sbptail(void);
void formparm(void);
void stmt(void);
void ifstmt(void);
void whlstmt(void);
void rptstmt(void);

void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern char lexeme[];
/* FILE with program object */
extern FILE *source;
int expr(int);
int smpexpr(int);
int term(int);
int fact(int);

/* Variables used to error treatment */
extern int linecounter;
extern int columcounter;