%{
  #include "grammar.tab.h"
  //#include "helper.h"

  void set_var(char*, float);
  float get_var_val(char*);

  extern int debug;
  extern int print_strs;
%}

/* scanner for a simplified numeric control (NC from CNC machining) language */

%x call

digit       [0-9]
cap_letter  [A-Z]
letter      [A-Za-z]
alnum       [A-Za-z0-9]

inum     {digit}+
fnum    ({digit}+"."{digit}*)|({digit}*"."{digit}+)

/*At least two Letters before digits or underscore*/
/*So G00,X12,A_ can't be an id*/
/*Don't end on an underscore -> forbids: ABC_*/
id       {letter}{letter}+([_]*{alnum}+)*

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

{id}":" {
  char* s = yytext;
  s[strlen(s) - 1] = '\0';
  yylval.LABEL = strdup(s);
  if(debug){printf("A label:'%s'\n", s);}
  return LABEL;
}

"CALL" {if(debug){puts("An Include CALL!\n");}
   return CALL;}
"LASER_ON"|"LASER_OFF" {
  yylval.SPECIAL_CMD = strdup(yytext);
  return SPECIAL_CMD;}
"IF" {if(debug){puts("An IF!");}
  return IF;}
"THEN" {if(debug){puts("A THEN!");}
  return THEN;}
"ENDIF" {if(debug){puts("An ENDIF!");}
  return ENDIF;}
"GOTOB"|"GOTOF"|"GOTO" {if(debug){puts("A GOTO!");}
  return GOTO;}
"REPEAT" {if(debug){puts("A REPEAT!");}
  return REPEAT;}
"ROT" {if(debug){puts("A ROT!");}
  return ROT;}
"AROT" {if(debug){puts("An AROT!");}
  return AROT;}
"TRANS" {if(debug){puts("A TRANS!");}
  return TRANS;}
"SUPA" {if(debug){printf("A SUPA:'%s'\n",yytext);}
  return SUPA;}
"MSG" {if(debug){printf("A MSG:'%s'\n",yytext);}
  return MSG;}

{id} {if(debug){printf("An ID:'%s'\n",yytext);}
  yylval.ID = strdup(yytext);
  return ID;}

"_"{id} {if(debug){printf("An hidden ID:'%s'\n",yytext);}
  yylval.ID = strdup(yytext);
  return ID;}

[G]      {yylval.G_CMD = yytext[0]; return G_CMD;}
[XYZ]    {yylval.XYZ_CMD = yytext[0]; return XYZ_CMD;}
[ABC]    {yylval.ABC_CMD = yytext[0]; return ABC_CMD;}
{cap_letter} {yylval.CMD = yytext[0];
  if(debug){ printf("A command: %c\n", yytext[0]); }
  return CMD;
}

[+\-*/]   {if(debug){ printf("An arithmetic operator: %s\n",yytext);} return yytext[0];}
[<>!&|]   {if(debug){ printf("An boolean operator: %s\n",yytext);} return yytext[0];}
"="       {if(debug){ printf("An equal sign: %s\n",yytext);} return yytext[0];}

[()]      {return yytext[0];}


"\""[^\"]*"\"" {
  if (print_strs) {
    printf("A string: %s\n", yytext);
    puts("...");
  }

  // Allocate exact size for string contents excluding quotes, plus 1 for '\0'
  yylval.STRING = (char*)malloc(yyleng - 1);
  if (yylval.STRING != NULL) {
    // Copy (yyleng - 2) characters starting from yytext + 1
    memcpy(yylval.STRING, yytext + 1, yyleng - 2);
    yylval.STRING[yyleng - 2] = '\0';
  }

  return STRING;
}

";"[^\n]* {if(debug){printf("A comment:'%s'\n",yytext);}}

[ \t]      		 { if(debug){ printf("A SEP!\n");}  return SEP;}
"\r\n"         {if(debug){ printf("A NEWLINE!\n\n");}
                set_var("line", get_var_val("line") + 1); return NEWLINE;}
"\n"           {if(debug){ printf("A NEWLINE!\n\n");}
                set_var("line", get_var_val("line") + 1); return NEWLINE;}
","            {return yytext[0];}



<<EOF>> {yypop_buffer_state();
  if(!YY_CURRENT_BUFFER){
      yyterminate();}
}

.       {return OTHER;}

%%

int yywrap(){
    return 1;}
