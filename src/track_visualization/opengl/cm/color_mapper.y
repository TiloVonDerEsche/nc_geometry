%{
#include "ast.h"
#include <stdint.h>
#include <iostream>
#include <memory>

extern int yylex();
extern int yyparse();
void yyerror(const char *s);
uint8_t is_valid_uint8(int ui);

// Globaler Pointer für die Wurzel des ASTs
std::unique_ptr<ASTNode> rootAST = nullptr;
MappingNode* prevMapNode = nullptr;

int debug = 0;
%}

%define api.value.type union /* Generate YYSTYPE from these types: */

%token <int> INUM
%token <float> FNUM
%token ARROW
%token OR AND
%token EQ NEQ
%token LTEQ GTEQ

%token T_ID
%token T_START_X T_START_Y T_START_Z
%token T_END_X T_END_Y T_END_Z
%token B_CMD C_CMD
%token MACHINE_SPEED
%token LASER_POWER LASER_STATUS
%token G_CODE
%token HRAD VRAD


//%type <float> arith_expr
%type <Color> rgb_color
%type <ASTNode*> val
%type <ASTNode*> mapping
%type <ASTNode*> bool_expr


%left OR //Bool Algebra, OR ^= ADD, AND ^= MULT
%left AND // (x < 5) && (y > 10), instead of x < (5 && y) > 10
%left '=' EQ NEQ
%left '<' '>' LTEQ GTEQ
%left '+' '-'
%left '*' '/'
%left '(' ')'
%right '!' UNEG //unary negate

%%

maps:
    %empty
    | maps mapping
;

mapping:
  bool_expr ARROW rgb_color ';' {
    MappingNode* newNode = new MappingNode($1, $3);
    $$ = newNode;

    if (!rootAST) {
        // Index 0: First mapping becomes the root of the AST/linked list
        rootAST = std::unique_ptr<ASTNode>(newNode);
        prevMapNode = newNode;
    } else {
        // Append subsequent mapping nodes to the linked list
        prevMapNode->next = std::unique_ptr<MappingNode>(newNode);
        prevMapNode = newNode;
    }
  }
;

rgb_color:
  '{' INUM ',' INUM ',' INUM '}' {
      if(!is_valid_uint8($2)) {fprintf(stderr,"First argument of rgb_color: %d, is not in range [0,255]!\n",$2);}
      if(!is_valid_uint8($4)) {fprintf(stderr,"Second argument of rgb_color: %d, is not in range [0,255]!\n",$4);}
      if(!is_valid_uint8($6)) {fprintf(stderr,"Third argument of rgb_color: %d, is not in range [0,255]!\n",$6);}
      $$.r = $2;
      $$.g = $4;
      $$.b = $6;
      if(debug){printf("Parsed RGB: R=%d, G=%d, B=%d\n", $$.r, $$.g, $$.b);}
  }
;

val:
  INUM   { $$ = new NumberNode($1); }
  | FNUM { $$ = new NumberNode($1); /*TODO resolve INT / FLOAT problem*/}
  //Track Attributes:
  //NOTE arbitrary 0 to 14, mapping for switch case VariableNode,
  //NOTE to avoid string compare if/else chain
  | T_ID          {$$ = new VariableNode("t_id"); }
  | T_START_X     {$$ = new VariableNode("t_start_x"); }
  | T_START_Y     {$$ = new VariableNode("t_start_y"); }
  | T_START_Z     {$$ = new VariableNode("t_start_z"); }
  | T_END_X       {$$ = new VariableNode("t_end_x"); }
  | T_END_Y       {$$ = new VariableNode("t_end_y"); }
  | T_END_Z       {$$ = new VariableNode("t_end_z"); }
  | B_CMD         {$$ = new VariableNode("B"); }
  | C_CMD         {$$ = new VariableNode("C"); }
  | MACHINE_SPEED {$$ = new VariableNode("machine_speed"); }
  | LASER_POWER   {$$ = new VariableNode("laser_power");}
  | LASER_STATUS  {$$ = new VariableNode("laser_status");}
  | G_CODE        {$$ = new VariableNode("G");}
  | HRAD          {$$ = new VariableNode("hradius");}
  | VRAD          {$$ = new VariableNode("vradius");}

;

bool_expr:
  val                           {$$ = $1;}
  | bool_expr '+' bool_expr     {$$ = new BinaryOpNode('+', $1, $3);}
  | bool_expr '-' bool_expr     {$$ = new BinaryOpNode('-', $1, $3);}
  | bool_expr '*' bool_expr     {$$ = new BinaryOpNode('*', $1, $3);}
  | bool_expr '/' bool_expr     {$$ = new BinaryOpNode('/', $1, $3);}
  | bool_expr OR bool_expr      {$$ = new BinaryOpNode('O', $1, $3);}
  | bool_expr AND bool_expr     {$$ = new BinaryOpNode('A', $1, $3);}
  | bool_expr EQ bool_expr      {$$ = new BinaryOpNode('E', $1, $3);}
  | bool_expr NEQ bool_expr     {$$ = new BinaryOpNode('N', $1, $3);}
  | bool_expr LTEQ bool_expr    {$$ = new BinaryOpNode('L', $1, $3);}
  | bool_expr GTEQ bool_expr    {$$ = new BinaryOpNode('G', $1, $3);}
  | bool_expr '<' bool_expr     {$$ = new BinaryOpNode('<', $1, $3);}
  | bool_expr '>' bool_expr     {$$ = new BinaryOpNode('>', $1, $3);}
  | '!' bool_expr               {$$ = new UnaryOpNode('!', $2);}
  | '-' bool_expr %prec UNEG    {$$ = new UnaryOpNode('-', $2);}
  | '(' bool_expr ')'           {$$=$2;} //Discard parants
;


%%
void yyerror(const char *s) { std::cerr << "Fehler: " << s << "\n"; }

uint8_t is_valid_uint8(int ui) {
  return (0<=ui && ui<=255);
}
