/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "grammar.y"

  #include <stdio.h>
  #include <math.h>

  #include "khashl_helper.h"

  int yylex (void);
  int yyerror(char* s);

  strfloat_t* h;
  khint_t k;
  int absent;

  FILE* hmhis;

  extern long byte_counter;
  extern int skip;
  char* pending_jump_label = NULL;
  int jump_requested = 0;

  void jump(char*);
  void set_var(char*, float);
  float get_var_val(char*);
  void init_hashmap();
  void init_hmhis();


#line 99 "grammar.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "grammar.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEP = 3,                        /* SEP  */
  YYSYMBOL_NEWLINE = 4,                    /* NEWLINE  */
  YYSYMBOL_SEMICOLON = 5,                  /* SEMICOLON  */
  YYSYMBOL_COMMENT = 6,                    /* COMMENT  */
  YYSYMBOL_OTHER = 7,                      /* OTHER  */
  YYSYMBOL_STRING = 8,                     /* STRING  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_FLOAT = 10,                     /* FLOAT  */
  YYSYMBOL_EQ = 11,                        /* EQ  */
  YYSYMBOL_NEQ = 12,                       /* NEQ  */
  YYSYMBOL_LTEQ = 13,                      /* LTEQ  */
  YYSYMBOL_GTEQ = 14,                      /* GTEQ  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ENDIF = 16,                     /* ENDIF  */
  YYSYMBOL_GOTO = 17,                      /* GOTO  */
  YYSYMBOL_VAR = 18,                       /* VAR  */
  YYSYMBOL_CMD = 19,                       /* CMD  */
  YYSYMBOL_SPECIAL_CMD = 20,               /* SPECIAL_CMD  */
  YYSYMBOL_CUSTOM_VAR = 21,                /* CUSTOM_VAR  */
  YYSYMBOL_CALL = 22,                      /* CALL  */
  YYSYMBOL_MISC_ID = 23,                   /* MISC_ID  */
  YYSYMBOL_LABEL = 24,                     /* LABEL  */
  YYSYMBOL_25_ = 25,                       /* '+'  */
  YYSYMBOL_26_ = 26,                       /* '-'  */
  YYSYMBOL_27_ = 27,                       /* '*'  */
  YYSYMBOL_28_ = 28,                       /* '/'  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '('  */
  YYSYMBOL_31_ = 31,                       /* ')'  */
  YYSYMBOL_32_ = 32,                       /* ','  */
  YYSYMBOL_33_ = 33,                       /* '<'  */
  YYSYMBOL_34_ = 34,                       /* '>'  */
  YYSYMBOL_35_ = 35,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 36,                  /* $accept  */
  YYSYMBOL_prog = 37,                      /* prog  */
  YYSYMBOL_lines = 38,                     /* lines  */
  YYSYMBOL_line = 39,                      /* line  */
  YYSYMBOL_opt_skip = 40,                  /* opt_skip  */
  YYSYMBOL_opt_seps = 41,                  /* opt_seps  */
  YYSYMBOL_seps = 42,                      /* seps  */
  YYSYMBOL_exprs = 43,                     /* exprs  */
  YYSYMBOL_expr = 44,                      /* expr  */
  YYSYMBOL_45_1 = 45,                      /* $@1  */
  YYSYMBOL_if_body = 46,                   /* if_body  */
  YYSYMBOL_if_element = 47,                /* if_element  */
  YYSYMBOL_assignment = 48,                /* assignment  */
  YYSYMBOL_val = 49,                       /* val  */
  YYSYMBOL_fn = 50,                        /* fn  */
  YYSYMBOL_params = 51,                    /* params  */
  YYSYMBOL_arith_expr = 52,                /* arith_expr  */
  YYSYMBOL_bool_expr = 53                  /* bool_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  101

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,     2,     2,     2,     2,     2,
      30,    31,    27,    25,    32,    26,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      33,    29,    34,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    57,    57,    61,    62,    66,    67,    81,    85,    86,
      90,    91,    95,    96,   100,   101,   105,   105,   116,   117,
     118,   121,   130,   138,   139,   140,   141,   145,   146,   150,
     151,   152,   157,   158,   159,   160,   165,   169,   173,   174,
     178,   182,   183,   184,   185,   186,   190,   191,   192,   193,
     194,   195,   196,   197,   201,   202,   203,   204,   205,   206,
     207,   208
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEP", "NEWLINE",
  "SEMICOLON", "COMMENT", "OTHER", "STRING", "INT", "FLOAT", "EQ", "NEQ",
  "LTEQ", "GTEQ", "IF", "ENDIF", "GOTO", "VAR", "CMD", "SPECIAL_CMD",
  "CUSTOM_VAR", "CALL", "MISC_ID", "LABEL", "'+'", "'-'", "'*'", "'/'",
  "'='", "'('", "')'", "','", "'<'", "'>'", "'!'", "$accept", "prog",
  "lines", "line", "opt_skip", "opt_seps", "seps", "exprs", "expr", "$@1",
  "if_body", "if_element", "assignment", "val", "fn", "params",
  "arith_expr", "bool_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-45)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-9)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -45,     3,    74,   -45,   -45,   -45,     8,    22,   -45,   -45,
     211,    25,   -45,    30,    31,     2,   135,   -45,     1,    32,
       9,   -45,    22,   -45,   -45,   -45,   -45,    28,    17,   170,
     -45,   -45,   -45,   -45,     9,   170,   170,   170,   -45,   -45,
      56,   170,   170,    39,   185,   -45,   119,    28,    28,   212,
     -45,   -45,    56,   -22,    56,    34,   170,   170,   170,   170,
      56,    56,   -45,     9,   -13,    56,   -45,   -18,    19,   -45,
      23,   145,   160,    24,   -45,   -45,   -22,   -22,   -45,   -45,
     -45,   195,   -45,   170,   170,    56,   170,    56,   170,    97,
       9,    56,    56,    56,    56,    56,   -45,   -45,   -45,   -45,
     -45
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     7,     9,     0,    10,     4,    12,
       5,    11,    26,     0,     0,     0,     0,    22,     0,     0,
      24,    20,    10,    14,    19,    25,    13,     0,     0,     0,
      38,    39,    36,    37,     0,     0,     0,     0,    46,    47,
      18,     0,     0,     0,    41,     6,    11,     0,     0,     0,
      16,    21,    32,    53,    33,     0,     0,     0,     0,     0,
      35,    34,    23,    43,     0,    42,    15,     0,     0,    60,
       0,     0,     0,     0,    27,    52,    48,    49,    50,    51,
      40,     0,    61,     0,     0,    54,     0,    55,     0,     0,
      45,    44,    56,    58,    59,    57,    30,    31,    17,    29,
      28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,   -45,   -45,   -45,    26,    33,   -45,   -44,   -45,
     -45,   -45,   -45,   -45,   -10,   -45,   -15,   -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     6,     7,    10,    11,    22,    23,    74,
      89,   100,    24,    38,    39,    64,    49,    50
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      25,    40,    66,     3,    41,    58,    59,    56,    57,    58,
      59,    70,     8,    75,    52,    71,    72,    73,    80,    81,
      53,    54,    55,    68,    69,     9,    60,    61,    26,    65,
      42,    29,    67,    27,    28,    43,    25,    30,    31,    44,
      51,    76,    77,    78,    79,    99,    32,    62,    45,    33,
      82,    34,    83,    88,    35,    46,    85,    87,    47,    56,
      57,    58,    59,    48,     0,    75,    91,     0,    92,    93,
       0,    94,     0,    95,    -2,     4,     0,    -8,    -8,    25,
      -8,    56,    57,    58,    59,     0,     0,     0,     0,    -8,
       0,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,     0,
      96,    97,     5,    12,     0,     0,     0,     0,     0,     0,
       0,     0,    13,    98,    14,    15,    16,    17,    18,    19,
      20,    21,    26,     0,     0,    12,     0,     0,     0,     0,
       0,     0,     0,     0,    13,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    30,    31,     0,     0,     0,     0,
       0,     0,     0,    32,    30,    31,    33,     0,    34,     0,
       0,    35,     0,    32,    36,    37,    33,     0,    34,    30,
      31,    35,     0,     0,    84,    37,     0,     0,    32,    30,
      31,    33,     0,    34,     0,     0,    35,     0,    32,    86,
      37,    33,     0,    34,    30,    31,    35,     0,     0,     0,
      37,     0,     0,    32,    30,    31,    33,     0,    63,     0,
       0,    35,     0,    32,     0,    37,    33,    12,    90,     0,
       0,    35,     0,     0,     0,    37,    13,     0,    14,    15,
      16,    17,    18,    19,    20,    21,     0,    56,    57,    58,
      59,    70,     0,     0,     0,    71,    72,    73
};

static const yytype_int8 yycheck[] =
{
      10,    16,    46,     0,     3,    27,    28,    25,    26,    27,
      28,    29,     4,    31,    29,    33,    34,    35,    31,    32,
      35,    36,    37,    47,    48,     3,    41,    42,     3,    44,
      29,    29,    47,     3,     3,     3,    46,     9,    10,    30,
      23,    56,    57,    58,    59,    89,    18,     8,    22,    21,
      31,    23,    29,    29,    26,    22,    71,    72,    30,    25,
      26,    27,    28,    35,    -1,    31,    81,    -1,    83,    84,
      -1,    86,    -1,    88,     0,     1,    -1,     3,     4,    89,
       6,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
       3,     4,    28,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     3,    -1,    -1,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,     9,    10,    21,    -1,    23,    -1,
      -1,    26,    -1,    18,    29,    30,    21,    -1,    23,     9,
      10,    26,    -1,    -1,    29,    30,    -1,    -1,    18,     9,
      10,    21,    -1,    23,    -1,    -1,    26,    -1,    18,    29,
      30,    21,    -1,    23,     9,    10,    26,    -1,    -1,    -1,
      30,    -1,    -1,    18,     9,    10,    21,    -1,    23,    -1,
      -1,    26,    -1,    18,    -1,    30,    21,     6,    23,    -1,
      -1,    26,    -1,    -1,    -1,    30,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    25,    26,    27,
      28,    29,    -1,    -1,    -1,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    37,    38,     0,     1,    28,    39,    40,     4,     3,
      41,    42,     6,    15,    17,    18,    19,    20,    21,    22,
      23,    24,    43,    44,    48,    50,     3,     3,     3,    29,
       9,    10,    18,    21,    23,    26,    29,    30,    49,    50,
      52,     3,    29,     3,    30,    41,    42,    30,    35,    52,
      53,    23,    52,    52,    52,    52,    25,    26,    27,    28,
      52,    52,     8,    23,    51,    52,    44,    52,    53,    53,
      29,    33,    34,    35,    45,    31,    52,    52,    52,    52,
      31,    32,    31,    29,    29,    52,    29,    52,    29,    46,
      23,    52,    52,    52,    52,    52,     3,     4,    16,    44,
      47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    38,    38,    39,    39,    39,    40,    40,
      41,    41,    42,    42,    43,    43,    45,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    46,    46,    47,
      47,    47,    48,    48,    48,    48,    49,    49,    49,    49,
      50,    51,    51,    51,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    53,    53,    53,    53,    53,    53,
      53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     3,     2,     4,     1,     0,     1,
       0,     1,     1,     2,     1,     3,     0,     6,     2,     1,
       1,     3,     1,     3,     1,     1,     1,     0,     2,     1,
       1,     1,     3,     3,     3,     3,     1,     1,     1,     1,
       4,     0,     1,     1,     3,     3,     1,     1,     3,     3,
       3,     3,     3,     2,     3,     3,     4,     4,     4,     4,
       2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* lines: lines line NEWLINE  */
