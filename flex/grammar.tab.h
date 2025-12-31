/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SPACE = 258,                   /* SPACE  */
    NEWLINE = 259,                 /* NEWLINE  */
    SET = 260,                     /* SET  */
    SEMICOLON = 261,               /* SEMICOLON  */
    COMMENT = 262,                 /* COMMENT  */
    OTHER = 263,                   /* OTHER  */
    STRING = 264,                  /* STRING  */
    INT = 265,                     /* INT  */
    FLOAT = 266,                   /* FLOAT  */
    EQ = 267,                      /* EQ  */
    NEQ = 268,                     /* NEQ  */
    LTEQ = 269,                    /* LTEQ  */
    GTEQ = 270,                    /* GTEQ  */
    IF = 271,                      /* IF  */
    GOTO = 272,                    /* GOTO  */
    VAR = 273,                     /* VAR  */
    CMD = 274,                     /* CMD  */
    SPECIAL_CMD = 275,             /* SPECIAL_CMD  */
    CUSTOM_VAR = 276,              /* CUSTOM_VAR  */
    CALL = 277,                    /* CALL  */
    MISC_ID = 278,                 /* MISC_ID  */
    LABEL = 279                    /* LABEL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
  char* STRING;                            /* STRING  */
  char* VAR;                               /* VAR  */
  char* CMD;                               /* CMD  */
  char* SPECIAL_CMD;                       /* SPECIAL_CMD  */
  char* CUSTOM_VAR;                        /* CUSTOM_VAR  */
  char* MISC_ID;                           /* MISC_ID  */
  char* LABEL;                             /* LABEL  */
  float FLOAT;                             /* FLOAT  */
  float val;                               /* val  */
  float fn;                                /* fn  */
  float arith_expr;                        /* arith_expr  */
  int INT;                                 /* INT  */

#line 101 "grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
