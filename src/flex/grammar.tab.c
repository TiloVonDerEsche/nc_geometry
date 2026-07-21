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

  #include "helper.h"
  #include "stack.c"

  extern Config config;
  extern int debug;
  extern strfloat_t* h;

  extern FILE* hmhis;
  extern FILE* tl;

  extern FILE* yyin;

  extern void yyrestart(FILE*);

  int yylex (void);
  int yyerror(char* s);

  int is_coord(char);
  vec3D net_point();

  strfloat_t* init_hashmap();
  float get_var_val(char*);
  void set_var_rot(char, float);
  void set_var_incr(char*, float);
  void set_var(char*, float);

  void write_track_line();
  void modify_tl();

  void request_jump(char*);
  void jump(size_t,long);
  void handle_repeat(char*, char*, size_t);
  void handle_tracks_def_by_coord_lines();

  size_t target_line = 0;
  long target_byte_offset = 0;

  int jump_requested = 0;
  int skip = 0;
  int incr_mode = 0;
  int rot_mode = 0;

  int track_written = 0; //bad name, this is a counter to skip lines

  size_t tid = 0;
  vec3D t_start = {0,0,0};
  vec3D t_end = {0,0,0};

  Stack ret_stack;

#line 126 "grammar.tab.c"

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
  YYSYMBOL_END = 5,                        /* END  */
  YYSYMBOL_OTHER = 6,                      /* OTHER  */
  YYSYMBOL_COMMENT = 7,                    /* COMMENT  */
  YYSYMBOL_MSG = 8,                        /* MSG  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ENDIF = 10,                     /* ENDIF  */
  YYSYMBOL_GOTO = 11,                      /* GOTO  */
  YYSYMBOL_REPEAT = 12,                    /* REPEAT  */
  YYSYMBOL_ROT = 13,                       /* ROT  */
  YYSYMBOL_MISC_ID = 14,                   /* MISC_ID  */
  YYSYMBOL_VAR = 15,                       /* VAR  */
  YYSYMBOL_G_CMD = 16,                     /* G_CMD  */
  YYSYMBOL_XYZ_CMD = 17,                   /* XYZ_CMD  */
  YYSYMBOL_ABC_CMD = 18,                   /* ABC_CMD  */
  YYSYMBOL_CMD = 19,                       /* CMD  */
  YYSYMBOL_LABEL = 20,                     /* LABEL  */
  YYSYMBOL_SPECIAL_CMD = 21,               /* SPECIAL_CMD  */
  YYSYMBOL_CUSTOM_VAR = 22,                /* CUSTOM_VAR  */
  YYSYMBOL_STRING = 23,                    /* STRING  */
  YYSYMBOL_INT = 24,                       /* INT  */
  YYSYMBOL_FLOAT = 25,                     /* FLOAT  */
  YYSYMBOL_26_ = 26,                       /* '+'  */
  YYSYMBOL_27_ = 27,                       /* '-'  */
  YYSYMBOL_28_ = 28,                       /* '*'  */
  YYSYMBOL_29_ = 29,                       /* '/'  */
  YYSYMBOL_LOW_PREC = 30,                  /* LOW_PREC  */
  YYSYMBOL_31_ = 31,                       /* '='  */
  YYSYMBOL_32_ = 32,                       /* '('  */
  YYSYMBOL_33_ = 33,                       /* ')'  */
  YYSYMBOL_34_ = 34,                       /* ','  */
  YYSYMBOL_35_ = 35,                       /* '<'  */
  YYSYMBOL_36_ = 36,                       /* '>'  */
  YYSYMBOL_37_ = 37,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 38,                  /* $accept  */
  YYSYMBOL_prog = 39,                      /* prog  */
  YYSYMBOL_lines = 40,                     /* lines  */
  YYSYMBOL_line = 41,                      /* line  */
  YYSYMBOL_opt_skip = 42,                  /* opt_skip  */
  YYSYMBOL_opt_seps = 43,                  /* opt_seps  */
  YYSYMBOL_seps = 44,                      /* seps  */
  YYSYMBOL_exprs = 45,                     /* exprs  */
  YYSYMBOL_expr = 46,                      /* expr  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_if_body = 48,                   /* if_body  */
  YYSYMBOL_if_element = 49,                /* if_element  */
  YYSYMBOL_assignment = 50,                /* assignment  */
  YYSYMBOL_val = 51,                       /* val  */
  YYSYMBOL_fn = 52,                        /* fn  */
  YYSYMBOL_params = 53,                    /* params  */
  YYSYMBOL_arith_expr = 54,                /* arith_expr  */
  YYSYMBOL_bool_expr = 55                  /* bool_expr  */
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
#define YYLAST   260

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


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
       2,     2,     2,    37,     2,     2,     2,     2,     2,     2,
      32,    33,    28,    26,    34,    27,     2,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    31,    36,     2,     2,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    30
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   104,   104,   113,   114,   118,   119,   134,   138,   139,
     143,   144,   148,   149,   153,   154,   158,   158,   173,   191,
     201,   207,   208,   209,   210,   270,   273,   279,   283,   296,
     297,   298,   299,   303,   304,   308,   309,   310,   315,   324,
     325,   326,   327,   332,   336,   340,   341,   345,   349,   350,
     351,   352,   353,   358,   359,   360,   361,   362,   363,   364,
     365,   369,   370,   371,   372,   373,   374,   375,   376
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
  "END", "OTHER", "COMMENT", "MSG", "IF", "ENDIF", "GOTO", "REPEAT", "ROT",
  "MISC_ID", "VAR", "G_CMD", "XYZ_CMD", "ABC_CMD", "CMD", "LABEL",
  "SPECIAL_CMD", "CUSTOM_VAR", "STRING", "INT", "FLOAT", "'+'", "'-'",
  "'*'", "'/'", "LOW_PREC", "'='", "'('", "')'", "','", "'<'", "'>'",
  "'!'", "$accept", "prog", "lines", "line", "opt_skip", "opt_seps",
  "seps", "exprs", "expr", "$@1", "if_body", "if_element", "assignment",
  "val", "fn", "params", "arith_expr", "bool_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-53)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-11)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -53,    15,    89,   -53,   -53,   -53,   -53,    13,     8,    17,
     -53,   -53,   214,   -53,   -53,    27,    41,    43,    45,   -53,
      22,    46,   102,     7,     7,   102,   -53,   -53,    53,    46,
     -53,   -53,   -53,    34,    98,    47,    48,   186,    28,    22,
     -53,   -53,   -53,   -53,   102,   102,   -53,   -53,   -20,    29,
     -20,    36,   -20,   -20,   102,    40,   -53,   149,   -53,    98,
      98,   223,   -53,   -53,    69,    22,    -8,   -20,    46,    -1,
      14,   102,   102,   102,   102,    46,    46,   -20,    46,   -53,
     211,    42,   -53,    51,   158,   172,    55,   -53,    59,   -53,
     192,   102,   -53,    -1,    -1,   -53,   -53,   102,   102,   102,
     -53,   102,   102,   -20,   102,   -20,   102,   129,   -53,    22,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -53,
     -53,   -53,   -53,   -53
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     0,     1,     2,     7,    12,     0,     8,    11,
       4,     9,     0,    13,    32,     0,     0,     0,     0,    22,
      30,    10,     0,    10,    10,     0,    24,    28,    10,    10,
      14,    23,    31,     0,     0,     0,     0,    48,     0,     0,
      43,    44,    45,    46,     0,     0,    53,    54,    18,     0,
      19,     0,    20,    21,    12,     0,     6,    11,    29,     0,
       0,     0,    16,    25,    26,    50,     0,    49,    10,    60,
       0,     0,     0,     0,     0,    10,    10,    42,    10,    15,
       0,     0,    67,     0,     0,     0,     0,    33,     0,    47,
       0,     0,    59,    55,    56,    57,    58,     0,     0,     0,
      68,     0,     0,    61,     0,    62,     0,     0,    27,    52,
      51,    40,    38,    39,    41,    63,    65,    66,    64,    36,
      37,    17,    35,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -53,   -53,   -53,   -53,   -53,   -10,    50,   -53,   -52,   -53,
     -53,   -53,   -53,   -53,   -12,   -53,   -21,   -24
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,     7,    12,     8,     9,    29,    30,    87,
     107,   123,    31,    46,    47,    66,    61,    62
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      32,    48,    50,    52,    53,    79,    71,    72,    73,    74,
       6,    38,    -5,    49,    51,     3,    67,    10,    55,    56,
      13,    39,    40,    69,    70,    89,    90,    73,    74,    41,
      33,    42,    43,    77,    44,    81,    82,    11,    80,    45,
      71,    72,    73,    74,    34,    32,    35,    92,    36,     6,
      93,    94,    95,    96,    37,   122,    54,    58,    91,    68,
      75,    63,    64,   103,   105,    97,    98,    76,    99,   110,
     111,    78,    88,   108,     0,   100,   112,   113,   114,    57,
     115,   116,   101,   117,     0,   118,   106,     0,     0,     4,
       5,     0,     6,   -10,     0,    32,   -10,   -10,   -10,     0,
     -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
     -10,   -10,    39,    40,     0,     0,    39,    40,   -10,     0,
      41,     0,    42,    43,    41,    44,    42,    43,     0,    44,
      59,     0,   119,   120,    45,    60,    14,    15,    16,   121,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    13,     0,     0,     0,    14,    15,    16,     0,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    39,    40,     0,     0,     0,     0,     0,     0,
      41,     0,    42,    43,     0,    44,    39,    40,     0,   102,
      45,     0,     0,     0,    41,     0,    42,    43,     0,    44,
      65,    40,     0,   104,    45,     0,   109,    40,    41,     0,
      42,    43,     0,    44,    41,     0,    42,    43,    45,    44,
       0,    14,    15,    16,    45,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    71,    72,    73,
      74,     0,    83,     0,    92,     0,    84,    85,    86,    71,
      72,    73,    74,     0,    83,     0,     0,     0,    84,    85,
      86
};