#line 62 "grammar.y"
                       {set_var("line",get_var_val("line")+1);}
#line 1231 "grammar.tab.c"
    break;

  case 6: /* line: opt_skip opt_seps exprs opt_seps  */
#line 68 "grammar.y"
                        {
                         print_hashmap(h, hmhis);
                         print_hashmap(h, stdout);

                         if (jump_requested) {
                              jump_requested = 0;
                              skip = 0; // Reset skip before jumping
                              char* target = pending_jump_label;
                              pending_jump_label = NULL;
                              jump(target);
                              free(target);
                         }
                        }
#line 1249 "grammar.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 105 "grammar.y"
                   {
      if (!(yyvsp[0].bool_expr)) {
          skip++;
          printf("Skip=%d\n",skip);
      }
  }
#line 1260 "grammar.tab.c"
    break;

  case 17: /* expr: IF SEP bool_expr $@1 if_body ENDIF  */
#line 110 "grammar.y"
                  {
      if (skip > 0) {
        skip--;
        printf("Skip=%d\n",skip);
      }
    }
#line 1271 "grammar.tab.c"
    break;

  case 18: /* expr: CMD arith_expr  */
#line 116 "grammar.y"
                         {if(!skip){ set_var((yyvsp[-1].CMD),(yyvsp[0].arith_expr));}}
