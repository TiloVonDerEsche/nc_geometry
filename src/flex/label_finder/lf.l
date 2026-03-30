%option prefix="lf"
%{
  #include "./../helper.h"
  void set_var(char*, float);
  float get_var_val(char*);

  long byte_counter = 0;
  #define YY_USER_ACTION byte_counter += yyleng;

  #define MAX_LINE_LEN 1024
  #define MAX_LABEL_LEN 32

  int lfwrap();
  size_t line = 1;
%}

/* label finder */
/**/

digit    [0-9]
inum     {digit}+
fnum    ({digit}+"."{digit}*)|({digit}*"."{digit}+)


alnum       ([a-zA-Z0-9])+
letter      [A-Z]

id       [A-Za-z][_A-Za-z0-9]*

%%

"REPEAT" {
  //gets exceeded when 32 < 8 + line_digits <=> 24 < line_digits (decimal)
  //Meaning: line > 10^24
  //64-Bit: 2^64 - 1 =^ max 20 decimal digits
  char line_label[MAX_LABEL_LEN];
  snprintf(line_label, sizeof(line_label), "REPEAT_%lu", line);
  set_var(line_label, (float)byte_counter);
}



{letter}{id}":" {
                char* s = lftext;
                s[strlen(s) - 1] = '\0';
                if(debug){ printf("A label: %s, len=%lu \n", s,strlen(s)); }

                char label_id[MAX_LABEL_LEN+1]; //+1 for '\0'
                strncpy(label_id, s, strlen(s));
                label_id[strlen(s)] = '\0';

		            set_var(label_id, (float)byte_counter);

                char line_label[MAX_LABEL_LEN+6];
                snprintf(line_label, sizeof(line_label), "%s_line", label_id);
                set_var(line_label, (float)line);
                }

"\""[^\"]*"\"" { if(debug){ printf("A string: %s\n",lftext); }}
";"[^\n]*      {if(debug){ printf("A comment: %s\n", lftext); }}

[ \t]          {}
"\r\n"         {line++;}
"\n"           {line++;}
<<EOF>>        {return 0;}

.	       {}

%%

int lfwrap(){
    return 1;
}
