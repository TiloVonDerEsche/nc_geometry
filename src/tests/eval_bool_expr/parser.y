%code requires {
  #include <stdint.h>
  uint8_t is_valid_uint8(int ui);

  typedef struct {
      uint8_t r, g, b;
  } Color;

  extern Color default_color;
  extern Color rcolor;
}

%code {
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int yyerror(const char *s);
int yylex(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

Color default_color = (Color){255,0,0};
Color rcolor = (Color){255,0,0}; //resulting color for track

int debug = 0;
}

%define api.value.type union /* Generate YYSTYPE from these types: */
%define parse.error detailed //bison v3.6+

%token <int> INT
%token <float> FLOAT
%token ARROW
%token OR AND
%token EQ NEQ
%token LTEQ GTEQ

%type <float> val
//%type <float> arith_expr
%type <int> bool_expr
%type <Color> rgb_color

%left OR //Bool Algebra, OR ^= ADD, AND ^= MULT
%left AND // (x < 5) && (y > 10), instead of x < (5 && y) > 10
%left '=' EQ NEQ
%left '<' '>' LTEQ GTEQ
%left '+' '-'
%left '*' '/'
%left '(' ')'
%right '!' UNEG //unary negate
//%nonassoc '<' '>' //forbid: 1<2<3

%%
mapping:
  bool_expr ARROW rgb_color ';' {
    if(debug){printf("bool_expr eval'd to: %d\n", $1);}
    if($1) {
      rcolor=$3;
    } else {rcolor=default_color;}
    if(debug){printf("Setting color to: {%u,%u,%u}\n", rcolor.r,rcolor.g,rcolor.b);}
  }
;

rgb_color:
  '{' INT ',' INT ',' INT '}' {
      if(!is_valid_uint8($2)) {fprintf(stderr,"First argument of rgb_color is not in range [0,255]!\n");}
      if(!is_valid_uint8($4)) {fprintf(stderr,"Second argument of rgb_color is not in range [0,255]!\n");}
      if(!is_valid_uint8($6)) {fprintf(stderr,"Third argument of rgb_color is not in range [0,255]!\n");}
      $$.r = $2;
      $$.g = $4;
      $$.b = $6;
      if(debug){printf("Parsed RGB: R=%d, G=%d, B=%d\n", $$.r, $$.g, $$.b);}
  }
;

val:
  INT   {$$=$1;}
  | FLOAT {$$=$1;}
;

bool_expr:
  val                           {$$=$1;}
  | bool_expr '+' bool_expr     {$$=$1+$3;}
  | bool_expr '-' bool_expr     {$$=$1-$3;}
  | bool_expr '*' bool_expr     {$$=$1*$3;}
  | bool_expr '/' bool_expr     {$$=$1/$3;}
  | bool_expr OR bool_expr      {$$=$1||$3;}
  | bool_expr AND bool_expr     {$$=$1&&$3;}
  | bool_expr EQ bool_expr      {$$=$1==$3;}
  | bool_expr NEQ bool_expr     {$$=$1!=$3;}
  | bool_expr LTEQ bool_expr    {$$=$1<=$3;}
  | bool_expr GTEQ bool_expr    {$$=$1>=$3;}
  | bool_expr '<' bool_expr     {$$=$1<$3;}
  | bool_expr '>' bool_expr     {$$=$1>$3;}
  | '!' bool_expr               {$$=!$2;}
  | '-' bool_expr %prec UNEG    {$$=-$2;}
  | '(' bool_expr ')'           {$$=$2;}
;

//Can't handle:

/*
(1<50 || 50<1)
(val<val || val<val)
(bool_expr || bool_expr)

bool_expr:
  arith_expr '<' arith_expr       {$$=$1<$3;}
  | arith_expr '>' arith_expr     {$$=$1>$3;}
  | arith_expr '<' '=' arith_expr {$$=$1<=$4;}
  | arith_expr '>' '=' arith_expr {$$=$1>=$4;}

  | arith_expr '=' '=' arith_expr {$$=$1==$4;}
  | arith_expr '!' '=' arith_expr {$$=$1!=$4;}

  | arith_expr '|' '|' arith_expr {$$=$1||$4;}
  | arith_expr '&' '&' arith_expr {$$=$1&&$4;}
  | '!' bool_expr                 {$$=!$2;}
  | '(' bool_expr ')'             {$$=$2;}
;*/

%%

int yyerror(const char* s) {
	printf("Error: %s, in line: 0\n", s);
	return 0;
}

Color map_color(const char* expr_str) {
    rcolor = default_color;

    YY_BUFFER_STATE buffer = yy_scan_string(expr_str);
    int parse_status = yyparse();
    yy_delete_buffer(buffer);

    if (parse_status != 0) {
        printf("Syntax-Error in expr: %s\n", expr_str);
    }

    return rcolor;
}

uint8_t is_valid_uint8(int ui) {
  return 0<=ui<=255;
}
