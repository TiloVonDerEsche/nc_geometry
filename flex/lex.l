%{
  #include "grammar.tab.h"

  long byte_counter = 0;
  #define YY_USER_ACTION byte_counter += yyleng;
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
        /*printf("An integer: %d\n", yylval.INT);*/
        return INT;
        }

{fnum} {
        yylval.FLOAT = atof(yytext);
        /*printf("A float: %f\n", yylval.FLOAT);*/
        return FLOAT;
        }

R{inum} {yylval.VAR = strdup(yytext); return VAR;}
H{inum} {yylval.VAR = strdup(yytext); return VAR;}


"/"{id}         {
                yylval.SPECIAL_CMD = strdup(yytext);
                /*printf("A special command: %s\n", yytext);*/
                return SPECIAL_CMD;
                }

{letter}{id}":" {
                yylval.LABEL = strdup(yytext);
                /*printf("A label: %s\n", yytext);*/
                return LABEL;
                }

{letter}{id} {
              if(strcmp(yytext,"LASER_ON") == 0 ||
                 strcmp(yytext,"LASER_OFF") == 0 ) {
                yylval.SPECIAL_CMD = strdup(yytext);
                return SPECIAL_CMD;
              }
              else if(strcmp(yytext,"PUIS_LASER") == 0 ||
                 strcmp(yytext,"MODE_LASER") == 0 ||
                 strcmp(yytext,"VIT_TIR") == 0) {
                yylval.CUSTOM_VAR = strdup(yytext);
                /*printf("A custom Variable: %s\n", yytext);*/
                return CUSTOM_VAR;
              }
              else if(strcmp(yytext,"CALL") == 0) {
                return CALL;
              }
              else {
                /*printf("A MISC_ID: %s\n",yytext);*/
                return MISC_ID;
              }

             }

{letter}       {yylval.CMD = strdup(yytext);
                /*printf("A command: %s\n", yytext);*/
                return CMD;
               }

"_"{id}   {
        /*printf("A hidden MISC_ID: %s\n",yytext);*/
        return MISC_ID;
       }


[+\-*/]   { /*printf("An arithmetic operator: %s\n", yytext);*/ return yytext[0]; }

[()]      {return yytext[0];}

"\""[^\"]*"\"" {printf("A string: %s\n",yytext); return STRING;}

";"[^\n]* {/*printf("A comment: %s\n", yytext);*/ return COMMENT;}

" "      		   {return SPACE;}
"\n"           {return NEWLINE;}
"="            {return SET;}
","            {return yytext[0];}
.      		     {return OTHER;}

%%


int yywrap(){
    return 1;
}
