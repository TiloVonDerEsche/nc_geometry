%{
  #include <stdio.h>  /* For printf, etc. */
  #include <math.h>   /* For pow, used in the grammar. */
  #include "calc.h"   /* Contains definition of 'symrec'. */
  #include "helper.h"

  #define YYDEBUG 1
  int yylex (void);
  void yyerror (char const *);

  extern strfloat_t* h;
  extern khint_t k;
  extern int absent;
  khint_t kr;
 %}


%define api.value.type union /* Generate YYSTYPE from these types: */
%token <double>  NUM     /* Double precision number. */
%token <char*> VAR  /* Symbol table pointer: variable/function. */
%token <symrec*> FUN
%nterm <double>  exp

%precedence '='
%left '-' '+'
%left '*' '/'
%precedence NEG /* negation--unary minus */
%right '^'      /* exponentiation */


%%

input:
  %empty
| input line
;


line:
  '\n'
| exp '\n'   { printf ("%.10g\n", $1);
              //print_hashmap(h,stdout);
             }
| error '\n' { yyerrok; }
;


exp:
  NUM
| VAR                { //kr = strfloat_get(h, $1);
                      //if (kr < kh_end(h)) {
                      //  $$ = kh_val(h, kr);
                      //}
                      kr = strfloat_put(h, $1, &absent);
                      $$ = kh_val(h, kr);
                     }
| VAR '=' exp        { k = strfloat_put(h, $1, &absent);
                      kh_val(h, k) = $3;
                      $$ = $3;
                     }
| FUN '(' exp ')'    { $$ = $1->value.fun ($3);         }
| exp '+' exp        { $$ = $1 + $3;                    }
| exp '-' exp        { $$ = $1 - $3;                    }
| exp '*' exp        { $$ = $1 * $3;                    }
| exp '/' exp        { $$ = $1 / $3;                    }
| '-' exp  %prec NEG { $$ = -$2;                        }
| exp '^' exp        { $$ = pow ($1, $3);               }
| '(' exp ')'        { $$ = $2;                         }
;

%%
