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

void yyerror(const char *s);
int yylex(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

Color default_color = (Color){255,0,0};
Color rcolor = (Color){255,0,0}; //resulting color for track
}

%define api.value.type union /* Generate YYSTYPE from these types: */
%define parse.error detailed //bison v3.6+

%token <int> INT
%token <float> FLOAT
%token ARROW

%type <float> val
%type <float> arith_expr
%type <uint8_t> bool_expr
%type <Color> rgb_color

%left '+' '-'
%left '*' '/'

%%
mapping:
  bool_expr ARROW rgb_color {
    printf("bool_expr eval'd to: %u\n", $1);
    if($1) {
      rcolor=$3;
    } else {rcolor=default_color;}
    printf("Setting color to: {%u,%u,%u}\n", rcolor.r,rcolor.g,rcolor.b);
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
      printf("Parsed RGB: R=%d, G=%d, B=%d\n", $$.r, $$.g, $$.b);
  }
;

val:
  INT   {$$=$1;}
  | FLOAT {$$=$1;}
;

arith_expr:
  val         {$$=$1;}
  | arith_expr '+' arith_expr {$$=$1+$3;}
  | arith_expr '-' arith_expr {$$=$1-$3;}
  | arith_expr '*' arith_expr {$$=$1*$3;}
  | arith_expr '/' arith_expr {$$=$1/$3;}
  | '(' arith_expr ')'        {$$=$2;}
  | '-' arith_expr            {$$=-$2;}
;

bool_expr:
  arith_expr '<' arith_expr       {$$=$1<$3;}
  | arith_expr '>' arith_expr     {$$=$1>$3;}
  | arith_expr '=' '=' arith_expr {$$=$1==$4;}
  | arith_expr '!' '=' arith_expr {$$=$1!=$4;}
  | arith_expr '<' '=' arith_expr {$$=$1<=$4;}
  | arith_expr '>' '=' arith_expr {$$=$1>=$4;}
  | '!' bool_expr                 {$$=!$2;}
  | '(' bool_expr ')'             {$$=$2;}
;

%%

void yyerror(const char *s) {
    /* Stille Fehlerbehandlung oder Logging */
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
