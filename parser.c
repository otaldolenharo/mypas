/**@<parser.c>::**/

/**
 * This is the Syntactic Analyzer of the MyPas Language Compiler 
 * it was developed by:
 * 
 * Daniel Hideki Kawabata
 * Gabriel Lenharo da Cunha
 * 
 * Supervised by Professor Eraldo Pereira Marinho
**/

#include <parser.h>

/** iscompat table: **/
/***************************************************************************************************
 *        bool   int32   int64   flt32   flt64     '+'     '-'     '*'     '/'    NOT    OR    AND
 * bool   bool   -----   -----   -----   -----    -----   -----   -----   -----  bool   bool   bool
 * int32  ----   int32   int64   flt32   flt64    int32   int32   int32   int32  ----   ----   ----
 * int64  ----   int64   int64   flt32   flt64    int64   int64   int64   int64  ----   ----   ----
 * flt32  ----   flt32   flt32   flt32   flt64    flt32   flt32   flt32   flt32  ----   ----   ----
 * flt64  ----   flt64   flt64   flt64   flt64    flt64   flt64   flt64   flt64  ----   ----   ----
 ***************************************************************************************************/
/***
 * enum {
 *	INCOMPTBL = -1,
 *	VOID,
 *	BOOL,
 *	INT32,
 *	INT64,
 *	FLT32,
 *	FLT64,
 * };
 ***/
/* It references iscompat table at line 5 */
int iscompat(int acc_type, int syn_type)
{
	/* 
	 * It must find what value is acc_type(bool, int32, and so on)
	 * and then verifies what syn_type is about (its AND, OR, bigger than int32, and so on)
	 */
    switch (acc_type) {
        case VOID:
            return syn_type;
        case BOOL:
            if (syn_type == AND || syn_type == OR || syn_type == NOT || syn_type == BOOL) return BOOL;
            break;
        case INT32:
            if (syn_type >= INT32 && syn_type <= FLT64) return syn_type;
            break;
        case FLT32:
            if (syn_type > FLT32 && syn_type <= FLT64) return syn_type;
            if (syn_type == INT32) return FLT32;
            break;
        case FLT64:
            if (syn_type >= INT32 && syn_type <= FLT64) return FLT64;
    }
    if (acc_type >= INT32 && acc_type <= FLT64) {
        switch(syn_type) {
            case'+':case'-':case'*':case'/':case DIV:case MOD:
                return acc_type;
        }
    }
	/* Returns incompatible is none of other requirements were acquired */
    return INCOMPTBL;
}

/*****************************************************************************
 * mypas -> PROGRAM ID ; declarative imperative .
 *****************************************************************************/
/* Lexicallevel shows on which semantic level it is */
/**/ int lexicallevel = 0; /**/
/*** objtype = 1 => variable; = 2 => procedure; = 3 => function ***/
/**/ int objtype; /**/ 
/*** transp_type =1 => local storege; = 2 => passaged by value; = 3 => passaged by reference ***/
/**/ int transp_type; /**/

/* MAIN PROGRAM STARTS HERE */
void mypas(void) 
{ 
	/* Every .pas program must start as 'PROGRAM' followed by an 'ID' name, followed by ';' */
	match(PROGRAM); 
	match(ID); 
	match(';'); 
	/* Entering program, thus it must increased lexical level*/
	/**/ lexicallevel++; /**/
	declarative();
	imperative(); 
	/* Exiting program, thus it must decrease lexical level */
	/**/ lexicallevel--; /**/ 
	/* Every .pas program must end with a dot('.') */
	match ('.');
}
/*****************************************************************************
 * declarative -> vardecl sbpdecl
 *****************************************************************************/
void declarative(void) 
{ 
	vardecl(); 
	sbpdecl(); 
}
/*****************************************************************************
 * vardecl ->  [ VAR varlist : typemod ; { varlist : typemod ; } ]
 *****************************************************************************/
void vardecl(void)
{
	if (lookahead == VAR ) {
		match (VAR);
		/**/int first_pos;/**/
		/**/ objtype = transp_type = 1; //it's a variable /**/
		_varlist_head:
		/**/first_pos = symtab_next_entry;/**/
		/* varlist() wil append in symtab table the current variable*/
		varlist();
		match(':');
		/**/int type = /**/typemod(); 
		/**/symtab_update_type(first_pos, type);/**/
		match(';'); 
		/* If there are other variables(IDs), it repeats append-update process */
		if (lookahead == ID) { goto _varlist_head; }
	} else {
		/* Do nothing otherwise */
		;
	}
}
/*****************************************************************************
 * varlist -> ID { , ID }
 *****************************************************************************/
