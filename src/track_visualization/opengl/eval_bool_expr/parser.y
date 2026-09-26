%code requires {
  #include <stdint.h>
  #include "../typedefs.h"
  uint8_t is_valid_uint8(int ui);
}
%parse-param { Track* t }

%code {
#include <stdio.h>
#include <stdlib.h>

int yyerror(Track* t, const char *s);
int yylex(void);

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

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

%token T_ID
%token T_START_X T_START_Y T_START_Z
%token T_END_X T_END_Y T_END_Z
%token LASER_POWER MACHINE_SPEED
%token G_CODE
%token HRAD VRAD

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
      t->color=$3;
    } //else {default_color;}
    if(debug){printf("Setting color to: {%u,%u,%u}\n", t->color.r,t->color.g,t->color.b);}
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
  //Track Attrs:
  | T_ID {$$=t->id;}
  | T_START_X {$$=t->ax;}
  | T_START_Y {$$=t->ay;}
  | T_START_Z {$$=t->az;}
  | T_END_X {$$=t->bx;}
  | T_END_Y {$$=t->by;}
  | T_END_Z {$$=t->bz;}
  | LASER_POWER {$$=t->laser_power;}
  | MACHINE_SPEED {$$=t->machine_speed;}
  | G_CODE {$$=t->g_code;}
  | VRAD {$$=t->vradius;}
  | HRAD {$$=t->hradius;}
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

%%

int yyerror(Track* t, const char* s) {
  (void)t; // suppress unused parameter warning
	printf("Error: %s, in line: 0\n", s);
	return 0;
}

//mutate color attr of t
//access other attrs for bool eval
void map_color(const char* expr_str, Track* t_ptr) {
    YY_BUFFER_STATE buffer = yy_scan_string(expr_str);
    int parse_status = yyparse(t_ptr);
    yy_delete_buffer(buffer);

    if (parse_status != 0) {
        printf("Syntax-Error in expr: %s\n", expr_str);
    }

    //return t_ptr->color;
}

uint8_t is_valid_uint8(int ui) {
  return (0<=ui && ui<=255);
}
