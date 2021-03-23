/**@<tokens.h>::**/
/* 
* This file contains all tokens used in this modified Pascal,
* such as ID, UINT, FLOAT, and so on
* referenced as numbers from 1024 to 1032
 */

/* token values referenced as numbers: */
enum {
	ID = 1024,
	UINT,
	FLOAT,
	OCT,
	HEX,
	ASGN, /* ASGN = ":=" (pascal)*/
	GEQ, /* GEQ = '>=' */
	LEQ, /* GEQ = '<=' */
	NEQ, /* NEQ = '<>=' */
};