/* semantic error counter (fact function -> symtab.c)*/
/**/int semantic_error = 0;/**/
void varlist(void)
{
	_match_id_head:
	/* It append(add) the current variable to symtab table */
	/**/symtab_append(lexeme,lexicallevel, objtype, transp_type);/**/
	match(ID);
	/* It keeps going while next caracter is ',' (var i, j, n: integer;) */
	if (lookahead == ',') { match(','); goto _match_id_head; }
}
/*****************************************************************************
 * typemod -> BOOLEAN | INTEGER | REAL | DOUBLE
 *****************************************************************************/
int typemod(void)
{
	/**/int type;/**/
	/* It verifies what is the current type (INT32, INT64, FLT32, FLT64, BOOL) */
	switch (lookahead) {
		case INTEGER:
			/**/type = INT32;/**/
			break;
		case REAL:
			/**/type = FLT32;/**/
			break;
		case DOUBLE:
			/**/type = FLT64;/**/
			break;
		default:
			/**/type = BOOL;/**/
	}
	match(lookahead);
	/* Type returns with it's value verified */
	return type;
}
/*****************************************************************************
 * sbpdecl -> { PROCEDURE sbphead ; sbptail | FUNCTION sbphead : typemod ; sbptail }
 *****************************************************************************/
void sbpdecl(void)
{
	/**/int isfunc = 0;/**/
	/**/ char sbpname[MAXIDLEN+1];/**/
	/* It'll store the next entry of symtab, to later be resumed from where it stopped */
	/**/int symtab_sentinel; /**/
	_switch_head:
	/* It must verify in what level it is (procedure, function or variable) */
	switch (lookahead) {
		/* Function level */
		case FUNCTION:
			isfunc = 1;
		/* Procedure level */
		case PROCEDURE:
			/**/ objtype = 2 + isfunc; /**/
			match(lookahead);
			/**/ strcpy(sbpname, lexeme); /**/
			/**/int sbppos = symtab_append(sbpname,lexicallevel, objtype, transp_type); /**/
			match(ID); 
			/**/ symtab_sentinel = symtab_next_entry; /**/
			/* It entered a procedure, thus lexical level is increased */
			/**/ lexicallevel++; /**/ 
			formparm(); 
			if (isfunc) {
				isfunc = 0;
				match(':'); 
				/**/ int rettype = /**/typemod();
				/**/ symtab[sbppos].type = rettype; /**/
			} else {
				/**/ symtab[sbppos].type = VOID; /**/
			}
			/* A function ended, thus lexical level is decreased */
			/**/ lexicallevel--; /**/
			match(';');
			declarative(); 
			imperative(); 
			/* It must go back to the begining of the symtab table */
			/**/symtab_next_entry = symtab_sentinel; /**/
			match(';'); 
			/* While there are more functions/procedures, it keeps going */
			if (lookahead == FUNCTION || lookahead == PROCEDURE) { goto _switch_head; }
		default:
		/* It does nothing otherwise */
			;
	}
}
/*****************************************************************************
 * formparm -> [ ( [VAR] varlist : typemod { ; [VAR] varlist : typemod } ) ]
 *****************************************************************************/
void formparm(void)
{
	/* When a '(' appears, a new expression may exist */
	if (lookahead == '(') {
		match('(');
		/**/int first_pos;/**/
		/**/objtype = 1; /**/
		parm_list:
		/* first_pos saves the symtab table top */
		/**/first_pos = symtab_next_entry;/**/
		if (lookahead == VAR) { 
			match(VAR); 
			/**/ transp_type = 3; //Passage by reference/**/
		} else {
			/**/ transp_type = 2; //Passage by parameter/**/
		}
		/* varlist() wil append in symtab table the current variable*/
		varlist();
		match(':');
		/**/int type = /**/typemod();
		/**/ symtab_update_type(first_pos, type); /**/
		/* It keeps going while there are ';' */
		if (lookahead == ';') { match(';'); goto parm_list; }
		match(')');
	} else {
		;
	}
}
/*****************************************************************************
 * imperative -> BEGIN stmt { ; stmt } END
 *****************************************************************************/
void imperative(void)
{
	/* Imperative functions must start with 'BEGIN' and end with 'END' */
	match(BEGIN);
	stmt_list:
	stmt();
	/* It keeps going while there are ';' */
	if (lookahead == ';') { match(';'); goto stmt_list; }
	match(END);
}
/*****************************************************************************
 * stmt -> imperative | ifstmt | whlstmt | rptstmt | fact | <empty>
 *****************************************************************************/
