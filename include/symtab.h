#pragma once 

#include <string.h>
#include <stdio.h>

#include <constants.h>

typedef
struct __symtab__ {
	char symbol[MAXIDLEN+1];
	char offset[MAXIDLEN+1]; /* Representar variaveis de niveis diferentes */
	int type;
	int data_size;
	int lexical_level;
	int objtype;
	int transp_type;
}
SYMTAB;

//int symtab_rtrvtype(const char *, int);
int symtab_rtrvtype(const char *symbol, int lexical_level);
int symtab_lookup(const char *symbol, int lexical_level);
int symtab_append(const char *symbol, int lexical_level, int objtype, int transp_type) ;
void symtab_update_type(int start, int type);

extern SYMTAB symtab[MAXSTBSIZE];
extern int symtab_next_entry;
extern int semantic_error;
extern int symtab_entry;
