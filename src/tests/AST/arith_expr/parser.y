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
%}

// Steuerung der Typen für modernere Bison C++ Parser
%union {
    int num;
    ASTNode* node;
}

%token <num> NUMBER
%token VAR_X
%type <node> expr

%left '+' '-'
%left '*' '/'
%left '(' ')'
%right '!' UNEG

%%

program:
    expr { rootAST = std::unique_ptr<ASTNode>($1); }
    ;

expr:
    NUMBER          { $$ = new NumberNode($1); }
    | VAR_X         { $$ = new VariableNode(); }
    | expr '+' expr { $$ = new BinaryOpNode('+', $1, $3); }
    | expr '-' expr { $$ = new BinaryOpNode('-', $1, $3); }
    | expr '*' expr { $$ = new BinaryOpNode('*', $1, $3); }
    | expr '/' expr { $$ = new BinaryOpNode('/', $1, $3); }
    | '-' expr %prec UNEG {$$ = new UnaryOpNode('-', $2);}
    | '(' expr ')'  { $$ = $2; } // Klammern fallen im AST weg!
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

        // 2. RUNTIME PHASE: Execute the AST 500,000 times
        std::cout <<
        "Fuehre den AST mehrfach mit unterschiedlichen Eingaben aus:\n";
        for (double x_input = 1.0; x_input <= 500000.0; x_input += 1.0) {
            double result = rootAST->evaluate(x_input);
            std::cout << "Eingabe x = " << x_input << " ---> Ergebnis = "
            << result << "\n";
        }
    } else {
        std::cerr << "Fehler beim Parsen des Ausdrucks.\n";
    }

    return 0;
}
