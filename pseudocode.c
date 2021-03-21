#include <stdio.h>
#include <types.h>
#include <tokens.h>

//precisa mudar aqui
//onde aparece nome, é pra trabalhor com o offset da variavel ou índice da variavel na tabela de simbolos
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

void gofalse(int loopnumber) 
{
    printf("\tgofalse .L%d\n", loopnumber);
}

void mklabel(int loopnumber)
{
    printf(".L%d:\n", loopnumber);
}

void golabel(int looknumber) 
{
printf("\tgoto .L%d\n", looknumber);
}