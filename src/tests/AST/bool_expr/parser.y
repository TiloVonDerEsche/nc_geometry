%{
#include "ast.h"
#include "typedefs.h"
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
%token VAR_X


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
  | VAR_X         { $$ = new VariableNode(); }
  //Track Attrs:
  /*
  | T_ID {$$=t->id;}
  | T_START_X {$$=t->start_x;}
  | T_START_Y {$$=t->start_y;}
  | T_START_Z {$$=t->start_z;}
  | T_END_X {$$=t->end_x;}
  | T_END_Y {$$=t->end_y;}
  | T_END_Z {$$=t->end_z;}
  | B_CMD   {$$=t->b;}
  | C_CMD   {$$=t->c;}
  | MACHINE_SPEED {$$=t->machine_speed;}
  | LASER_POWER {$$=t->laser_power;}
  | LASER_STATUS{$$=t->laser_status;}
  | G_CODE {$$=t->g_code;}
  | VRAD {$$=t->vradius;}
  | HRAD {$$=t->hradius;}*/
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
    std::cout <<
    "Geben Sie einen Ausdruck mit 'x' ein (z.B. x * 2 + 5) und Strg+D:\n";
    
    // 1. COMPILE PHASE: Parse ONCE from stdin (e.g. typing: x * 2 + 5)
    if (yyparse() == 0 && rootAST) {
        std::cout << "\n--- Generierter Abstract Syntax Tree (AST) ---\n";
        rootAST->print();
        std::cout << "---------------------------------------------\n\n";

        // 2. RUNTIME PHASE: Execute the AST multiple times
        std::cout <<
        "Fuehre den AST mehrfach mit unterschiedlichen Eingaben aus:\n";
        for (double x_input = 1.0; x_input <= 500.0; x_input += 1.0) {
            double result = rootAST->evaluate(x_input);
            std::cout << "Eingabe x = " << x_input << " ---> Ergebnis = "
            << result << "\n";
        }
    } else {
        std::cerr << "Fehler beim Parsen des Ausdrucks.\n";
    }

    return 0;
}