#line 1277 "grammar.tab.c"
    break;

  case 20: /* expr: LABEL  */
#line 118 "grammar.y"
                         {
                          if(!skip){ set_var((yyvsp[0].LABEL), (float)byte_counter); }
                         }
#line 1285 "grammar.tab.c"
    break;

  case 21: /* expr: GOTO SEP MISC_ID  */
#line 121 "grammar.y"
                       {if(!skip){
                          pending_jump_label = strdup((yyvsp[0].MISC_ID));
                          jump_requested = 1;

                          // Increment skip so the parser ignores everything
                          // until it reaches the end of the current IF or line
                          skip = 1;
                          }
                        }
#line 1299 "grammar.tab.c"
    break;

  case 22: /* expr: SPECIAL_CMD  */
#line 130 "grammar.y"
                         {
                          if(strcmp((yyvsp[0].SPECIAL_CMD),"LASER_ON") == 0) {
                            set_var("laser",1);
                          }
                          else if(strcmp((yyvsp[0].SPECIAL_CMD),"LASER_OFF") == 0) {
                            set_var("laser",0);
                          }
                         }
#line 1312 "grammar.tab.c"
    break;

  case 32: /* assignment: VAR '=' arith_expr  */
#line 157 "grammar.y"
                              {if(!skip){set_var((yyvsp[-2].VAR),(yyvsp[0].arith_expr));}}
