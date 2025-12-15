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



%define api.value.type union /* Generate YYSTYPE from these types: */

%token <int> INT
%token <float> FLOAT

%token <char*> EQ
%token <char*> NEQ
%token <char*> LTEQ
%token <char*> GTEQ


%token <char*> VAR
%token <char*> CUSTOM_VAR
%token <char*> CMD
%token <char*> SPECIAL_CMD
%token <char*> IF
%token <char*> LABEL

%nterm <float> val
%nterm <float> arith_expr
%nterm <float> bool_expr

%left '+' '-'
%left '*' '/'

%%
input:
  %empty          {puts("%empty");}
| input line      {puts("\ninput line!");}
;


line:
  '\n'
| arith_expr '\n'   {puts("arith!"); printf ("%f\n", $1);
                      print_hashmap(h,stdout);
                      fflush(stdout);
                    }
| bool_expr '\n'    { printf ("%d\n", $1); }
| VAR '=' arith_expr '\n' {
                            // Logic to store $3 into the variable $1
                            printf("Assignment: %s = %f\n", $1, $3);
                            /*k = strfloat_put(h, $1, &absent);
                            if (absent) {
                              kh_key(h, k) = strdup($1);}
                            kh_val(h, k) = $3;
                            print_hashmap(h, stdout);*/
                          }
| CMD '=' arith_expr '\n' { /* Handle simple commands */ }
| error '\n' { puts("An error has occured:"); }
;

val:
  FLOAT  {$$ = $1;}
  | INT  {$$ = $1;}
  | VAR  { /*k = strfloat_get(h, $1);
          if ( kh_exist(h, k) ) {
            $$ = kh_val(h, k); }
          else {
            yyerrok;
            }*/
         }
;

arith_expr:
  val         {$$=$1;}
  | arith_expr '+' arith_expr {puts("SUM!");$$=$1+$3;printf("%f+%f=%f\n", $1,$3,$$);fflush(stdout);}
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
