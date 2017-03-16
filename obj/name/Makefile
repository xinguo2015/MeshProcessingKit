NAME=name
YACC=yacc
LEX=flex
CC=gcc
CPPFLAGS=
CFLAGS=
 
prog: $(NAME).o $(NAME)-yy.o 
	gcc -o prog $(NAME).o $(NAME)-yy.o 

.PHONY: clean

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $^

%-yy.c: %.lex
	$(LEX) -o $@ $^

%.c: %.y
	$(YACC) -d -o $@ $^

clean:
	rm -rf *.o
	rm -rf *.c
