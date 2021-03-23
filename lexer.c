/**@<lexer.c>::**/

/**
 * This is the Lexical Analyzer of the MyPas Language Compiler 
 * it was developed by:
 * 
 * Daniel Hideki Kawabata
 * Gabriel Lenharo da Cunha
 * 
 * Supervised by Professor Eraldo Pereira Marinho
**/

#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <lexer.h>

int columcounter = 1;
int linecounter = 1;

/**
 * Skips the unused characters in the compilation process
 * like spaces and commentaries
**/
void skipunused ( FILE *tape )
{
	int head;

	_skip_spaces_head:
	while ( isspace ( head = getc (tape) ) ) { //While reads any kind of space
		
		if (head == '\t'){ //If it is a Horizontal Tab, adds 4 spaces to the count
			columcounter+=4;
		} else if (head == '\40') //If not, adds just one
		{
			columcounter++;
		}
		
		if (head == '\n'){ //If founds a line feed
			columcounter = 1; //Resets Column Counter to the begining
			linecounter++; //And adds 1 to the Line Counter variable
		}
	}
	
	if ( (head == '{') ) { //If a '{' is found, it means that a commentary is begining
		while((head = getc(tape)) != '}' && (head != EOF)) {
			
			//It keeps counting lines and columns besides that

			if (head == '\t'){ 
				columcounter+=4;
			} else if (head == '\40') 
			{
				columcounter++;
			}

			if (head == '\n'){
				columcounter = 1;
				linecounter++;
			}
		}
		if (head == '}') { goto _skip_spaces_head; } //Do it until a '}' is found
	}	

	ungetc ( head, tape );

}

int lookahead;
char lexeme[MAXIDLEN+1];

