%{
#include "ast.h"
#include <iostream>
#include <memory>

extern int yylex();
extern int yyparse();
void yyerror(const char *s);

// Globaler Pointer für die Wurzel des ASTs
std::unique_ptr<ASTNode> rootAST = nullptr;
// Das finale Wurzel-Objekt des ASTs
//ASTNode* rootAST;

int debug = 0;
%}

// Steuerung der Typen für modernere Bison C++ Parser
%union {
    ASTNode* node;
    int INUM;
    float FNUM;
}

%token <INUM> INUM
%token <FNUM> FNUM
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
//%type <Color> rgb_color
%type <node> val
%type <node> bool_expr


%left OR //Bool Algebra, OR ^= ADD, AND ^= MULT
%left AND // (x < 5) && (y > 10), instead of x < (5 && y) > 10
%left '=' EQ NEQ
%left '<' '>' LTEQ GTEQ
%left '+' '-'
%left '*' '/'
%left '(' ')'
%right '!' UNEG //unary negate

%%

program:
    bool_expr { rootAST = std::unique_ptr<ASTNode>($1); }
;

val:
  INUM   { $$ = new NumberNode($1); }
  | FNUM { $$ = new NumberNode($1); /*TODO resolve INT / FLOAT problem*/}
  //Track Attributes:
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

int main() {
  Track t = (Track){12,         //id
             2.3, 11.2, -7.9,   //start
             5.12, 17.10, -0.9, //end
             20,350.9,          //b,c
             200, 1750,         //machine_speed, laser_power
             1,                 //laser_status
             0,                 //g_code
             (Color){0,0,0},
             0.5,0.5
            };



    // 1. COMPILE PHASE: Parse ONCE from stdin TODO read color_config
    if (yyparse() == 0 && rootAST) {
        std::cout << "\n--- Generierter Abstract Syntax Tree (AST) ---\n";
        rootAST->print();
        std::cout << "---------------------------------------------\n\n";


        // 2. RUNTIME PHASE: Execute the AST multiple times
        //TODO Read a track in, return mapped Color
        //for (double x_input = 1.0; x_input <= 500.0; x_input += 1.0) {
        double result = rootAST->evaluate(&t);
        std::cout << "Eingabe Track t = _"" ---> Ergebnis = "
        << result << "\n";
        //}
    } else {
        std::cerr << "Fehler beim Parsen des Ausdrucks.\n";
    }

    return 0;
}
