/**@<pseudocode.c>::**/

/**
 * This is the Intermediate Pseudocode Generator of the MyPas Language Compiler 
 * it was developed by:
 * 
 * Daniel Hideki Kawabata
 * Gabriel Lenharo da Cunha
 * 
 * Supervised by Professor Eraldo Pereira Marinho
**/

#include <stdio.h>
#include <types.h>
#include <tokens.h>

/**
 * Negates the content of accumulator register
**/
void negate(int type)
{
    switch(type) { 
        case BOOL:
            printf("\tnotb accb");
            break;
        case INT32:
            printf("\tnegl accl");
            break;
        case INT64:
            printf("\tnegq accq");
            break;
        case FLT32:
            printf("\tnegf accf");
            break;
        case FLT64:
            printf("\tnegdf accdf");
            break;
        default:
            ;
    }
}

/**
 * Moves the content between two addresses in pseudocode.
 * 
 * Usually used in assignment
**/
void move(int type, const char *src, const char *dest)
{
    switch(type) { 
        case BOOL:
            printf("\tmovb %s, %s\n", src, dest);
            break;
        case INT32:
            printf("\tmovl %s, %s\n", src, dest);
            break;
        case INT64:
            printf("\tmovq %s, %s\n", src, dest);
            break;
        case FLT32:
            printf("\tmovf %s, %s\n", src, dest);
            break;
        case FLT64:
            printf("\tmovdf %s, %s\n", src, dest);
            break;
        default:
            ;
    }
}

/**
 * Pushes the value to the stack
**/
void push(int type)
{
    switch(type) { 
        case BOOL:
            printf("\tpushb accb\n");
            break;
        case INT32:
            printf("\tpushl accl\n");
            break;
        case INT64:
            printf("\tpushq accq\n");
            break;
        case FLT32:
            printf("\tpushf accf\n");
            break;
        case FLT64:
            printf("\tpushdf accdf\n");
            break;
        default:
            ;
    }
}

/**
 * Adds the content of aux register to the accumulator register
**/
void add(int type)
{
    switch(type) { 
        case INT32:
            printf("\taddl auxl, accl\n");
            break;
        case INT64:
            printf("\taddq auxq, accq\n");
            break;
        case FLT32:
            printf("\taddf auxf, accf\n");
            break;
        case FLT64:
            printf("\tadddf auxdf, accdf\n");
            break;
        default:
            ;
    }
}

/**
 * Subtracts the content of aux register to the accumulator register
**/
void sub(int type)
{
    switch(type) { 
        case INT32:
            printf("\tsubl auxl, accl\n");
            break;
        case INT64:
            printf("\tsubq auxq, accq\n");
            break;
        case FLT32:
            printf("\tsubf, auxf, accf\n");
            break;
        case FLT64:
            printf("\tsubdf auxdf, accdf\n");
            break;
        default:
            ;
    }
}

/**
 * Multiply the content of aux register by the content accumulator register
**/
void mul(int type)
{
    switch(type) { 
        case INT32:
            printf("\tmul auxl\n");
            break;
        case INT64:
            printf("\tmulq auxlq\n");
            break;
        case FLT32:
            printf("\tmulf auxlf\n");
            break;
        case FLT64:
            printf("\tmuldf auxldf\n");
            break;
        default:
            ;
    }
}

/**
 * Divides the content of aux register by the content of accumulator register
**/
void divl(int type)
{
    switch(type) { 
        case INT32:
            printf("\tdivl auxl\n");
            break;
        case INT64:
            printf("\tdivl auxq\n");
            break;
        case FLT32:
            printf("\tdivl auxf\n");
            break;
        case FLT64:
            printf("\tdivl auxl\n");
            break;
        default:
            ;
    }
}

/**
 * Compares the content of *aux and *acc
**/
void cmp(int relop, int type, char *aux, char *acc) {

    char *suffix, *instr;
    switch(type) { 
        case BOOL:
            suffix = "b";
            break;
        case INT32:
            suffix = "l";
            break;
        case INT64:
            suffix = "q";
            break;
        case FLT32:
            suffix = "f";
            break;
        case FLT64:
            suffix = "df";
            break;
        default:
            ;
    }
    switch (relop){
    
        case '>':
            instr = "gt";
            break;
        case '<':
            instr = "lt";
            break;
        case '=':
            instr = "eq";
            break;
        case NEQ:
            instr = "neq";
            break;
        case LEQ:
            instr = "leq";
            break;
        case GEQ:
            instr = "geq";
            break;
        default:
            ;
    }

    printf("\t%s%s %s%s, %s%s\n", instr,suffix, aux,suffix, acc,suffix);
}

/**
 * Makes the program go to some label in case of 
 * boolean comparison results in false
**/
void gofalse(int loopnumber) 
{
    printf("\tgofalse .L%d\n", loopnumber);
}

/**
 * Creates a label
**/
void mklabel(int loopnumber)
{
    printf(".L%d:\n", loopnumber);
}

/**
 * Forces the program go to some label
**/
void golabel(int looknumber) 
{
printf("\tgoto .L%d\n", looknumber);
}