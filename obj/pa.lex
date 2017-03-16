/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <stdio.h>
#include <math.h>
%}

DIGIT    [0-9]
ID       [a-z][a-z0-9]*
STR     "'"[^\n]*"'"
%%

{DIGIT}+    {
				printf( "An integer: %s (%d)\n", yytext,
                    atoi( yytext ) );
            }

{DIGIT}+"."{DIGIT}*        {
				printf( "A float: %s (%g)\n", yytext,
                    atof( yytext ) );
            }

if|then|var|real|integer|char|begin|end|procedure|program {
				printf( "A keyword: %s\n", yytext );
            }
{ID}"("[^)]*")"  printf( "Function call: %s\n", yytext );
{ID}         printf( "An identifier: %s\n", yytext );
{STR}      printf( "A string: %s\n", yytext );
":"|";"   printf( "A deliminator: %s\n", yytext );
":="|"+"|"-"|"*"|"/"   printf( "An operator: %s\n", yytext );

\{[^}\n]*[\n\}]   /* eat up comments */

[ \t\n]+          /* eat up whitespace */

.           printf( "Unrecognized character: %s\n", yytext );

%%

int main( int argc, char **argv )
{
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;

    yylex();
}
int yywrap()
{
	return 1;
}
