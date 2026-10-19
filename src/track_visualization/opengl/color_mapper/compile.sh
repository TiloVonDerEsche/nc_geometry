# 1. Bison-Parser generieren (erzeugt parser.tab.c und parser.tab.h)
bison -d parser.y

# 2. Flex-Lexer generieren (erzeugt lex.yy.c)
flex lexer.l

# 3. Alle C++ Dateien zusammen kompilieren
g++ -std=c++17 -Wall cm.c lex.yy.c -o ast_parser.bin
