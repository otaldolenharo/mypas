/**@<keywords.c>::**/

/**
 * This is the Keyword Handler of the MyPas Language Compiler 
 * it was developed by:
 * 
 * Daniel Hideki Kawabata
 * Gabriel Lenharo da Cunha
 * 
 * Supervised by Professor Eraldo Pereira Marinho
**/

#include <string.h>
#include <keywords.h>

/* There are all the keywords found in this modified Pascal */
char *keywords[] = {
    "begin",
    "not",
    "or",
    "and",
    "div",
    "mod",
    "program",
    "procedure",
    "function",
    "return",
    "var",
    "integer",
    "real",
    "double",
    "boolean",    
    "if",
    "then",
    "else",
    "while",
    "do",
    "repeat",
    "until",
    "end",
};

/* It verifies if the current keyword is in the keywords[] string */
int iskeyword(const char *name){
    int i;
    for(i = BEGIN; i < END + 1; i++){
        if(strcmp(keywords[i-BEGIN], name) == 0) return i;
    }
    return 0;
}
