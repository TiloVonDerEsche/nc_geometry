#include "lex.yy.c"
#include "grammar.tab.c"



int
main(int argc, char *argv[])
{
        ++argv; --argc;  /* skip over program name */
        if (argc > 0)
                yyin = fopen(argv[0], "rb");
        else
                yyin = stdin;

        if(hmhis_json) {hmhis = init_file("./data/hmhis.json","[\n");}
        tl = init_file("./data/track_list.csv","track_index,Ax,Ay,Az,Bx,By,Bz");

        init_hashmap();


        yyparse ();

        if(hmhis_json) {close_hmhis();}
        fclose(tl);
        return 0;
}
