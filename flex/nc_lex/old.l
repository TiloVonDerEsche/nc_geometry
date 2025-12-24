%{
  #include "grammar.tab.h"
%}


/* scanner for a simplified numeric control (NC from CNC machining) language */

DIGIT    [0-9]
INT      {DIGIT}+

/* "1." and ".5" possible */
FLOAT    ({DIGIT}+"."{DIGIT}*)|({DIGIT}*"."{DIGIT}+)
NUM      {INT}|{FLOAT}

ID       [A-Za-z][_A-Za-z0-9]*
LETTER   [A-Z]


%%



{INT} {
        yylval.INT = atoi(yytext);
        printf("An integer: %s\n", yytext);
        return INT;
}

{FLOAT} {
        yylval.FLOAT = atof(yytext);
        printf("A float: %s\n", yytext);
        return FLOAT;
}

R{INT} {
        yylval.VAR = strdup(yytext);
        printf("A variable: %s\n", yytext);
        return VAR;
}

IF {
        printf("A keyword: %s\n", yytext);
        return IF;
}

"/"{LETTER}{ID} {
        yylval.SPECIAL_CMD = strdup(yytext);
        printf("A special command: %s\n", yytext);
        return SPECIAL_CMD;
}

{LETTER}{ID}":" {
        yylval.LABEL = strdup(yytext);
        printf("A label: %s\n", yytext);
        return LABEL;
}

{LETTER}{ID} {
        yylval.CUSTOM_VAR = strdup(yytext);
        printf("A custom Variable: %s\n", yytext);
        return CUSTOM_VAR;
}

{LETTER} {
        yylval.CMD = strdup(yytext);
        printf("A command: %s\n", yytext);
        return CMD;
}


"=="  {return EQ;}
"!="  {return NEQ;}
"<="  {return LTEQ;}
">="  {return GTEQ;}

[+\-*/]   { printf("An arithmetic operator: %s\n", yytext); return yytext[0]; }
[<>!]     { printf("A boolean operator: %s\n", yytext); return yytext[0]; }



";"[^\n]* {printf("A comment: %s\n", yytext); return SEMICOLON;}

"="   {printf("An assignment!\n");}
" "   {printf("A space\n"); return SPACE;}
[\t\n]+

.       {printf("Unrecognized character: %s\n", yytext); return OTHER;}

%%
