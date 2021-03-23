/* 
* This file contains the iscompat table values
* referenced as numbers from -1 to 5
 */
#pragma once
/* An ID cannot lentgh more than 32 algarisms */
#define MAXIDLEN 32
#define MAXSTBSIZE 0x10000

/* iscompat values referenced as numbers: */
enum {
	INCOMPTBL = -1,
	VOID,
	BOOL,
	INT32,
	INT64,
	FLT32,
	FLT64,
};