#include "lex.yy.c"
#include "grammar.tab.c"



int
main(int argc, char *argv[])
{
        ++argv; --argc;  /* skip over program name */
        if (argc > 0)
                yyin = fopen(argv[0], "r");
        else
                yyin = stdin;

        init_hashmap();
        init_hmhis();

        yyparse ();

        close_hmhis();
        return 0;
}
