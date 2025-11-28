%{
  #include <stdio.h>  /* For printf, etc. */
  #include <math.h>   /* For pow, used in the grammar. */
  #include "calc.h"   /* Contains definition of 'symrec'. */
  #include "helper.h"
  #include "helper_fns.h"

  #define CMD_BUF_LEN 20
  #define FNUM_BUF_LEN 20
  char cmd_buf[CMD_BUF_LEN] = {};
  char fnum_buf[FNUM_BUF_LEN] = {};

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
%nterm <double> exp

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
              print_hashmap(h,stdout);
             }
| error '\n' { yyerrok; }
;


exp:
  NUM
| VAR NUM            {
                      puts("VAR NUM case!");
                      if (strcmp($1,"PUIS_LASER") == 0) {
                        printf("PUIS_LASER=%f",$2);
                        k = strfloat_put(h, "laser_power", &absent);
                        kh_val(h, k) = $2;
                      }
                     }
| VAR                {
                      //variable
                      k = strfloat_get(h, $1);
                      if ( kh_exist(h, k) ) {
                        $$ = kh_val(h, k);
                      }

                      //cmd_w_fnum
                      else {
                        puts("CMD_W_FNUM case!!");
                        split_cmd_w_fnum($1,cmd_buf,fnum_buf);

                        //if(is_valid_fnum)
                        printf("cmd=%s, fnum=%s\n",cmd_buf,fnum_buf);

                        k = strfloat_put(h, cmd_buf, &absent);

                        if (absent) {
                          kh_key(h, k) = strdup(cmd_buf);}

                        kh_val(h, k) = atof(fnum_buf);
                      }

                     }
| VAR '=' exp        {k = strfloat_put(h, $1, &absent);
                      if (absent) {
                        kh_key(h, k) = strdup($1);}
                      kh_val(h, k) = $3;
                      $$ = $3;
                     }
| '/' VAR             {puts("SPECIAL CMD!");
                       if(strcmp($2,"LASER_ON") == 0) {
                         k = strfloat_put(h, "laser", &absent);
                         kh_val(h, k) = 1;
                       }
                       else if(strcmp($2,"LASER_OFF") == 0) {
                         k = strfloat_put(h, "laser", &absent);
                         kh_val(h, k) = 0;
                       }

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