#line 1318 "grammar.tab.c"
    break;

  case 33: /* assignment: CMD '=' arith_expr  */
#line 158 "grammar.y"
                              {if(!skip){set_var((yyvsp[-2].CMD),(yyvsp[0].arith_expr));}}
#line 1324 "grammar.tab.c"
    break;

  case 34: /* assignment: CUSTOM_VAR '=' arith_expr  */
#line 159 "grammar.y"
                              {if(!skip){set_var((yyvsp[-2].CUSTOM_VAR),(yyvsp[0].arith_expr));}}
#line 1330 "grammar.tab.c"
    break;

  case 35: /* assignment: CUSTOM_VAR SEP arith_expr  */
#line 160 "grammar.y"
                              {if(!skip){set_var((yyvsp[-2].CUSTOM_VAR),(yyvsp[0].arith_expr));}}
#line 1336 "grammar.tab.c"
    break;

  case 36: /* val: VAR  */
#line 165 "grammar.y"
                 {
                  /*printf("Getting VAR=%s\n",$1);*/
                  (yyval.val) = get_var_val((yyvsp[0].VAR));
                 }
#line 1345 "grammar.tab.c"
    break;

  case 37: /* val: CUSTOM_VAR  */
#line 169 "grammar.y"
                 {
                  /*printf("Getting CUSTOM_VAR=%s\n",$1);*/
                  (yyval.val) = get_var_val((yyvsp[0].CUSTOM_VAR));
                 }
#line 1354 "grammar.tab.c"
    break;

  case 38: /* val: INT  */
#line 173 "grammar.y"
                 {(yyval.val)=(yyvsp[0].INT);}
#line 1360 "grammar.tab.c"
    break;

  case 39: /* val: FLOAT  */
#line 174 "grammar.y"
                 {(yyval.val)=(yyvsp[0].FLOAT);}
#line 1366 "grammar.tab.c"
    break;

  case 40: /* fn: MISC_ID '(' params ')'  */
#line 178 "grammar.y"
                         {(yyval.fn)=0;}
#line 1372 "grammar.tab.c"
    break;

  case 46: /* arith_expr: val  */
#line 190 "grammar.y"
              {(yyval.arith_expr)=(yyvsp[0].val);}
#line 1378 "grammar.tab.c"
    break;

  case 47: /* arith_expr: fn  */
#line 191 "grammar.y"
              {(yyval.arith_expr)=(yyvsp[0].fn);}
#line 1384 "grammar.tab.c"
    break;

  case 48: /* arith_expr: arith_expr '+' arith_expr  */
#line 192 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)+(yyvsp[0].arith_expr); /*printf("%f+%f=%f\n", $1,$3,$$);*/}
#line 1390 "grammar.tab.c"
    break;

  case 49: /* arith_expr: arith_expr '-' arith_expr  */
#line 193 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)-(yyvsp[0].arith_expr);}
#line 1396 "grammar.tab.c"
    break;

  case 50: /* arith_expr: arith_expr '*' arith_expr  */
#line 194 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)*(yyvsp[0].arith_expr);}
#line 1402 "grammar.tab.c"
    break;

  case 51: /* arith_expr: arith_expr '/' arith_expr  */
