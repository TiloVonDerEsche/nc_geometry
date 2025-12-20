%{
  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  int yyerror(char *s);

  strfloat_t* h;
  khint_t k;
  int absent;

%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token SPACE NEWLINE SET SEMICOLON OTHER
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
  exprs                 {print_hashmap(h, stdout);}
;


exprs:
  %empty
  | expr
  | expr SPACE exprs;
;

expr:
  CMD INT                {printf("CMD: %s=%d\n",$1,$2);}
  | CMD FLOAT            {printf("CMD: %s=%f\n",$1,$2);}
  | assignment
  | arith_expr           {printf("arith_expr=%f\n",$1);}
;

assignment:
  VAR SET arith_expr     {
                            // Logic to store $3 into the variable $1
                            printf("Assignment: %s = %f\n", $1, $3);
                            k = strfloat_put(h, $1, &absent);
                            if (absent) {
                              kh_key(h, k) = strdup($1);}
                            kh_val(h, k) = $3;
                            printf("Set %s to %f\n", $1, $3);
                          }
  | CMD SET arith_expr   { /* Handle simple commands */ }
;

val:
  VAR            {
                  printf("Getting VAR=%s\n",$1);
                  k = strfloat_get(h, $1);
                  if ( kh_exist(h, k) ) {
                    $$ = kh_val(h, k);
                    printf("'%s'=%f\n",$1,$$);
                  } else {$$=0;}
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


void init_hashmap() {
  khint_t k;
  int absent;

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 123;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");

  kh_val(h, k) = -1.2;
  k = strfloat_put(h, "laser_power", &absent);
  kh_key(h, k) = strdup("laser_power");
  kh_val(h, k) = 3500;

  k = strfloat_put(h, "R1", &absent);
  kh_key(h, k) = strdup("R1");
  kh_val(h, k) = 456;

  print_hashmap(h, stdout);
}
