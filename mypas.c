/**@<mypas.c>::**/

/**
 * This is the MyPas Language Compiler 
 * it was developed by:
 * 
 * Daniel Hideki Kawabata
 * Gabriel Lenharo da Cunha
 * 
 * Supervised by Professor Eraldo Pereira Marinho
**/

#include <mypas.h>

FILE *source;

/**
 * main function of the MyPas Compiler
**/
int main (int argc, char const *argv[])
{
	//If it receives a agrument with the program opening
	//tries to open it as a archive
	if (argc > 1) {
		source = fopen(argv[1], "r"); 
		if(source == NULL) {
			fprintf(stderr, "%s: cannot open\n", //If it can't be opened, exit
				argv[1]
			);
			exit (-1);
		}
	} else {
		source = stdin;
	}

	lookahead = gettoken (source); //Then gets the first token

	//And calls the mypas function in parser
	mypas();

	exit (0);
}
