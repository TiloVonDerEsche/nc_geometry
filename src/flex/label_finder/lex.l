%{
  #include "./../grammar.tab.h"
  void set_var(char*, float);
  float get_var_val(char*);

  long byte_counter = 0;
  #define YY_USER_ACTION byte_counter += yyleng;
  int debug = 1;

  #define MAX_LINE_LEN 1024
%}

/* label finder */

digit    [0-9]
inum     {digit}+
fnum    ({digit}+"."{digit}*)|({digit}*"."{digit}+)


alnum       ([a-zA-Z0-9])+
letter      [A-Z]

id       [A-Za-z][_A-Za-z0-9]*

%%

{letter}{id}":" {
                char* s = yytext;
                s[strlen(s) - 1] = '\0';
                if(debug){ printf("A label: %s, len=%lu \n", s,strlen(s)); }
               
		char label_id[MAX_LINE_LEN];
                strncpy(label_id, s, strlen(s));
                label_id[strlen(s)] = '\0';
               

		set_var(label_id, (float)byte_counter);

                char line_label[MAX_LINE_LEN+5];
                snprintf(line_label, sizeof(line_label), "%s_line", label_id);
                set_var(line_label, (float)get_var_val("line"));

                return LABEL;
                }

"\""[^\"]*"\"" { if(debug){ printf("A string: %s\n",yytext); } return STRING;}
";"[^\n]*      {if(debug){ printf("A comment: %s\n", yytext); } return COMMENT;}

[ \t]          {return SEP;}
"\r\n"         {set_var("line", get_var_val("line") + 1); return NEWLINE;}
"\n"           {set_var("line", get_var_val("line") + 1); return NEWLINE;}

.	       {return OTHER;}

%%


int yywrap(){
    return 1;
}
