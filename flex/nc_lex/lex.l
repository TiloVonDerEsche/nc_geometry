%{
  #include "grammar.tab.c"
%}


/* scanner for a simplified numeric control (NC from CNC machining) language */

DIGIT    [0-9]
INT      {DIGIT}+

/* "1." and ".5" possible */
FLOAT    ({DIGIT}+"."{DIGIT}*)|({DIGIT}*"."{DIGIT}+)
NUM      {INT}|{FLOAT}

ID       [A-Za-z][_|(A-Za-z0-9)]*
LETTER   [A-Z]


%%



{INT} {
        yylval.ival = atoi(yytext);
        printf("An integer: %s\n", yytext);
        return INT;
}

{FLOAT} {
        yylval.fval = atof(yytext);
        printf("A float: %s\n", yytext);
        return FLOAT;
}

R{INT} {
        yylval.sval = yytext; //strdup(yytext);
        printf("A variable: %s\n", yytext);
        return VAR;
}

IF {
        printf("A keyword: %s\n", yytext);
        return IF;
}

"/"{LETTER}{ID} {
        yylval.sval = yytext;
        printf("A special command: %s\n", yytext);
        return SPECIAL_CMD;
}

{LETTER}{ID}":" {
        yylval.sval = yytext;
        printf("A label: %s\n", yytext);
        return LABEL;
}

{LETTER}{ID} {
        yylval.sval = yytext;
        printf("A custom Variable: %s\n", yytext);
        return CUSTOM_VAR;
}

{LETTER} {
        yylval.sval = yytext;
        printf("A command: %s\n", yytext);
        return CMD;
}


"=="  {return EQ;}
"!="  {return NEQ;}
"<="  {return LTEQ;}
">="  {return GTEQ;}

[+\-*/]   { printf("An arithmetic operator: %s\n", yytext); return yytext[0]; }
[<>!]     { printf("A boolean operator: %s\n", yytext); return yytext[0]; }



";"[^\n]* printf("A comment: %s\n", yytext);

"="   {printf("An assignment!\n");}
" "   printf("A space\n");
[\t\n]+

.       printf("Unrecognized character: %s\n", yytext);

%%
