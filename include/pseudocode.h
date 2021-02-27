#pragma once
void not(void);
void negate(int type);
void move(int type, const char *src, const char *dest);
void push(int type);
void add(int type);
void sub(int type);
void mul(int type);
void divl(int type);
void gofalse(int loopnumber);
void mklabel(int loopnumber);
void golabel(int looknumber);