/* Return expression (from modified Pascal) */
void rtrn(void)
{
    match(RETURN); 
	/* It calls a VOID expression */
    expr(VOID);
}
void stmt(void)
{
	/**/int fact_type;/**/
	/* 
	 * It verifies if the current statement is
	 * imperative, if, while, repeatm, ID, BOOL, UINT or FLOAT 
	 */
	switch (lookahead) {
		case BEGIN:
			imperative(); break;
		case IF:
			ifstmt(); break;
		case WHILE:
			whlstmt(); break;
		case REPEAT:
			rptstmt(); break;
		case '(':
		case ID:
		case BOOL:
		case UINT:
		case FLOAT:
			/**/fact_type = /**/fact(VOID); break;
		default:
			/* Do nothing otherwise */
			;
	}
}
/*****************************************************************************
 * ifstmt -> IF expr THEN stmt [ ELSE stmt ]
 *****************************************************************************/
/* loop_count will help to know the respective 'else' for each 'if' */
/**/int loop_count = 1;/**/
void ifstmt(void)
{
    /**/int expr_type, else_count, endif_count;/**/
    match(IF); 
	/* It must be BOOL */
    /**/expr_type = /**/expr(BOOL); 
    match(THEN);
	/* gofalse if the number of ifs and elses are equal */
    /**/gofalse(else_count = endif_count = loop_count++);/**/
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        /**/
        golabel(endif_count = loop_count++); 
        mklabel(else_count);
        /**/
        stmt();
    }
    /**/mklabel(endif_count);/**/
}
/*****************************************************************************
 * whlstmt -> WHILE expr DO stmt
 *****************************************************************************/
void whlstmt(void)
{
	/**/int expr_type, whlhead, whltail;/**/
	match(WHILE);
	
	/**/mklabel(whlhead = loop_count++);/**/
	/**/expr_type = /**/expr(BOOL);
	match(DO);
	/* It exits when while tail is equal to loop_count */
	/**/gofalse(whltail = loop_count++);/**/
	stmt();
	/**/golabel(whlhead);/**/
	/**/mklabel(whltail);/**/
}
/*****************************************************************************
 * rptstmt -> REPEAT stmt { ; stmt } UNTIL expr
 *****************************************************************************/
void rptstmt(void)
{
	/**/int expr_type; int replbl;/**/
	/**/printf(".L%d:\n", replbl = loop_count++); /**/
	stmt_head:
	stmt();
	/* It keeps executing stmt() while there are ';' */
	if (lookahead == ';') { match(';'); goto stmt_head; }
	match(UNTIL);
	/**/expr_type = /**/expr(BOOL);
	/**/printf("\tgofalse .L%d:\n", replbl); /**/
}
/* expr -> smpexpr [ relop smpexpr ] */
int isrelop(void)
{
	/* It's verefied which relational operand is */
	switch(lookahead) {
		case '=':
		case '>':
		case '<':
		case GEQ:
		case NEQ:
		case LEQ:
			return lookahead;
	}
	/* returns 0 otherwise */
	return 0;
}

/* expr -> smpexpr [ relop smpexpr ] */
int expr(int expr_type)
{
	/**/int relop; int left_type, right_type;/**/
	/**/left_type = /**/smpexpr(VOID);

	/* What relational operand is? */
	if (isrelop()) {
		/**/relop = lookahead;/**/
		/* It's INCOMPATIBLE is it's not BOOL type */
		/**/if (expr_type != BOOL) expr_type = INCOMPTBL;/**/

		match(lookahead);

		/**/right_type = /**/smpexpr(left_type);
		/**/left_type = iscompat(left_type, right_type);/**/
		/**/if (left_type > 0) {
				cmp(relop, left_type, "aux", "acc");
				return expr_type;
			}/**/
	} else {
		/**/expr_type = iscompat(expr_type, left_type);/**/
	}
	/**/return expr_type;/**/
}
/* smpexpr -> ['+''-'] term { (+) term } */
int smpexpr(int smpexpr_type) 
{
	/**/int signal = 0;/**/
	/* First operand will say if its a positive or negative value */
	if (lookahead == '+' || lookahead == '-' || lookahead == NOT) {
		/**/signal = lookahead;/**/
		/***/smpexpr_type = iscompat(smpexpr_type, signal);/***/

		match(lookahead);
	}

	/***/int term_type = /***/ term(smpexpr_type); 
	/**/ smpexpr_type = iscompat(smpexpr_type, term_type); /**/
	/* The expression type must be changed if it stated with '-' or NOT */
	/**/if (signal == '-' || signal == NOT) negate(smpexpr_type);/**/
	/***/smpexpr_type = iscompat(smpexpr_type, term_type);/***/

	/* Is the operand within '+' or '-' or 'OR'? */
	while ( lookahead == '+' || lookahead == '-' || lookahead == OR) {
		/**/int oplus = lookahead;/**/
		/***/smpexpr_type = iscompat(smpexpr_type, oplus);/***/
		/**/push(smpexpr_type);/**/

	 	match (lookahead); /***/term_type = /***/ term(smpexpr_type);

		/***/smpexpr_type = iscompat(smpexpr_type, term_type);/***/
		/**/move(smpexpr_type, "acc", "aux");/**/

		/**/
		/* Its add, otherwise its sub */
		switch(oplus) {
		case '+':
			add(smpexpr_type);
			break;
		default:
			sub(smpexpr_type);
		}
		/**/
	}

	/***/return smpexpr_type;/***/
}

