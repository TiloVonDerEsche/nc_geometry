%{
  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  void yyerror (char const *);

  extern strfloat_t* h;
  extern khint_t k;
  extern int absent;
%}



%union {
  int ival;
  float fval;
  char* sval;

  char arith_op;
  char bool_op;
}

%token <ival> INT
%token <fval> FLOAT

%token <bool_op> EQ
%token <bool_op> NEQ
%token <bool_op> LTEQ
%token <bool_op> GTEQ


%token <sval> VAR
%token <sval> CUSTOM_VAR
%token <sval> CMD
%token <sval> SPECIAL_CMD
%token <sval> IF
%token <sval> LABEL

%nterm <fval> val
%nterm <fval> arith_expr
%nterm <ival> bool_expr

%left '+' '-'
%left '*' '/'

%%
input:
  %empty
| input line
;


line:
  '\n'
| arith_expr '\n'   { printf ("%.3g\n", $1);
                      print_hashmap(h,stdout);
                    }
| bool_expr '\n'    { printf ("%d\n", $1); }
| error '\n' { yyerrok; }
;

val:
  FLOAT  {$$ = $1;}
  | INT  {$$ = $1;}
  | VAR  {k = strfloat_get(h, $1);
          if ( kh_exist(h, k) ) {
            $$ = kh_val(h, k);
          }}
;

arith_expr:
  val         {$$=$1;}
  | arith_expr '+' arith_expr {puts("SUM!");$$=$1+$3;printf("%f+%f=%f\n", $1,$3,$$);}
  | arith_expr '-' arith_expr {$$=$1-$3;}
  | arith_expr '*' arith_expr {$$=$1*$3;}
  | arith_expr '/' arith_expr {$$=$1/$3;}

;

bool_expr:
  arith_expr '<' arith_expr {$$=$1<$3;}
  | arith_expr '>' arith_expr {$$=$1>$3;}
  | arith_expr EQ arith_expr {$$=$1==$3;}
  | arith_expr NEQ arith_expr {$$=$1!=$3;}
;


%%
