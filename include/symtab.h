#pragma once 
/* 
* This file constains all libraries used in symtab.c
* It also contains the base structure of the symtab table
 */
#include <string.h>
#include <stdio.h>

#include <constants.h>

typedef
struct __symtab__ {
	char symbol[MAXIDLEN+1];
	char offset[MAXIDLEN+1]; /* To represent different variable levels */
	int type; 
	int data_size; /* Size in bytes */
	/* It describes which sintatic level it is, from 1(global) to n(local functions) */
	int lexical_level;
	/*** objtype = 1 => variable; = 2 => procedure; = 3 => function ***/
	int objtype; 
	/*** transp_type =1 => local storege; = 2 => passaged by value; = 3 => passaged by reference ***/
	int transp_type;
}
SYMTAB;

/* Function prototypes */
int symtab_rtrvtype(const char *, int);
int symtab_rtrvtype(const char *symbol, int lexical_level);
int symtab_lookup(const char *symbol, int lexical_level);
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type) ;
void symtab_update_type(int start, int type);

/* Extern variables */
extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
