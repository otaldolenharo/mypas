# set default C-flags to allow debug and also get include directory as "/include"
CFLAGS = -g -I./include

mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~