static const yytype_int8 yycheck[] =
{
      12,    22,    23,    24,    25,    57,    26,    27,    28,    29,
       3,    21,     4,    23,    24,     0,    37,     4,    28,    29,
       3,    14,    15,    44,    45,    33,    34,    28,    29,    22,
       3,    24,    25,    54,    27,    59,    60,    29,    59,    32,
      26,    27,    28,    29,     3,    57,     3,    33,     3,     3,
      71,    72,    73,    74,    32,   107,     3,    23,    68,    31,
      31,    14,    14,    84,    85,    75,    76,    31,    78,    90,
      91,    31,     3,    14,    -1,    33,    97,    98,    99,    29,
     101,   102,    31,   104,    -1,   106,    31,    -1,    -1,     0,
       1,    -1,     3,     4,    -1,   107,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    14,    15,    -1,    -1,    14,    15,    29,    -1,
      22,    -1,    24,    25,    22,    27,    24,    25,    -1,    27,
      32,    -1,     3,     4,    32,    37,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     3,    -1,    -1,    -1,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    24,    25,    -1,    27,    14,    15,    -1,    31,
      32,    -1,    -1,    -1,    22,    -1,    24,    25,    -1,    27,
      14,    15,    -1,    31,    32,    -1,    14,    15,    22,    -1,
      24,    25,    -1,    27,    22,    -1,    24,    25,    32,    27,
      -1,     7,     8,     9,    32,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    26,    27,    28,
      29,    -1,    31,    -1,    33,    -1,    35,    36,    37,    26,
      27,    28,    29,    -1,    31,    -1,    -1,    -1,    35,    36,
      37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    39,    40,     0,     0,     1,     3,    41,    43,    44,
       4,    29,    42,     3,     7,     8,     9,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    45,
      46,    50,    52,     3,     3,     3,     3,    32,    43,    14,
      15,    22,    24,    25,    27,    32,    51,    52,    54,    43,
      54,    43,    54,    54,     3,    43,    43,    44,    23,    32,
      37,    54,    55,    14,    14,    14,    53,    54,    31,    54,
      54,    26,    27,    28,    29,    31,    31,    54,    31,    46,
      54,    55,    55,    31,    35,    36,    37,    47,     3,    33,
      34,    43,    33,    54,    54,    54,    54,    43,    43,    43,
      33,    31,    31,    54,    31,    54,    31,    48,    14,    14,
      54,    54,    54,    54,    54,    54,    54,    54,    54,     3,
       4,    10,    46,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    41,    41,    41,    42,    42,
      43,    43,    44,    44,    45,    45,    47,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    48,    48,    49,    49,    49,    50,    50,
      50,    50,    50,    51,    51,    51,    51,    52,    53,    53,
      53,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    55,    55,    55,    55,    55,    55,    55
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     3,     1,     4,     1,     0,     1,
       0,     1,     1,     2,     1,     3,     0,     6,     2,     2,
       2,     2,     1,     1,     1,     3,     3,     5,     1,     3,
       1,     1,     1,     0,     2,     1,     1,     1,     5,     5,
       5,     5,     3,     1,     1,     1,     1,     4,     0,     1,
       1,     3,     3,     1,     1,     3,     3,     3,     3,     3,
       2,     3,     3,     4,     4,     4,     4,     2,     3
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


/* User initialization code.  */
#line 95 "grammar.y"
{
    init_stack(&ret_stack);
    //h = init_hashmap(); //h is global in helper.c for now
}

#line 1078 "grammar.tab.c"

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
  case 2: /* prog: lines $end  */
#line 104 "grammar.y"
              {
    printf("%lu tracks written to %s!\n",tid,config.track_list_csv);
    if (get_var_val("line") <= 1) {
      printf("Warning: File %s is empty!\n",config.mpf_file);
    }
  }
#line 1286 "grammar.tab.c"
    break;

  case 6: /* line: opt_seps opt_skip exprs opt_seps  */
#line 120 "grammar.y"
    {
     rot_mode = 0; //reset linewise
     track_written--; //counter for how many lines not to print_track

     if(config.hmhis_to_file) {print_hashmap(h, hmhis);}
     if(config.hmhis_to_stdout) {print_hashmap(h, stdout);}

     if (jump_requested) {
          jump_requested = 0;
          skip = 0;
          jump(target_line,target_byte_offset);
          printf("Jumped to line=%lu, byte=%ld\n\n",target_line, target_byte_offset);
     }
    }
#line 1305 "grammar.tab.c"
    break;

  case 16: /* $@1: %empty  */
#line 158 "grammar.y"
                   {
      if (!(yyvsp[0].bool_expr)) {
          skip++; //ignore code lines, if condition is false
          if(debug) {printf("Skip=%d\n",skip);}
      }
  }
#line 1316 "grammar.tab.c"
    break;

  case 17: /* expr: IF SEP bool_expr $@1 if_body ENDIF  */
#line 163 "grammar.y"
                  {
      if (skip > 0) {
        skip--; //decr skip, to resume to normal code line execution
        //we are using skip as a counter, to allow nested false IF blocks
        //ToDo: Are nested false IF blocks possible?
        //Can't we just ignore the inner IF blocks, if the outer is false
        //-> I guess?
        if(debug) {printf("Skip=%d\n",skip);}
      }
    }
#line 1331 "grammar.tab.c"
    break;

  case 18: /* expr: G_CMD arith_expr  */
#line 173 "grammar.y"
                     {if(!skip){
      //handle G codes
      switch((int)(yyvsp[0].arith_expr)) {
        //Nullpunktverschiebung (NPV) =^ origin_offset
        case 54: origin_offset=(vec3D){0,0,139.4};break; //
        case 55: origin_offset=(vec3D){0,0,124.4};break; //
        case 56: origin_offset=(vec3D){0,10,0};break;
        case 57: origin_offset=(vec3D){0,0,10};break;
        case 58: origin_offset=(vec3D){10,15,0};break;
        case 59: origin_offset=(vec3D){0,10,20};break;
        //(Un-)Set incr_mode ->
        //assingments of coords happens absolute or relative
        case 90: incr_mode=0;break; //absolute
        case 91: incr_mode=1;break; //relative
        default: break;
      }
    }
  }
#line 1354 "grammar.tab.c"
    break;

  case 19: /* expr: XYZ_CMD arith_expr  */
#line 191 "grammar.y"
                       {
    if(!skip) {
      if(rot_mode) {
        set_var_rot((yyvsp[-1].XYZ_CMD), (yyvsp[0].arith_expr));
      } else {
        set_var((yyvsp[-1].XYZ_CMD),(yyvsp[0].arith_expr));
        handle_tracks_def_by_coord_lines();
      }
    }
  }
#line 1369 "grammar.tab.c"
    break;

  case 20: /* expr: ABC_CMD arith_expr  */
#line 201 "grammar.y"
                       {
    if(!skip) {
      set_var((yyvsp[-1].ABC_CMD),(yyvsp[0].arith_expr));
      handle_tracks_def_by_coord_lines();
    }
  }
#line 1380 "grammar.tab.c"
    break;

  case 21: /* expr: CMD arith_expr  */
#line 207 "grammar.y"
                         {}
#line 1386 "grammar.tab.c"
    break;

  case 22: /* expr: ROT  */
#line 208 "grammar.y"
                       {rot_mode = 1;}
#line 1392 "grammar.tab.c"
    break;

  case 24: /* expr: LABEL  */
#line 210 "grammar.y"
                       {
                        if(!skip && !is_empty(&ret_stack)){

                          Elem top;
                          //Get stack top, without removing it from the stack
                          peek(&ret_stack, &top);
                          printf("$1_LABEL=%s, ret_stack_top.label=%s\n",(yyvsp[0].LABEL),top.label);

                          //if the currently read LABEL="END_LABEL"
                          printf("strcmp($1,'END_LABEL')=%d\n",strcmp((yyvsp[0].LABEL),"END_LABEL"));
                          if((strcmp((yyvsp[0].LABEL),"END_LABEL") == 0)) {
                            //d->distance btw
                            //current runtime line and
                            //noted line of "<start_label>_END_LABEL" construct
                            int d = get_var_val("line")-get_var_val(top.label);
                            printf("d=%d\n...\n",d);

                            //lf -> label_finder
                            //tolerate off by one error, btw runtime- and lf-line
                            if(abs(d)<=1) {
                               //if the lines of "<start_label>_END_LABEL" construct,
                               //and the runtime line match, then we found the END_LABEL,
                               //to the currently active REPEAT block
                               printf("'END_LABEL' found! top.label=%s\n",top.label);

                               jump_requested = 1;
                               skip = 1;

                               target_line = top.line;
                               target_byte_offset = top.byte_offset;

                               printf("Jump Requested!\
                               \r\ntarget_line=%lu\ntarget_byte_offset=%ld\n",
                               target_line, target_byte_offset);

                               pop(&ret_stack, &top);
                            }
                          }
                          else if (strcmp((yyvsp[0].LABEL),top.label) == 0) {
                            jump_requested = 1;
                            skip = 1;

                            target_line = top.line;
                            target_byte_offset = top.byte_offset;

                            printf("Jump Requested!\
                            \r\ntarget_line=%lu\ntarget_byte_offset=%ld\n",
                            target_line, target_byte_offset);

                            pop(&ret_stack, &top);
                          }
                          //else:
                          //goto not_found;

                          //end_label_found:


                          //not_found:
                        }
                       }
#line 1457 "grammar.tab.c"
    break;

  case 25: /* expr: GOTO SEP MISC_ID  */
#line 270 "grammar.y"
                       {if(!skip){
                          request_jump((yyvsp[0].MISC_ID));}
                        }
#line 1465 "grammar.tab.c"
    break;

  case 26: /* expr: REPEAT SEP MISC_ID  */
#line 274 "grammar.y"
                        {if(!skip){
                            char start_end_l[32+11];
                            snprintf(start_end_l, sizeof(start_end_l), "%s_END_LABEL", (yyvsp[0].MISC_ID));
                            handle_repeat((yyvsp[0].MISC_ID),start_end_l,(size_t)get_var_val("line"));}
                        }
#line 1475 "grammar.tab.c"
    break;

  case 27: /* expr: REPEAT SEP MISC_ID SEP MISC_ID  */
#line 280 "grammar.y"
                        {if(!skip){
                            handle_repeat((yyvsp[-2].MISC_ID),(yyvsp[0].MISC_ID),(size_t)get_var_val("line"));}
                        }
#line 1483 "grammar.tab.c"
    break;

  case 28: /* expr: SPECIAL_CMD  */
#line 283 "grammar.y"
                         {
                          if(strcmp((yyvsp[0].SPECIAL_CMD),"LASER_ON") == 0) {
                            set_var("laser",1);
                            if (config.tracks_def_by_laser){t_start=net_point();}

                          }
                          else if(strcmp((yyvsp[0].SPECIAL_CMD),"LASER_OFF") == 0) {
                            set_var("laser",0);
                            if (config.tracks_def_by_laser){t_end=net_point();}

                            write_track_line();
                          }
                         }
#line 1501 "grammar.tab.c"
    break;

  case 38: /* assignment: XYZ_CMD opt_seps '=' opt_seps arith_expr  */
#line 316 "grammar.y"
  {if(!skip){
    if (rot_mode) {
      set_var_rot((yyvsp[-4].XYZ_CMD),(yyvsp[0].arith_expr));
    }
    else {
     set_var_incr((yyvsp[-4].XYZ_CMD),(yyvsp[0].arith_expr));
  }}}
#line 1513 "grammar.tab.c"
    break;

  case 39: /* assignment: ABC_CMD opt_seps '=' opt_seps arith_expr  */
#line 324 "grammar.y"
                                                {if(!skip){set_var_incr((yyvsp[-4].ABC_CMD),(yyvsp[0].arith_expr));}}
#line 1519 "grammar.tab.c"
    break;

  case 40: /* assignment: VAR opt_seps '=' opt_seps arith_expr  */
#line 325 "grammar.y"
                                                {if(!skip){set_var((yyvsp[-4].VAR),(yyvsp[0].arith_expr));}}
#line 1525 "grammar.tab.c"
    break;

  case 41: /* assignment: CUSTOM_VAR opt_seps '=' opt_seps arith_expr  */
#line 326 "grammar.y"
                                                {if(!skip){set_var((yyvsp[-4].CUSTOM_VAR),(yyvsp[0].arith_expr));}}
#line 1531 "grammar.tab.c"
    break;

  case 42: /* assignment: CUSTOM_VAR SEP arith_expr  */
#line 327 "grammar.y"
                                                {if(!skip){set_var((yyvsp[-2].CUSTOM_VAR),(yyvsp[0].arith_expr));}}
#line 1537 "grammar.tab.c"
    break;

  case 43: /* val: VAR  */
#line 332 "grammar.y"
                 {
                  /*printf("Getting VAR=%s\n",$1);*/
                  (yyval.val) = get_var_val((yyvsp[0].VAR));
                 }
#line 1546 "grammar.tab.c"
    break;

  case 44: /* val: CUSTOM_VAR  */
#line 336 "grammar.y"
                 {
                  /*printf("Getting CUSTOM_VAR=%s\n",$1);*/
                  (yyval.val) = get_var_val((yyvsp[0].CUSTOM_VAR));
                 }
#line 1555 "grammar.tab.c"
    break;

  case 45: /* val: INT  */
#line 340 "grammar.y"
                 {(yyval.val)=(yyvsp[0].INT);}
#line 1561 "grammar.tab.c"
    break;

  case 46: /* val: FLOAT  */
#line 341 "grammar.y"
                 {(yyval.val)=(yyvsp[0].FLOAT);}
#line 1567 "grammar.tab.c"
    break;

  case 47: /* fn: MISC_ID '(' params ')'  */
#line 345 "grammar.y"
                         {(yyval.fn)=0;}
#line 1573 "grammar.tab.c"
    break;

  case 53: /* arith_expr: val  */
#line 358 "grammar.y"
              {(yyval.arith_expr)=(yyvsp[0].val);}
#line 1579 "grammar.tab.c"
    break;

  case 54: /* arith_expr: fn  */
#line 359 "grammar.y"
              {(yyval.arith_expr)=(yyvsp[0].fn);}
#line 1585 "grammar.tab.c"
    break;

  case 55: /* arith_expr: arith_expr '+' arith_expr  */
#line 360 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)+(yyvsp[0].arith_expr); /*printf("%f+%f=%f\n", $1,$3,$$);*/}
#line 1591 "grammar.tab.c"
    break;

  case 56: /* arith_expr: arith_expr '-' arith_expr  */
#line 361 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)-(yyvsp[0].arith_expr);}
#line 1597 "grammar.tab.c"
    break;

  case 57: /* arith_expr: arith_expr '*' arith_expr  */
#line 362 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)*(yyvsp[0].arith_expr);}
#line 1603 "grammar.tab.c"
    break;

  case 58: /* arith_expr: arith_expr '/' arith_expr  */
#line 363 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-2].arith_expr)/(yyvsp[0].arith_expr);}
#line 1609 "grammar.tab.c"
    break;

  case 59: /* arith_expr: '(' arith_expr ')'  */
#line 364 "grammar.y"
                              {(yyval.arith_expr)=(yyvsp[-1].arith_expr);}
#line 1615 "grammar.tab.c"
    break;

  case 60: /* arith_expr: '-' arith_expr  */
#line 365 "grammar.y"
                              {(yyval.arith_expr)=-(yyvsp[0].arith_expr);}
#line 1621 "grammar.tab.c"
    break;

  case 61: /* bool_expr: arith_expr '<' arith_expr  */
#line 369 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-2].arith_expr)<(yyvsp[0].arith_expr);}
#line 1627 "grammar.tab.c"
    break;

  case 62: /* bool_expr: arith_expr '>' arith_expr  */
#line 370 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-2].arith_expr)>(yyvsp[0].arith_expr);}
#line 1633 "grammar.tab.c"
    break;

  case 63: /* bool_expr: arith_expr '=' '=' arith_expr  */
#line 371 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)==(yyvsp[0].arith_expr);}
#line 1639 "grammar.tab.c"
    break;

  case 64: /* bool_expr: arith_expr '!' '=' arith_expr  */
#line 372 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)!=(yyvsp[0].arith_expr);}
#line 1645 "grammar.tab.c"
    break;

  case 65: /* bool_expr: arith_expr '<' '=' arith_expr  */
#line 373 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)<=(yyvsp[0].arith_expr);}
#line 1651 "grammar.tab.c"
    break;

  case 66: /* bool_expr: arith_expr '>' '=' arith_expr  */
#line 374 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-3].arith_expr)>=(yyvsp[0].arith_expr);}
#line 1657 "grammar.tab.c"
    break;

  case 67: /* bool_expr: '!' bool_expr  */
#line 375 "grammar.y"
                                  {(yyval.bool_expr)=!(yyvsp[0].bool_expr);}
#line 1663 "grammar.tab.c"
    break;

  case 68: /* bool_expr: '(' bool_expr ')'  */
#line 376 "grammar.y"
                                  {(yyval.bool_expr)=(yyvsp[-1].bool_expr);}
#line 1669 "grammar.tab.c"
    break;


#line 1673 "grammar.tab.c"

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

#line 380 "grammar.y"

void request_jump(char* target) {
  jump_requested = 1;
  skip = 1;

  char line_label[32+6];
  snprintf(line_label, sizeof(line_label), "%s_line", target);
  target_line = (size_t)get_var_val(line_label);
  target_byte_offset = get_var_val(target);
}

void jump(size_t target_line,long target_byte_offset) {
  if (target_byte_offset >= 0) {
      set_var("line", target_line);
      skip = 0;
      jump_requested = 0;

      if(debug){
        printf("Jumping to line=%lu, offset=%ld\n\n",
        target_line,target_byte_offset);}

      fseek(yyin, target_byte_offset, SEEK_SET);
      yyrestart(yyin);
  }
}

void handle_repeat(char* start_label, char* end_label, size_t linep) {
  char labelp[32]; //search towards +inf dir
  char labeln[32]; //search towards -inf dir

  size_t linen = linep;
  size_t line=-1;
  size_t byte_offset=-1;
  //ball search
  while (0<linen) {
    snprintf(labelp, sizeof(labelp), "REPEAT_%lu", linep);
    snprintf(labeln, sizeof(labeln), "REPEAT_%lu", linen);

    long byte_offsetp = (long)get_var_val(labelp);
    long byte_offsetn = (long)get_var_val(labeln);

    printf("labelp=%s, labeln=%s, linep=%lu, linen=%lu, line var_val=%lu\n",
      labelp,labeln,linep,linen,(size_t)get_var_val("line"));
    if (byte_offsetp <= 0 && byte_offsetn <= 0) {
      if (debug) {
        printf("Error: REPEAT return byte_offset=%ld\n",byte_offset);
      }
    }
    else {
      if (byte_offsetp > 0) {
        byte_offset=byte_offsetp;
        line=linep;
     }
     else if (byte_offsetn > 0) {
       byte_offset=byte_offsetn;
       line=linen;
     }
     break; //REPEAT found, we are finished
    }
    linen--;linep++;
  }
  printf("REPEAT found in line=%lu, byte_offset=%ld\n",line,byte_offset);

  push(&ret_stack,
    strdup(end_label), //return label
    line,       //return line
    byte_offset //return byte_offset
  );

  request_jump(start_label);

  if (debug > 0) {
    Elem temp;
    if (peek(&ret_stack, &temp)) {
        printf("Return: Label %s, Line: %zu, Offset: %ld)\n\n",
                temp.label, temp.line, temp.byte_offset);
    }
  }
}

void handle_tracks_def_by_coord_lines() {
  //handle (X,Y,Z) points rotated
  //in the experimental tracks_def_by_coord_lines mode
  if (!rot_mode && !config.tracks_def_by_laser && !(track_written>0))
  {
    t_end = net_point();
    write_track_line();
    t_start = net_point(); //TODO perhaps cleaner sol, than net_point weird global var inline function
    //prevent mult writes in line with mult coord cmds
    track_written = config.track_mid_len;
  }
}

void write_track_line() {
  fprintf(tl,"%lu, %f, %f, %f, %f, %f, %f, %f, %f, 0, 0, 0, %f, %f\n",
  tid++, t_start.x, t_start.y, t_start.z, t_end.x, t_end.y, t_end.z,
  get_var_val("PUIS_LASER"), get_var_val("VIT_TIR"),
  //coll_vec,
  config.hrad, config.vrad);
}


void set_var_incr(char* varname, float fnum) {
    //incr_mode only applies to (X,Y,Z) & (A,B,C), which we call coords
    if (incr_mode) {
      set_var(varname, get_var_val(varname)+fnum);
    }
    else {
      set_var(varname,fnum);
    }
}

int yyerror(char* s) {
	printf("Error: %s, in line: %d\n", s, (int)get_var_val("line"));
	return 0;
}