/* term -> fact { (*) fact } */
int term(int term_type)
{ 
	/***/int fact_type = /***/fact(term_type); 
	/**/ term_type = iscompat(term_type, fact_type); /**/
	
	/* otimes recieves the operand withing '{}' */
	while ( lookahead == '*' || lookahead == '/' || lookahead == DIV || lookahead == MOD || lookahead == AND) {
		/**/int otimes = lookahead;/**/
		/***/term_type = iscompat(term_type, otimes);/***/
		/**/push(term_type);/**/

		match (lookahead); /***/fact_type = /***/fact(term_type);

		/***/term_type = iscompat(term_type, fact_type);/***/

		/**/move(term_type, "acc", "aux");/**/

		/**/
		/* Its mul, otherwise its div */
		switch(otimes) {
			case '*':
				mul(term_type);
				break;
			default:
				divl(term_type);
		}
		/**/
	}

	/***/return term_type;/***/
}

/*  fact ->  ( expr )
 *       | n
 *       | v [ = expr ]
 */
int fact(int fact_type) 
{
    /**/char name[MAXIDLEN+1];/**/ 
    /***/int expr_type;/***/

    /* It could be '(', any constant type or an ID */ 
    switch (lookahead) {
        case '(':
            match('('); /***/expr_type = /***/expr(fact_type); match(')');
            /***/fact_type = iscompat(fact_type, expr_type);/***/
            break;
        case BOOL:
            /***/fact_type = iscompat(fact_type, BOOL);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(BOOL);
            break;
        case UINT:
            /***/fact_type = iscompat(fact_type, INT32);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(UINT);
            break;
        case FLOAT:
            /***/fact_type = iscompat(fact_type, FLT32);/***/
            /**/move(fact_type, lexeme, "acc");/**/
            
            match(FLOAT);
            break;
        default:
            /**/strcpy(name, lexeme);/**/
            /* When it's an ID, it must be verified it's assignment */
            match(ID);
            if (lookahead == ASGN) {
                /**** L-Value ****/
                match(ASGN); /***/expr_type = /***/expr(fact_type);
                /**/
                if ( symtab_lookup(name, lexicallevel) < 0 ) {
                    fprintf(stderr, "%s undeclared\n", name);
                    semantic_error++;
                } else {
                    if (symtab[symtab_entry].objtype != 1) {
                        fprintf(stderr, "%s cannot be an L-Value\n", name);
                        semantic_error++;
                    } else {
                        fact_type = iscompat(expr_type, symtab[symtab_entry].type);
                        /*** variable entry in symbol table is set in symtab_entry ***/
                        move(fact_type, "acc", symtab[symtab_entry].offset);                        
                    }
                }
                /**/
            } else {
                /**** R-Value ****/
                /**/
				/* 
				 * If it's name it's not on symtab table or lexical level is incompatible,
				 * a semantic error ocours 
				 */
                if ( symtab_lookup(name, lexicallevel) < 0 ) {
                    fprintf(stderr, "%s undeclared\n", name);
                    semantic_error++;
                } else {
					/* It's name was found in symtab table */
                    switch(symtab[symtab_entry].objtype) {
						/* variable case */
                        case 1:
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
						/* procedure case */
                        case 3:
							/* 
							 * a procedure starts with an ID, followed by '('
							 * and it finishes with ')' 
							 */
                            if (lookahead == '(') {
                                match('(');
                                _expr_list:
                                expr(VOID);
								/* It keeps going while there are ',' */
                                if (lookahead == ',') { match(','); goto _expr_list; }
                                match(')');
                            }
                            fact_type = iscompat(fact_type, symtab[symtab_entry].type);
                            /*** variable entry in symbol table is set in symtab_entry ***/
                            move(fact_type, symtab[symtab_entry].offset, "acc");
                            break;
                    }
                }
                /**/
            }
    }
    return fact_type;
}

