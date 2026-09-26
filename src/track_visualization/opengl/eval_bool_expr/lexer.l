%{
#include "parser.tab.h"
#include <stdlib.h>

extern int debug;
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

"t_id" {return T_ID;}
"t_start_x" {return T_START_X;}
"t_start_y" {return T_START_Y;}
"t_start_z" {return T_START_Z;}
"t_end_x" {return T_END_X;}
"t_end_y" {return T_END_Y;}
"t_end_z" {return T_END_Z;}
"laser_power" {return LASER_POWER;}
"machine_speed" {return MACHINE_SPEED;}
"g_code" {return G_CODE;}
"hradius" {return HRAD;}
"vradius" {return VRAD;}

{id} {//if(debug){ printf("An ID: %s\n", yytext); }
        fprintf(stderr,"An unknown ID: %s\n", yytext);
      }

[ \t\n\r]+            ; /* Trim SEPS */

"->"      {if(debug){ printf("An arrow!\n");} return ARROW;}
"||"      {return OR;}
"&&"      {return AND;}
"=="      {return EQ;}
"!="      {return NEQ;}
"<="      {return LTEQ;}
">="      {return GTEQ;}
"="       {if(debug){ printf("An equal sign: %s\n", yytext); } return yytext[0]; }
[+\-*/]   { if(debug){ printf("An arithmetic operator: %s\n", yytext); } return yytext[0]; }
[<>!|&]      { if(debug){ printf("An boolean operator: %s\n", yytext); } return yytext[0]; }


[(){}]      {if(debug){ printf("A brace: %s\n", yytext);} return yytext[0];}
[;]       {if(debug){ printf("A semicolon: %s\n", yytext);} return yytext[0];}
[,]       {if(debug){ printf("A comma: %s\n", yytext);} return yytext[0];}

.          { return yytext[0]; }

%%
