%{
#include "parser.tab.h"
#include <stdlib.h>

int debug = 1;
%}

%option noyywrap

digit    [0-9]
inum     {digit}+
fnum    ({digit}+"."{digit}*)|({digit}*"."{digit}+)


letter      [A-Z]
id       [A-Za-z][_A-Za-z]*

%%

{inum} {
        yylval.INT = atoi(yytext);
        if(debug){ printf("An integer: %d\n", yylval.INT); }
        return INT;
        }

{fnum} {
        yylval.FLOAT = atof(yytext);
        if(debug){ printf("A float: %f\n", yylval.FLOAT); }
        return FLOAT;
        }

"{"{inum}","{inum}","{inum}"}"

{id} {if(debug){ printf("An ID: %s\n", yytext); }
      //return strdup(yytext);;
      }

[ \t\n\r]+            ; /* Trim SEPS */

"->"      {if(debug){ printf("An arrow!\n");} return ARROW;}
[+\-*/]   { if(debug){ printf("An arithmetic operator: %s\n", yytext); } return yytext[0]; }
[<>!|&]      { if(debug){ printf("An boolean operator: %s\n", yytext); } return yytext[0]; }
"="            {if(debug){ printf("An equal sign: %s\n", yytext); } return yytext[0]; }

[(){}]      {if(debug){ printf("A brace: %s\n", yytext);} return yytext[0];}
[;]       {if(debug){ printf("A semicolon: %s\n", yytext);} return yytext[0];}

.          { return yytext[0]; }

%%