/* 
 * TO ALGARISM funcion: it's used to treat the error message problem,
 * where numbers intead of proper ID's would appear
 * EX: token mismatch: expected 1024 whereas found 40
 * NEW: token mismach: expected ID whereas found (
 */
void toalg(int alg)
{
	if ((alg > (-2)) && (alg < 6)) {
		//This is related to what it's in constants.h
		switch (alg) {
			case 0:
				fprintf(stderr, "VOID");
				break;
			case 1:
				fprintf(stderr, "BOOL");
				break;
			case 2:
				fprintf(stderr, "INT32");
				break;
			case 3:
				fprintf(stderr, "INT64");
				break;
			case 4:
				fprintf(stderr, "FLT32");
				break;
			case 5:
				fprintf(stderr, "FLT64");
				break;
			default:
				fprintf(stderr, "INCOMPTBL");
				break;

		}
	} else if ((alg > 1023) && (alg < 1032)) {
		//This is related to tokens.h values
		switch (alg) {
			case 1025:
				fprintf(stderr, "UINT");
				break;
			case 1026:
				fprintf(stderr, "FLOAT");
				break;
			case 1027:
				fprintf(stderr, "OCT");
				break;
			case 1028:
				fprintf(stderr, "HEX");
				break;
			case 1029:
				fprintf(stderr, "ASGN");
				break;
			case 1030:
				fprintf(stderr, "GEQ");
				break;
			case 1031:
				fprintf(stderr, "LEQ");
				break;
			case 1032:
				fprintf(stderr, "NEQ");
				break;
			default:
				fprintf(stderr, "ID");
				break;
		}
	} else if (alg > 10000) {
		switch (alg) {
			//This is related to what it's in keywords.h
			case 10002:
				fprintf(stderr, "NOT");
				break;
			case 10003:
				fprintf(stderr, "OR");
				break;
			case 10004:
				fprintf(stderr, "AND");
				break;
			case 10005:
				fprintf(stderr, "DIV");
				break;
			case 10006:
				fprintf(stderr, "MOD");
				break;
			case 10007:
				fprintf(stderr, "PROGRAM");
				break;
			case 10008:
				fprintf(stderr, "PROCEDURE");
				break;
			case 10009:
				fprintf(stderr, "FUNCTION");
				break;
			case 10010:
				fprintf(stderr, "RETURN");
				break;
			case 10011:
				fprintf(stderr, "VAR");
				break;
			case 10012:
				fprintf(stderr, "INTEGER");
				break;
			case 10013:
				fprintf(stderr, "REAL");
				break;
			case 10014:
				fprintf(stderr, "DOUBLE");
				break;
			case 10015:
				fprintf(stderr, "BOOLEAN");
				break;
			case 10016:
				fprintf(stderr, "IF");
				break;
			case 10017:
				fprintf(stderr, "THEN");
				break;
			case 10018:
				fprintf(stderr, "ELSE");
				break;
			case 10019:
				fprintf(stderr, "WHILE");
				break;
			case 10020:
				fprintf(stderr, "DO");
				break;
			case 10021:
				fprintf(stderr, "REPEATE");
				break;
			case 10022:
				fprintf(stderr, "UNTIL");
				break;
			case 10023:
				fprintf(stderr, "END");
				break;
			default:
				fprintf(stderr, "BEGIN");
				break;
		}
	} else {
		//Do a ascii conversion to print on error message otherwise
		fprintf(stderr, "%c", alg);
	}
}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		/* 
		 * Error message example: 
		 * token mismach: expected <expected> whereas found <found> at line <line> column <column> 
		 */
		fprintf(stderr, "token mismatch: expected "); 
		toalg(expected); 
		fprintf(stderr, " whereas found "); 
		toalg(lookahead);  
		fprintf(stderr, " at line %d column %d\n", linecounter, columcounter);
		exit(-2);
	}
}

/*
void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch: expected %c whereas found %c at line %d column %d\n",
			expected, lookahead, linecounter, columcounter); //TODO: converter numero pra algarismo
		exit(-2);
	}
}
*/