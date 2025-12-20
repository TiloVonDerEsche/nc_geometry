%{
#include "grammar.tab.h"
void showError();
%}

/* scanner for a simplified numeric control (NC from CNC machining) language */

digit    [0-9]
inum     {digit}+
fnum    ({digit}+"."{digit}*)|({digit}*"."{digit}+)


alnum       ([a-zA-Z0-9])+
letter      [A-Z]

id       [A-Za-z][_A-Za-z0-9]*

%%


{inum} {
        yylval.INT = atoi(yytext);
        printf("An integer: %s->%d\n", yytext, yylval.INT);
        return INT;
        }

{fnum} {
        yylval.FLOAT = atof(yytext);
        printf("A float: %s->%f\n", yytext, yylval.FLOAT);
        return FLOAT;
        }

R{inum} {yylval.VAR = strdup(yytext); return VAR;}


"/"{letter}{id} {
                yylval.SPECIAL_CMD = strdup(yytext);
                printf("A special command: %s\n", yytext);
                return SPECIAL_CMD;
                }

{letter}{id}":" {
                yylval.LABEL = strdup(yytext);
                printf("A label: %s\n", yytext);
                return LABEL;
                }

{letter}{id} {
              yylval.CUSTOM_VAR = strdup(yytext);
              printf("A custom Variable: %s\n", yytext);
              return CUSTOM_VAR;
              }

{letter}       {yylval.CMD = strdup(yytext);
                printf("A command: %s\n", yytext);
                return CMD;
               }


[+\-*/]   { printf("An arithmetic operator: %s\n", yytext); return yytext[0]; }

" "      		   {return SPACE;}
"\n"           {return NEWLINE;}
"="            {puts("SET");return SET;}
.      		     {showError(); return OTHER;}

%%


void showError(){
    printf("Other input");
}
int yywrap(){
    return 1;
}
