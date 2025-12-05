#include "lex.yy.c"

void
yyerror (char const *s)
{
  fprintf (stderr, "err: %s\n", s);
}

int yywrap() {return 1;}

strfloat_t* h;
khint_t k;
int absent;

int
main(int argc, char *argv[])
{
        ++argv; --argc;  /* skip over program name */
        if (argc > 0)
                yyin = fopen(argv[0], "r");
        else
                yyin = stdin;


        init_hashmap(h);


        return yyparse ();
}