/**
 * isID identifies a identifier in MyPas language
 * 
 * It can be described as REGEX: ID = [A-Za-z][A-Za-z0-9]*
**/
int isID (FILE *tape)
{
	int i = 0;

	//Here is where the first part of the REGEX is processed, allowing only words
	//iniciated by [A-Za-z]
	if ( isalpha ( (lexeme[i] = getc (tape)) ) ) { 

		i++;

		//And here the rest of the REGEX, allowing [A-Za-z0-9] multiple times
		while ( isalnum ( (lexeme[i] = getc (tape)) ) ) {
			if (i < MAXIDLEN) i++;
		}

		ungetc ( lexeme[i], tape );

		columcounter+=(i-1); //Adds the size of ID into the column counter

		lexeme[i] = 0;
		int key = iskeyword(lexeme); //Checks if is a keyword
		if (key) return key; //If it is, then returns her code
		return ID; //Else returns the ID code
	}

	ungetc ( lexeme[i], tape );

	return 0; //Or if it isn't a ID returns 0

}
/**
 * isUINT identifies a unsigned int in MyPas language,
 * 
 * It can be described as REGEX: UINT = [1-9][0-9]* | 0
**/
int isUINT (FILE *tape)
{
	int i = 0;

	//Checks if it starts with a number
	if ( isdigit ( (lexeme[i] = getc (tape)) ) ) {

		i++;

		//If it's different than a Zero
		if ( lexeme[i] != '0' ) { 

			while ( isdigit ( (lexeme[i] = getc (tape)) ) ) {
				if (i < MAXIDLEN) i++; //Reads all the digits
			}

			ungetc ( lexeme[i], tape );

			lexeme[i] = 0;

		}

		columcounter+=(i-1); //Updates the size of the line
		return UINT; //And returns the UINT code
	}

	ungetc ( lexeme[i], tape );
	return 0; //If it isn't a UINT returns 0
}
/**
 * isNUM identifies a generic number in MyPas language,
 * 
 * It can be UINT or FLOAT, described respectively as
 * 
 * REGEX: UINT = [1-9][0-9]*  |  0
 * 
 * REGEX: FLOAT = ( UINT '.' [0-9]* | '.' [0-9]+ ) eE? | UINT eE
 * 
 * and eE being exponencial part of the Floating Point number, described as
 * 
 * REGEX: eE = [eE] ['+''-']? [0-9]+
**/
int isNUM(FILE *tape)
{
	int i = 0;
	int token;
	int head;

	//It primarely verify if it starts as a UINT
	if ( ( token = isUINT(tape) ) ) {

		//If it is followed by a '.', is considered a FLOAT
		if ( ( lexeme[i] = getc(tape) ) == '.' ) {
			i++;
			token = FLOAT;

			//And keeps reading the decimal part
			while ( isdigit((lexeme[i] = getc(tape))) ) {
				if (i < MAXIDLEN) i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;

		} else {
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
		}

	//If it's not started by UINT can be started by the '.' itself
	} else if ( (lexeme[i] = getc(tape)) == '.' ) {

		//It is still is a FLOAT
		if ( isdigit( (lexeme[i] = getc(tape)) ) ) {
			i++;
			token = FLOAT;
			while ( isdigit((lexeme[i] = getc(tape))) ) {
				if (i < MAXIDLEN) i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;

		} else {
			ungetc (lexeme[i], tape);
			lexeme[i] = 0;
			ungetc ('.', tape);
		}

	} else {
		ungetc (lexeme[i], tape);
		lexeme[i] = 0; 
	}

	//Now checks the token given in this last IF
	if ( token ) {
		int eE;
		//Verify if it have a exponencial part
		if ( toupper(eE = getc(tape)) == 'E' ) {
			i++;
			int plusminus;
			
			//And if this exponencial part is positive or negative
			if ( (plusminus = getc(tape)) == '+' || plusminus == '-' ) {
				i++;
			} else {
				ungetc(plusminus, tape);
				plusminus = 0;
			}
			
			//Catches the exponencial part
			if ( isdigit((lexeme[i] = getc(tape))) ) {
				i++;
				token = FLOAT;
				while ( isdigit((lexeme[i] = getc(tape))) ) {
					if (i < MAXIDLEN) i++;
				}
				ungetc(lexeme[i], tape);
				lexeme[i] = 1;
			} else {
				ungetc(lexeme[i], tape);
				lexeme[i] = 1;
				if (plusminus) {
					ungetc(plusminus, tape);
					lexeme[i] = 0;
				}
				ungetc(eE, tape);
				lexeme[i] = 0;
			}
		} else {
			ungetc(eE, tape);
			lexeme[i] = 0;
		}
	}
	
	columcounter+=(i-1); //Updates the length of the line
	return token; //Returns the token, that can be a UINT or a FLOAT
}

/**
 * isOCT identifies a Octal base number in MyPas language,
 * 
 * It can be described as REGEX: OCT = 0[0-7]+
**/
int isOCT(FILE *tape)
{
	int i = 0;

	//Octal pattern needs to start with a 0 digit,
	//to differenciate to others numerical bases
	if ( (lexeme[i] = getc(tape)) == '0' ) {
		i++;

		//And then reads the number
		if ( isdigit((lexeme[i] = getc(tape))) && lexeme[i] < '8' ) {
			i++;

			while ( isdigit((lexeme[i] = getc(tape))) && lexeme[i] < '8' ){
				if (i < MAXIDLEN) i++;
			}

			ungetc(lexeme[i], tape);
			lexeme[i] = 1;

			columcounter+=(i-1); //Updates the column read
			return OCT;

		}
		ungetc(lexeme[i], tape);
		i--;

		columcounter+=(i-1); //Updates the size of the line
		ungetc('0', tape);
		return 0;

	}

	ungetc(lexeme[i], tape);
	return 0;
}

/**
 * isHEX identifies a Hexadecimal base number in MyPas language,
 * 
 * It can be described as REGEX: HEX = 0[xX][0-9A-Fa-f]+
**/
int isHEX(FILE *tape)
{
	int i = 0;
	int x;

	//Hexadecimal, just like Octal, needs to start with a
	//specific sequence
	if ( (lexeme[i] = getc(tape)) == '0' ) {
		
		i++;

		//In Hex this sequence is 0x or 0X
		if ( toupper(x = getc(tape)) == 'X' ) {

			if ( isxdigit((lexeme[i] = getc(tape))) ) {
				
				i++;

				//That recognized, it starts to read the number
				while ( isxdigit((lexeme[i]= getc(tape))) ) {
					if (i < MAXIDLEN) i++;
				}

				ungetc (lexeme[i], tape);

				lexeme[i] = 0;

				columcounter+=(i-1); //Updates the column read
				return HEX;

			}

			ungetc (lexeme[i], tape);

			i--;
		}

		ungetc (x, tape);
		ungetc('0', tape);
		return 0;
	}

	ungetc(lexeme[i], tape);
	return 0;
}

/**
 * isASGN identifies a Assignment in MyPas language,
 * 
 * It is recognized by the follow of ':=' to a variable
**/
int isASGN(FILE *tape)
{
	//Differently to C Language, Pascal uses := as assignment
	//symbol, and so does MyPas as well
	if ( (lexeme[0] = getc(tape)) == ':') { 
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			lexeme[2] = 0;
			columcounter+=2; //Updates the column read
			return ASGN; //If it recognizes the := sequence, returns ASGN code
		}
		ungetc(lexeme[1], tape); 
	}
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}


/**
 * isRELOP identifies a Relational Operator in MyPas language,
 * 
 * It can be any kind of operators that compare values, 
 * such as '=', '>', '<', '>=', '<=' and '<>'
**/
int isRELOP(FILE *tape) 
{
	int i = 0;

	//To verify if it is a RELOP, switches between the three
	//possible starts, '=', '<' and '>'
	switch((lexeme[i] = getc(tape))) {
		//Case starts with '=' then it is just a equals sign
		case '=':
			i++;
			lexeme[i] = 0;
			columcounter+=1; //Updates the column read
			return '=';
			break;

		//Case starts with '>' can be only '>' or '>='
		case '>':
			i++;

			//If it's '>=' then returns GEQ code
			if ( ((lexeme[i] = getc(tape)) == '=') ) {
				i++;
				lexeme[i] = 0;
				columcounter+=2; //Updates line's length
				return GEQ;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			columcounter+=1; //updates line's size
			return '>'; //Else it is just '>'
			break;

		//Case starts with '<' can be '<', '<=' or '<>'
		case '<':
			i++;

			//If '<' is followed by '=' then returns LEQ code
			if ( ((lexeme[i] = getc(tape)) == '=') ) {
				i++;
				lexeme[i] = 0;
				columcounter+=2; //Updates size of line
				return LEQ;

			//Otherwise if it is followed by '>' returns NEQ code
			} else if (lexeme[i] == '>') {
				i++;
				lexeme[i] = 0;
				columcounter+=2; //Updates the column read
				return NEQ;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;

			columcounter+=1; //Updates column read
			//But if it doesn't have this symbols as followers returns just '<'
			return '<'; 
			break;
		default:
			break;
	}
	ungetc(lexeme[i], tape);
	return lexeme[i] = 0;
}

/**
 * Gets the token relative to what was read in *source
**/
int gettoken ( FILE *source )
{
	int token;

	//To tokenify, firstly skips all kinds of spaces
	skipunused(source);

	//And then searches what kind of token it is recognized by
	if ( ( token = isID(source) ) ) return token;

	if ( ( token = isOCT(source) ) ) return token;

	if ( ( token = isHEX(source) ) ) return token;

	if ( ( token = isNUM(source) ) ) return token;

	if ( ( token = isRELOP(source) ) ) return token;

	if ( ( token = isASGN(source) ) ) return token;
	
	//If it isn't recognized by any token, then is just a symbol
	//like ';' or ','
	lexeme[1] = 0;
	token = lexeme[0] = getc(source);
	columcounter+=1;
	
	skipunused(source);
	return token;
}
