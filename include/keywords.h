/* 
* This file contains all keywords used in this modified Pascal,
* such as BEGIN, END, VAR and so on
* referenced as numbers from 10001 to 10023
 */
#pragma once
int iskeyword(const char *);

/* keyword values referenced as numbers: */
enum keywords{
    BEGIN = 10001,
    NOT,
    OR,
    AND,
    DIV,
    MOD,
    PROGRAM,
    PROCEDURE,
    FUNCTION,
    RETURN,
    VAR,
    INTEGER,
    REAL,
    DOUBLE,
    BOOLEAN,    
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    REPEAT,
    UNTIL,
    END,
};