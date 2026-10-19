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
ASTNode* prevMapNode = nullptr;

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
    | maps mapping { rootAST = std::unique_ptr<ASTNode>($2); }
;

mapping:
  bool_expr ARROW rgb_color ';' {
    $$ = new MappingNode($1,$3);
    prevMapNode = $$;
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
  | T_ID          {$$ = new VariableNode(0); }
  | T_START_X     {$$ = new VariableNode(1); }
  | T_START_Y     {$$ = new VariableNode(2); }
  | T_START_Z     {$$ = new VariableNode(3); }
  | T_END_X       {$$ = new VariableNode(4); }
  | T_END_Y       {$$ = new VariableNode(5); }
  | T_END_Z       {$$ = new VariableNode(6); }
  | B_CMD         {$$ = new VariableNode(7); }
  | C_CMD         {$$ = new VariableNode(8); }
  | MACHINE_SPEED {$$ = new VariableNode(9); }
  | LASER_POWER   {$$ = new VariableNode(10);}
  | LASER_STATUS  {$$ = new VariableNode(11);}
  | G_CODE        {$$ = new VariableNode(12);}
  | VRAD          {$$ = new VariableNode(13);}
  | HRAD          {$$ = new VariableNode(14);}
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

Color eval(Track t) {
  double result = rootAST->evaluate(&t);
  std::cout << "Eingabe Track=\n"
  "t_id=" << t.id << "\n" <<
  "t_start=(" << t.start_x << ", " << t.start_y << ", " << t.start_z << ")\n" <<
  "t_end=(" << t.end_x << ", " << t.end_y << ", " << t.end_z << ")\n" <<
  "t_b=" << t.b << " t_c=" << t.c << "\n" <<
  "machine_speed=" << t.machine_speed << "\n" <<
  "laser_power=" << t.laser_power << " laser_status=" << t.laser_status << "\n" <<
  "g_code=" << t.g_code << "\n" <<
  "color=(" << (int)t.color.r << ", " << (int)t.color.g << ", " << (int)t.color.b << ")\n" <<
  "t_hrad=" << t.hradius << " t_vrad=" << t.vradius << "\n" <<
  " ---> Ergebnis = "
  << result << "\n"
  << "---------------------------\n\n";

  return (Color){0,0,0};
}

int main() {
  Track t1 = (Track){1,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             20,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };
  Track t2 = (Track){2,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             40,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };
  Track t3 = (Track){3,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             60,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){66,77,88},
             0.5,0.5
            };

    //t_id==1->{1,0,0};t_id==2->{0,1,0};t_id==3->{0,0,1};

    // 1. COMPILE PHASE: Parse ONCE from stdin TODO read color_config
    if (yyparse() == 0 && rootAST) {
        std::cout << "\n--- Generierter Abstract Syntax Tree (AST) ---\n";
        rootAST->print();
        std::cout << "---------------------------------------------\n\n";


        // 2. RUNTIME PHASE: Execute the AST multiple times
        //TODO Read a track in, return mapped Color

        eval(t1);
        eval(t2);
        eval(t3);
    } else {
        std::cerr << "Fehler beim Parsen des Ausdrucks.\n";
    }

    return 0;
}
