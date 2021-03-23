/**@<lexer.c>::**/

/* This is the first example of a lexical analyser
 * inspired by an NFA
 *
 * It is necessary to implement a method to ignore spaces, 
 * namely skipspaces
 */
#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <lexer.h>

int columcounter = 1;
int linecounter = 1;
void skipunused ( FILE *tape )
{
	int head;

	_skip_spaces_head:
	while ( isspace ( head = getc (tape) ) ) {
		if (head == '\n') linecounter++;
	}
	
	if ( (head == '{') ) {
		while((head = getc(tape)) != '}' && (head != EOF)) {
			if (head == '\n') linecounter++;
		}
		if (head == '}') { goto _skip_spaces_head; }
	}	

	ungetc ( head, tape );
}

/* Next step resides in defining a recognizer for
 * Pascal'like identifiers
 *
 * REGEX: ID = [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) = 1, iff x \in [A-Za-z]
 *            = 0, iff x \notin [A-Za-z]
 */
int lookahead;
char lexeme[MAXIDLEN+1];
int isID (FILE *tape)
{
	int i = 0;

	if ( isalpha ( (lexeme[i] = getc (tape)) ) ) {

		i++;

		while ( isalnum ( (lexeme[i] = getc (tape)) ) ) {
			if (i < MAXIDLEN) i++;
		}

		ungetc ( lexeme[i], tape );

		lexeme[i] = 0;
		int key = iskeyword(lexeme);
		if (key) return key; //AQUI
		return ID;
	}

	ungetc ( lexeme[i], tape );

	return 0;

}

/* Next we have to define the recognizer for
 * simple decimal pattern (unsigned int)
 *
 * REGEX: UINT = [1-9][0-9]*  |  0
 */
int isUINT (FILE *tape)
{
	int i = 0;

	if ( isdigit ( (lexeme[i] = getc (tape)) ) ) {

		i++;

		if ( lexeme[i] != '0' ) {

			while ( isdigit ( (lexeme[i] = getc (tape)) ) ) {
				if (i < MAXIDLEN) i++;
			}

			ungetc ( lexeme[i], tape );

			lexeme[i] = 0;

		}

		return UINT;
	}

	ungetc ( lexeme[i], tape );

	return 0;
}
/* NUM is the generic decimal pattern, which means it
 * can be an unsigned interger or a floating-point.
 *
 * UINT = [1-9][0-9]*  |  0
 *
 * FLOAT = ( UINT '.' [0-9]* | '.' [0-9]+ ) eE? | UINT eE
 *
 * eE = [eE] ['+''-']? [0-9]+
 */

int isNUM(FILE *tape)
{
	int i = 0;
	int token;
	int head;

	if ( ( token = isUINT(tape) ) ) {
		if ( ( lexeme[i] = getc(tape) ) == '.' ) {
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
		}
	} else if ( (lexeme[i] = getc(tape)) == '.' ) {
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
	if ( token ) {
		int eE;
		if ( toupper(eE = getc(tape)) == 'E' ) {
			i++;
			int plusminus;
			if ( (plusminus = getc(tape)) == '+' || plusminus == '-' ) {
				i++;
			} else {
				ungetc(plusminus, tape);
				plusminus = 0;
			}
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

	return token;
}

/* Octal pattern:
 * REGEX: 0[0-7]+
 */
int isOCT(FILE *tape)
{
	int i = 0;

	if ( (lexeme[i] = getc(tape)) == '0' ) {

		i++;

		if ( isdigit((lexeme[i] = getc(tape))) && lexeme[i] < '8' ) {

			i++;

			while ( isdigit((lexeme[i] = getc(tape))) && lexeme[i] < '8' ){
				if (i < MAXIDLEN) i++;
			}

			ungetc(lexeme[i], tape);

			lexeme[i] = 1;

			return OCT;

		}

		ungetc(lexeme[i], tape);

		i--;

		ungetc('0', tape);

		return 0;

	}

	ungetc(lexeme[i], tape);

	return 0;
}

/* Hexadecimal pattern:
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int i = 0;
	int x;

	if ( (lexeme[i] = getc(tape)) == '0' ) {
		
		i++;

		if ( toupper(x = getc(tape)) == 'X' ) {

			if ( isxdigit((lexeme[i] = getc(tape))) ) {
				
				i++;

				while ( isxdigit((lexeme[i]= getc(tape))) ) {
					if (i < MAXIDLEN) i++;
				}

				ungetc (lexeme[i], tape);

				lexeme[i] = 0;

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
/* ASGN = ":=" (pascal)*/
int isASGN(FILE *tape)
{
	if ( (lexeme[0] = getc(tape)) == ':') { 
		if ( (lexeme[1] = getc(tape)) == '=' ) {
			lexeme[2] = 0;
			return ASGN; // foi encontrado :=
		}
		ungetc(lexeme[1], tape); // não foi encontrado =
	}
	ungetc(lexeme[0], tape); // não foi encontrado :
	lexeme[0] = 0; // palavra vazia
	return 0; // não aconteceu nada
}

int isRELOP(FILE *tape) 
{
	int i = 0;
	switch((lexeme[i] = getc(tape))) {
		case '=':
			i++;
			lexeme[i] = 0;
			return '=';
			break;
		case '>':
			i++;
			if ( ((lexeme[i] = getc(tape)) == '=') ) {
				i++;
				lexeme[i] = 0;
				return GEQ;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return '>';
			break;
		case '<':
			i++;
			if ( ((lexeme[i] = getc(tape)) == '=') ) {
				i++;
				lexeme[i] = 0;
				return LEQ;
			} else if (lexeme[i] == '>') {
				i++;
				lexeme[i] = 0;
				return NEQ;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return '<';
			break;
		default:
			break;
	}
	ungetc(lexeme[i], tape);
	return lexeme[i] = 0;
}


int gettoken ( FILE *source )
{
	int token;

	skipunused(source);

	if ( ( token = isID(source) ) ) return token;

	if ( ( token = isOCT(source) ) ) return token;

	if ( ( token = isHEX(source) ) ) return token;

	if ( ( token = isNUM(source) ) ) return token;

	if ( ( token = isRELOP(source) ) ) return token;

	if ( ( token = isASGN(source) ) ) return token;
	
	lexeme[1] = 0;
	token = lexeme[0] = getc(source);
	
	skipunused(source);
	return token;
}
