/* scanner for a simplified numeric control (NC from CNC machining) language */

DIGIT    [0-9]
INT      {DIGIT}+

/* "1." and ".5" possible */
FLOAT    ({DIGIT}+"."{DIGIT}*)|({DIGIT}*"."{DIGIT}+)
NUM      {INT}|{FLOAT}

ID       [A-Za-z][_|(A-Za-z0-9)]*
LETTER   [A-Z]
SPACE    " "
EQ       "="


%%



{INT} {
        printf("An integer: %s\n", yytext);
}

{FLOAT} {
        printf("A float: %s\n", yytext);
}

R{INT} {
        printf("A variable: %s\n", yytext);
}


"/"{LETTER}{ID} {
        printf("A special command: %s\n", yytext);
}

{LETTER}{ID} {
        printf("A User Variable: %s\n", yytext);
}

{LETTER} {
        printf("A command: %s\n", yytext);
}



if|then|begin|end|procedure|function {
        printf("A keyword: %s\n", yytext);
}


"+"|"-"|"*"|"/"   printf("An operator: %s\n", yytext);

";"[^\n]* printf("A comment: %s\n", yytext);

{EQ}      {printf("An assignment!\n");}
{SPACE}   printf("A space\n");
[\t\n]+

.       printf("Unrecognized character: %s\n", yytext);

%%

int yywrap() {return 1;}

int
main(int argc, char *argv[])
{
        ++argv; --argc;  /* skip over program name */
        if (argc > 0)
                yyin = fopen(argv[0], "r");
        else
                yyin = stdin;

        yylex();
}