#line 195 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)/(yyvsp[0].arith_expr);}
#line 1408 "grammar.tab.c"
    break;

  case 52: /* arith_expr: '(' arith_expr ')'  */
#line 196 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-1].arith_expr);}
#line 1414 "grammar.tab.c"
    break;

  case 53: /* arith_expr: '-' arith_expr  */
#line 197 "grammar.y"
                              {(yyval.arith_expr)=-(yyvsp[0].arith_expr);}
#line 1420 "grammar.tab.c"
    break;

  case 54: /* bool_expr: arith_expr '<' arith_expr  */
#line 201 "grammar.y"
                            {(yyval.bool_expr)=(yyvsp[-2].arith_expr)<(yyvsp[0].arith_expr);}
#line 1426 "grammar.tab.c"
    break;

  case 55: /* bool_expr: arith_expr '>' arith_expr  */
#line 202 "grammar.y"
                              {(yyval.bool_expr)=(yyvsp[-2].arith_expr)>(yyvsp[0].arith_expr);}
#line 1432 "grammar.tab.c"
    break;

  case 56: /* bool_expr: arith_expr '=' '=' arith_expr  */
#line 203 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)==(yyvsp[0].arith_expr);}
#line 1438 "grammar.tab.c"
    break;

  case 57: /* bool_expr: arith_expr '!' '=' arith_expr  */
#line 204 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)!=(yyvsp[0].arith_expr);}
#line 1444 "grammar.tab.c"
    break;

  case 58: /* bool_expr: arith_expr '<' '=' arith_expr  */
#line 205 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)<=(yyvsp[0].arith_expr);}
#line 1450 "grammar.tab.c"
    break;

  case 59: /* bool_expr: arith_expr '>' '=' arith_expr  */
#line 206 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)>=(yyvsp[0].arith_expr);}
#line 1456 "grammar.tab.c"
    break;

  case 60: /* bool_expr: '!' bool_expr  */
#line 207 "grammar.y"
                                  {(yyval.bool_expr)=!(yyvsp[0].bool_expr);}
#line 1462 "grammar.tab.c"
    break;

  case 61: /* bool_expr: '(' bool_expr ')'  */
#line 208 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-1].bool_expr);}
#line 1468 "grammar.tab.c"
    break;


#line 1472 "grammar.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 212 "grammar.y"


void jump(char* label_name) {
    //size_t llen = strlen(label_name);
    //printf("\nlabel_len=%zu\n",llen);

    float offset = get_var_val(label_name) + strlen(label_name) + 2;


    if (debug) {
      k = strfloat_get(h, label_name);
      if ( kh_exist(h, k) ) {
        printf("Found in hm: label_name=%s\n",label_name);
        //offset = kh_val(h, k);
      }
      else {
        printf("NOT found in hm: label_name=%s\n",label_name);
      }
    }

    if(debug){ printf("Jumping to offset=%d\n\n",(long)offset); }


    if (offset >= 0) {
        fseek(yyin, (long)offset, SEEK_SET);
        yyrestart(yyin); //Tells Flex to flush buffers and read from yyin again
    }

    //printf("fgetc=%c\n\n",fgetc(yyin));

}


//hashmap fns

void set_var(char* varname, float fnum) {
  k = strfloat_put(h, varname, &absent);
  if (absent) {
    kh_key(h, k) = strdup(varname);}
  kh_val(h, k) = fnum;
  //printf("Set %s to %f\n", varname, fnum);
}

float get_var_val(char* varname) {
  k = strfloat_get(h, varname);
  if ( kh_exist(h, k) ) {
    return kh_val(h, k);
  }

  return 0;
}


void init_hashmap() {
  khint_t k;
  int absent;

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 1;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");
  kh_val(h, k) = 0;

  //print_hashmap(h, stdout);
}

void init_hmhis() {
  printf("Opening: hmhis.json in write mode...\n");
  hmhis = fopen("./data/hmhis.json", "w");
  if (hmhis == NULL) {
      fprintf(stderr, "Error: Could not open hmhis.json (in write mode)!\n");
      return;
  }
  fprintf(hmhis,"[");

}

void close_hmhis() {
  //delete last redundant comma
  fseek(hmhis, -3, SEEK_CUR);
  fprintf(hmhis,"]");
  fclose(hmhis);
}

//bison fns

int yyerror(char* s)
{
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}
