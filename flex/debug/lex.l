%{
  #include "grammar.tab.c"
%}


/* scanner for a simplified numeric control (NC from CNC machining) language */

DIGIT    [0-9]
INT      {DIGIT}+

LETTER   [A-Z]


%%



{INT} {
        yylval.INT = atoi(yytext);
        printf("An integer: %s\n", yytext);
        return INT;
}


R{INT} {
        yylval.VAR = strdup(yytext);
        printf("A variable: %s\n", yytext);
        return VAR;
}


" "   printf("A space\n");
[\t\n]+

.       printf("Unrecognized character: %s\n", yytext);

%%
