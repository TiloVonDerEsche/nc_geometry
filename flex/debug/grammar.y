%{
  #include <stdio.h>
  #include <math.h>
  #include <string.h>
  #include <stdlib.h>


  int yylex (void);
  int yyerror (char *);

%}


%define api.value.type union

%token SPACE NEWLINE SET OTHER
%token <int> INT
%token <float> FLOAT


%token <char*> VAR
%token <char*> CMD

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
  exprs                {puts("Test!");}
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
  VAR SET arith_expr     {printf("Assignment: %s=%f\n",$1,$3);

                         }
  | CMD SET arith_expr   { /* Handle simple commands */ }
;


val:
  VAR            {printf("VAR=%s\n",$1); $$=666;}
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

%%

int yyerror(char *s)
{
	printf("Syntax Error on line %s\n", s);
	return 0;
}

int main()
{
    yyparse();
    return 0;
}
