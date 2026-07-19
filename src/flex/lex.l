%{
  #include "grammar.tab.h"
  //#include "helper.h"

  void set_var(char*, float);
  float get_var_val(char*);

  extern int debug;
  extern int print_strs;
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
        if(debug){ printf("An integer: %d\n", yylval.INT); }
        return INT;
        }

{fnum} {
        yylval.FLOAT = atof(yytext);
        if(debug){ printf("A float: %f\n", yylval.FLOAT); }
        return FLOAT;
        }

R{inum} {yylval.VAR = strdup(yytext); return VAR;}
H{inum} {yylval.VAR = strdup(yytext); return VAR;}

{letter}{id}":" {
                char* s = yytext;
                s[strlen(s) - 1] = '\0';
                yylval.LABEL = strdup(s);
                if(debug){ printf("A label: %s\n", s); }
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
                 strcmp(yytext,"VIT_TIR") == 0 ||
                 strcmp(yytext,"Flow_gas_2") == 0 ||
                 strcmp(yytext,"Flow_gas_3") == 0 ||
                 strcmp(yytext,"Feeder_Speed_2") == 0 ||
                 strcmp(yytext,"Feeder_Speed_3") == 0 ||
                 strcmp(yytext,"TOFFL") == 0) {

                yylval.CUSTOM_VAR = strdup(yytext);
                if(debug){ printf("A custom Variable: %s\n", yytext); }
                return CUSTOM_VAR;
              }
              else if(strcmp(yytext,"IF") == 0) {
                if(debug){ puts("An IF!"); }
                return IF;
              }
              else if(strcmp(yytext,"ENDIF") == 0) {
                if(debug){ puts("An ENDIF!"); }
                return ENDIF;
              }
              else if(strcmp(yytext,"GOTOB") == 0 ||
                      strcmp(yytext,"GOTOF") == 0 ||
                      strcmp(yytext,"GOTO") == 0) {
                if(debug){ puts("A GOTO!"); }
                return GOTO;
              }
              else if(strcmp(yytext,"REPEAT") == 0) {
                if(debug){ puts("A REPEAT!"); }
                return REPEAT;
              }
              else if(strcmp(yytext,"ROT") == 0) {
                if(debug){ puts("A ROT!"); }
                return ROT;
              }
              else if(strcmp(yytext,"CALL") == 0 ||
                      strcmp(yytext,"MSG") == 0) {
                return MSG;
              }
              else {
                if(debug){ printf("A MISC_ID: %s\n",yytext); }
                yylval.MISC_ID = strdup(yytext);
                return MISC_ID;
              }

             }

[XYZ]{fnum}  {yylval.XYZ_CMD = yytext[0]; return XYZ_CMD;}
[ABC]{fnum}  {yylval.ABC_CMD = yytext[0]; return ABC_CMD;}
{letter}     {yylval.CMD = strdup(yytext);
                if(debug){ printf("A command: %s\n", yytext); }
                return CMD;
             }

"_"{id}   {
        if(debug){ printf("A hidden MISC_ID: %s\n",yytext); }
        return MISC_ID;
       }


[+\-*/]   { if(debug){ printf("An arithmetic operator: %s\n", yytext); } return yytext[0]; }
[<>!]      { if(debug){ printf("An boolean operator: %s\n", yytext); } return yytext[0]; }
"="            {if(debug){ printf("An equal sign: %s\n", yytext); } return yytext[0]; }

[()]      {return yytext[0];}

"\""[^\"]*"\"" { if(print_strs){ printf("A string: %s\n",yytext);puts("..."); } return STRING;}

";"[^\n]* {if(debug){ printf("A comment: %s\n", yytext); } return COMMENT;}

[ \t]      		 { if(debug){ printf("A SEP!\n");}  return SEP;}
"\r\n"         {if(debug){ printf("A NEWLINE!\n\n");}
                set_var("line", get_var_val("line") + 1); return NEWLINE;}
"\n"           {if(debug){ printf("A NEWLINE!\n\n");}
                set_var("line", get_var_val("line") + 1); return NEWLINE;}
","            {return yytext[0];}

.      		     {return OTHER;}

%%


int yywrap(){
    return 1;
    //set_var("line", get_var_val("line") + 1);
}
