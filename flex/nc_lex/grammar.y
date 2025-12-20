%{
  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  int yyerror(char *s);

  strfloat_t* h;
  khint_t k;
  int absent;

  void set_var(char*, float);
  float get_var_val(char*);
  void init_hashmap();
%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SPACE NEWLINE SET SEMICOLON COMMENT OTHER
%token <int> INT
%token <float> FLOAT

%token EQ NEQ LTEQ GTEQ
%token IF GOTO

%token <char*> VAR
%token <char*> CMD
%token <char*> SPECIAL_CMD
%token <char*> CUSTOM_VAR
%token <char*> LABEL

%nterm <float> val
%nterm <float> arith_expr
%nterm <int> bool_expr

%left '+' '-'
%left '*' '/'

%%
prog:
  lines
;

lines:
  %empty
  | line NEWLINE lines
;

line:
  exprs                 {
                         set_var("line",get_var_val("line")+1);
                         print_hashmap(h, stdout);
                        }
;


exprs:
  %empty
  | expr
  | expr SPACE exprs;
;

expr:
  CMD arith_expr       {set_var($1,$2);}
  | assignment
  | SPECIAL_CMD          {
                          if(strcmp($1,"/LASER_ON") == 0) {
                            set_var("laser",1);
                          }
                          else if(strcmp($1,"/LASER_OFF") == 0) {
                            set_var("laser",0);
                          }
                         }
  | CUSTOM_VAR SPACE INT {
                           if(strcmp($1,"PUIS_LASER") == 0) {
                             set_var("laser_power",(float)$3);
                           }
                          }
  | CUSTOM_VAR SPACE FLOAT {
                            if(strcmp($1,"PUIS_LASER") == 0) {
                              set_var("laser_power",$3);
                            }
                           }
  | COMMENT
;

assignment:
  VAR SET arith_expr     {set_var($1,$3);}
  | CMD SET arith_expr   {set_var($1,$3);}
  | CUSTOM_VAR SET arith_expr {set_var($1,$3);}
;


val:
  VAR            {
                  printf("Getting VAR=%s\n",$1);
                  $$ = get_var_val($1);
                 }
  | INT          {printf("INT=%d\n",$1); $$=$1;}
  | FLOAT        {printf("FLOAT=%f\n",$1); $$=$1;}
;

arith_expr:
  val         {$$=$1;}
  | arith_expr '+' arith_expr {$$=$1+$3; printf("%f+%f=%f\n", $1,$3,$$);}
  | arith_expr '-' arith_expr {$$=$1-$3;}
  | arith_expr '*' arith_expr {$$=$1*$3;}
  | arith_expr '/' arith_expr {$$=$1/$3;}
  | '(' arith_expr ')'        {$$=$2;}
  | '-' arith_expr            {$$=-$2;}
;

bool_expr:
  arith_expr '<' arith_expr {$$=$1<$3;}
  | arith_expr '>' arith_expr {$$=$1>$3;}
  | arith_expr EQ arith_expr {$$=$1==$3;}
  | arith_expr NEQ arith_expr {$$=$1!=$3;}
  | arith_expr LTEQ arith_expr {$$=$1<=$3;}
  | arith_expr GTEQ arith_expr {$$=$1>=$3;}

;


%%

int yyerror(char *s)
{
	printf("Syntax Error on line %s\n", s);
	return 0;
}

void set_var(char* varname, float fnum) {
  k = strfloat_put(h, varname, &absent);
  if (absent) {
    kh_key(h, k) = strdup(varname);}
  kh_val(h, k) = fnum;
  printf("Set %s to %f\n", varname, fnum);
}

float get_var_val(char* varname) {
  k = strfloat_get(h, varname);
  if ( kh_exist(h, k) ) {
    return kh_val(h, k);
  }

  return 0;
}


void init_hashmap() {
  khint_t k;
  int absent;

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 1;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");
  kh_val(h, k) = 0;

  k = strfloat_put(h, "laser_power", &absent);
  kh_key(h, k) = strdup("laser_power");
  kh_val(h, k) = 0;

  print_hashmap(h, stdout);
}
