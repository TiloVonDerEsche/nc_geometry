%{
#include "ast.h"
#include "parser.tab.h"  // Wird von Bison generiert und enthält die Token-Definitionen
#include <cstdlib>

void yyerror(const char *s);
%}

%option noyywrap

%%

[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
"x"            { return VAR_X; }   // NEW: Recognize variable x
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"("             { return '('; }
")"             { return ')'; }
[ \t\n]         { /* Ignoriere Whitespaces, Tabs und Zeilenumbrüche */ }
.               { std::cerr << "Unbekanntes Zeichen: " << yytext[0] << "\n"; }

%%
