/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EXPRDELIM = 257,
     ID = 258,
     NUMCONST = 259,
     EQUAL = 260,
     LEQUAL = 261,
     MEQUAL = 262,
     NOTEQUAL = 263,
     LETKW = 400,
     INKW = 401,
     INSPECTKW = 402,
     QUITKW = 403,
     LOADKW = 404,
     LETRECKW = 405,
     TRUEKW = 406,
     FALSEKW = 407,
     IFKW = 408,
     THENKW = 409,
     ELSEKW = 410,
     ANDKW = 411,
     ORKW = 412,
     NOTKW = 413,
     DIVKW = 414,
     MODKW = 415,
     CONSKW = 416,
     HEADKW = 417,
     TAILKW = 418,
     TESTNILKW = 419,
     DEFKW = 420,
     NILKW = 422,
     GARBAGEKW = 423,
     OPTIONKW = 424,
     INFOKW = 425,
     SAVEKW = 426,
     TRAVELKW = 427,
     ASTRING = 428,
     NEG = 430
   };
#endif
/* Tokens.  */
#define EXPRDELIM 257
#define ID 258
#define NUMCONST 259
#define EQUAL 260
#define LEQUAL 261
#define MEQUAL 262
#define NOTEQUAL 263
#define LETKW 400
#define INKW 401
#define INSPECTKW 402
#define QUITKW 403
#define LOADKW 404
#define LETRECKW 405
#define TRUEKW 406
#define FALSEKW 407
#define IFKW 408
#define THENKW 409
#define ELSEKW 410
#define ANDKW 411
#define ORKW 412
#define NOTKW 413
#define DIVKW 414
#define MODKW 415
#define CONSKW 416
#define HEADKW 417
#define TAILKW 418
#define TESTNILKW 419
#define DEFKW 420
#define NILKW 422
#define GARBAGEKW 423
#define OPTIONKW 424
#define INFOKW 425
#define SAVEKW 426
#define TRAVELKW 427
#define ASTRING 428
#define NEG 430




/* Copy the first part of user declarations.  */
#line 98 "parser.y"

#include "bohm.h"

#line 108 "parser.y"

#ifdef __cplusplus
extern "C" {
#endif

int yylex();
int yyerror(const char *msg);

#ifdef __cplusplus
}
#endif
#line 125 "parser.y"

bool			quit,
			       /* flag indicating quit request */
			loading_mode;
			       /* flag indicating if parsing is */
			       /* done after a load directive */
char                    *include_file;
FORM                    *lastinputterm,
			       /* pointer to the root of the */
			       /* term in input */
                        *current_pos;
 			       /* for inspection mode */
#line 143 "parser.y"

#define UNBOUND_VARIABLE "scoping error: undefined variable"

int                    app_nesting_depth;
PATTERN                *pattmp;

static bool defined(STBUCKET	*st);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 156 "parser.y"
{
		/* pointer to a symbol */
		/* table bucket */
		/* numerical constant */
		STBUCKET	*st_bucket;
		
		int		num_const;

		/* graph representation */
		/* of the term */
		TERM		*term;
	
		/* root_form  */
		FORM		*root;

		char            *astring;
		PATTERN         *pattern;
	}
/* Line 193 of yacc.c.  */
#line 229 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 242 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   583

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNRULES -- Number of states.  */
#define YYNSTATES  161

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   430

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    46,     2,     2,     2,     2,
      47,    48,    44,    42,    50,    43,    38,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    39,    41,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,    49,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    53,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,     2,     2,     2,     2,     2,     2,
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
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,     2,    31,    32,    33,    34,    35,    36,    37,     2,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    10,    15,    21,    25,
      30,    34,    39,    44,    49,    55,    56,    58,    59,    61,
      62,    64,    67,    70,    71,    77,    79,    82,    86,    90,
      94,    98,   102,   106,   110,   114,   118,   122,   126,   130,
     134,   136,   138,   140,   142,   146,   147,   148,   155,   156,
     157,   166,   167,   173,   180,   182,   185,   192,   197,   202,
     207,   209,   212,   215,   218,   222,   227,   229,   233,   234,
     238,   240,   244,   246,   250,   257,   259,   263,   265
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    57,    -1,    -1,    56,    61,    -1,    -1,
      46,    12,    58,     3,    -1,    46,    36,    58,     5,     3,
      -1,    46,    13,     3,    -1,    46,    14,    37,     3,    -1,
      46,    32,     3,    -1,    46,    33,    60,     3,    -1,    46,
      34,    59,     3,    -1,    46,    35,    37,     3,    -1,    46,
      35,    37,     4,     3,    -1,    -1,     4,    -1,    -1,    37,
      -1,    -1,     5,    -1,    64,     3,    -1,    62,     3,    -1,
      -1,    30,     4,    39,    63,    64,    -1,    65,    -1,    43,
      64,    -1,    64,    21,    64,    -1,    64,    22,    64,    -1,
      64,    40,    64,    -1,    64,     6,    64,    -1,    64,     9,
      64,    -1,    64,    41,    64,    -1,    64,     7,    64,    -1,
      64,     8,    64,    -1,    64,    42,    64,    -1,    64,    43,
      64,    -1,    64,    44,    64,    -1,    64,    24,    64,    -1,
      64,    25,    64,    -1,    16,    -1,    17,    -1,     5,    -1,
       4,    -1,    47,    73,    48,    -1,    -1,    -1,    49,    66,
      77,    67,    38,    64,    -1,    -1,    -1,    10,     4,    39,
      68,    64,    11,    69,    64,    -1,    -1,    15,     4,    39,
      70,    64,    -1,    18,    64,    19,    64,    20,    64,    -1,
      71,    -1,    23,    64,    -1,    26,    47,    64,    50,    64,
      48,    -1,    27,    47,    64,    48,    -1,    28,    47,    64,
      48,    -1,    29,    47,    64,    48,    -1,    31,    -1,    51,
      52,    -1,    51,    72,    -1,    64,    52,    -1,    64,    50,
      72,    -1,    64,    53,    64,    52,    -1,    64,    -1,    64,
      50,    75,    -1,    -1,    73,    74,    65,    -1,    64,    -1,
      64,    50,    75,    -1,    77,    -1,    77,    50,    76,    -1,
      26,    47,    77,    50,    77,    48,    -1,    31,    -1,    47,
      76,    48,    -1,     4,    -1,     1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   264,   264,   265,   265,   275,   282,   289,   295,   300,
     307,   312,   317,   322,   328,   343,   346,   359,   362,   368,
     371,   376,   384,   394,   393,   406,   410,   414,   419,   424,
     429,   434,   439,   444,   449,   454,   459,   464,   469,   474,
     481,   485,   489,   493,   503,   508,   513,   507,   525,   529,
     524,   541,   540,   552,   557,   562,   567,   572,   577,   582,
     589,   593,   597,   602,   607,   612,   619,   620,   626,   625,
     636,   640,   647,   651,   664,   676,   683,   687,   698
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EXPRDELIM", "ID", "NUMCONST", "EQUAL",
  "LEQUAL", "MEQUAL", "NOTEQUAL", "LETKW", "INKW", "INSPECTKW", "QUITKW",
  "LOADKW", "LETRECKW", "TRUEKW", "FALSEKW", "IFKW", "THENKW", "ELSEKW",
  "ANDKW", "ORKW", "NOTKW", "DIVKW", "MODKW", "CONSKW", "HEADKW", "TAILKW",
  "TESTNILKW", "DEFKW", "NILKW", "GARBAGEKW", "OPTIONKW", "INFOKW",
  "SAVEKW", "TRAVELKW", "ASTRING", "'.'", "'='", "'<'", "'>'", "'+'",
  "'-'", "'*'", "NEG", "'#'", "'('", "')'", "'\\\\'", "','", "'['", "']'",
  "'|'", "$accept", "input", "@1", "directive", "arg", "optstring",
  "optint", "term", "global_decl", "@2", "expr", "expr0", "@3", "@4", "@5",
  "@6", "@7", "list", "exprlist", "applist", "@8", "comlist", "comlistpat",
  "pattern", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   429,   257,   258,   259,   260,   261,   262,   263,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   422,   423,   424,   425,   426,   427,   428,    46,    61,
      60,    62,    43,    45,    42,   430,    35,    40,    41,    92,
      44,    91,    93,   124
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    56,    55,    55,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    58,    58,    59,    59,    60,
      60,    61,    61,    63,    62,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      65,    65,    65,    65,    65,    66,    67,    65,    68,    69,
      65,    70,    65,    65,    65,    65,    65,    65,    65,    65,
      71,    71,    71,    72,    72,    72,    73,    73,    74,    73,
      75,    75,    76,    76,    77,    77,    77,    77,    61
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     0,     4,     5,     3,     4,
       3,     4,     4,     4,     5,     0,     1,     0,     1,     0,
       1,     2,     2,     0,     5,     1,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     3,     0,     0,     6,     0,     0,
       8,     0,     5,     6,     1,     2,     6,     4,     4,     4,
       1,     2,     2,     2,     3,     4,     1,     3,     0,     3,
       1,     3,     1,     3,     6,     1,     3,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     2,    15,     0,     0,     0,    19,
      17,     0,    15,     1,     0,    43,    42,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,    60,     0,
       0,    45,     0,     4,     0,     0,    25,    54,    16,     0,
       8,     0,    10,    20,     0,    18,     0,     0,     0,    78,
       0,     0,     0,    55,     0,     0,     0,     0,     0,    26,
      66,    68,     0,    61,     0,    62,    22,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     9,    11,    12,    13,     0,     0,    48,    51,
       0,     0,     0,     0,     0,    23,     0,    44,     0,    77,
       0,    75,     0,    46,     0,    63,     0,    30,    33,    34,
      31,    27,    28,    38,    39,    29,    32,    35,    36,    37,
      14,     7,     0,     0,     0,     0,    57,    58,    59,     0,
      70,    67,    69,     0,     0,    72,     0,    64,     0,     0,
      52,     0,     0,    24,     0,     0,    76,     0,     0,    65,
      49,    53,    56,    71,     0,    73,    47,     0,     0,    50,
      74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    39,    46,    44,    33,    34,   129,
      64,    36,    62,   136,   122,   157,   123,    37,    65,    61,
      98,   131,   134,   135
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const yytype_int16 yypact[] =
{
       3,    -4,     6,   132,   -56,     7,    12,   -21,    14,    15,
     -12,    -2,     7,   -56,    30,   -56,   -56,    32,    33,   -56,
     -56,   212,   212,    -9,    -7,    -6,    -5,    40,   -56,   212,
     212,   -56,   172,   -56,    47,   437,   -56,   -56,   -56,    53,
     -56,    54,   -56,   -56,    55,   -56,    56,    10,    38,   -56,
      23,    45,   463,   539,   212,   212,   212,   212,    46,   -56,
     265,    42,     8,   -56,    39,   -56,   -56,   -56,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   -56,   -56,   -56,   -56,   -56,    83,    85,   -56,   -56,
     212,   289,   337,   362,   387,   -56,   212,   -56,   252,   -56,
      51,   -56,     8,   -56,   212,   -56,   212,   -20,   -20,   -20,
     -20,   539,   539,   -56,   -56,   -20,   -20,   -23,   -23,   -56,
     -56,   -56,   212,   212,   488,   212,   -56,   -56,   -56,   212,
     313,   -56,   -56,     8,    52,    57,    63,   -56,    88,   512,
     518,   212,   412,   518,   212,    58,   -56,     8,   212,   -56,
     -56,   518,   -56,   -56,     8,   -56,   518,   212,    66,   518,
     -56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -56,   -56,   -56,   -56,    90,   -56,   -56,   -56,   -56,   -56,
      -3,    13,   -56,   -56,   -56,   -56,   -56,   -56,     0,   -56,
     -56,   -39,   -41,   -55
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -6
static const yytype_int16 yytable[] =
{
      35,    74,    75,    -5,    74,    75,    13,   103,     5,     6,
       7,    38,    99,    85,    86,    40,    41,    42,    52,    53,
      43,    80,    78,    79,    80,    45,    59,    60,     8,     9,
      10,    11,    12,    49,   100,    47,    50,    51,    54,   101,
      55,    56,    57,    87,    58,    68,    69,    70,    71,     1,
      66,    91,    92,    93,    94,   102,    81,    82,    83,    84,
      72,    73,    88,    74,    75,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   145,    76,
      77,    78,    79,    80,    89,    95,   120,   124,   121,   104,
      97,   105,   106,   130,    68,    69,    70,    71,   133,   158,
     146,   148,    48,   138,   137,   153,   155,   147,   154,    72,
      73,   132,    74,    75,   160,     0,     0,     0,     0,   139,
     140,     0,   142,     0,     0,     0,   143,     0,    76,    77,
      78,    79,    80,    14,     0,     0,    15,    16,   151,     0,
     149,   130,    17,     0,     0,   156,     0,    18,    19,    20,
      21,     0,     0,     0,   159,    22,     0,     0,    23,    24,
      25,    26,    27,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    15,    16,     0,    30,
       0,    31,    17,    32,     0,     0,     0,    18,    19,    20,
      21,     0,     0,     0,     0,    22,     0,     0,    23,    24,
      25,    26,     0,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    15,    16,     0,    30,
       0,    31,    17,    32,    63,     0,     0,    18,    19,    20,
      21,     0,     0,     0,     0,    22,     0,     0,    23,    24,
      25,    26,     0,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    15,    16,     0,    30,
       0,    31,    17,    32,     0,     0,     0,    18,    19,    20,
      21,    68,    69,    70,    71,    22,     0,     0,    23,    24,
      25,    26,     0,    28,     0,     0,    72,    73,     0,    74,
      75,     0,     0,     0,     0,    68,    69,    70,    71,    30,
       0,    31,     0,    32,     0,    76,    77,    78,    79,    80,
      72,    73,     0,    74,    75,    96,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,     0,     0,    76,
      77,    78,    79,    80,    72,    73,     0,    74,    75,   125,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,     0,     0,    76,    77,    78,    79,    80,    72,    73,
       0,    74,    75,   144,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,     0,    76,    77,    78,
      79,    80,     0,    72,    73,   126,    74,    75,     0,     0,
       0,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,     0,    76,    77,    78,    79,    80,     0,    72,    73,
     127,    74,    75,     0,     0,     0,     0,     0,    68,    69,
      70,    71,     0,     0,     0,     0,     0,    76,    77,    78,
      79,    80,     0,    72,    73,   128,    74,    75,     0,     0,
      67,     0,     0,    68,    69,    70,    71,     0,     0,     0,
       0,     0,    76,    77,    78,    79,    80,     0,    72,    73,
     152,    74,    75,     0,     0,     0,     0,     0,     0,    68,
      69,    70,    71,     0,     0,     0,     0,    76,    77,    78,
      79,    80,    90,     0,    72,    73,     0,    74,    75,     0,
       0,     0,     0,     0,    68,    69,    70,    71,     0,     0,
       0,     0,     0,    76,    77,    78,    79,    80,   141,    72,
      73,     0,    74,    75,     0,     0,     0,     0,    68,    69,
      70,    71,     0,   150,    68,    69,    70,    71,    76,    77,
      78,    79,    80,    72,    73,     0,    74,    75,     0,    72,
      73,     0,    74,    75,     0,    68,    69,    70,    71,     0,
       0,     0,    76,    77,    78,    79,    80,     0,    76,    77,
      78,    79,    80,    74,    75,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
      77,    78,    79,    80
};

static const yytype_int16 yycheck[] =
{
       3,    24,    25,     0,    24,    25,     0,    62,    12,    13,
      14,     4,     4,     3,     4,     3,    37,     3,    21,    22,
       5,    44,    42,    43,    44,    37,    29,    30,    32,    33,
      34,    35,    36,     3,    26,    37,     4,     4,    47,    31,
      47,    47,    47,     5,     4,     6,     7,     8,     9,    46,
       3,    54,    55,    56,    57,    47,     3,     3,     3,     3,
      21,    22,    39,    24,    25,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,   133,    40,
      41,    42,    43,    44,    39,    39,     3,    90,     3,    50,
      48,    52,    53,    96,     6,     7,     8,     9,    47,   154,
      48,    38,    12,   106,   104,   144,   147,    50,    50,    21,
      22,    98,    24,    25,    48,    -1,    -1,    -1,    -1,   122,
     123,    -1,   125,    -1,    -1,    -1,   129,    -1,    40,    41,
      42,    43,    44,     1,    -1,    -1,     4,     5,   141,    -1,
      52,   144,    10,    -1,    -1,   148,    -1,    15,    16,    17,
      18,    -1,    -1,    -1,   157,    23,    -1,    -1,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,     4,     5,    -1,    47,
      -1,    49,    10,    51,    -1,    -1,    -1,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,     4,     5,    -1,    47,
      -1,    49,    10,    51,    52,    -1,    -1,    15,    16,    17,
      18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,     4,     5,    -1,    47,
      -1,    49,    10,    51,    -1,    -1,    -1,    15,    16,    17,
      18,     6,     7,     8,     9,    23,    -1,    -1,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    21,    22,    -1,    24,
      25,    -1,    -1,    -1,    -1,     6,     7,     8,     9,    47,
      -1,    49,    -1,    51,    -1,    40,    41,    42,    43,    44,
      21,    22,    -1,    24,    25,    50,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    43,    44,    21,    22,    -1,    24,    25,    50,
      -1,    -1,    -1,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    21,    22,
      -1,    24,    25,    50,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    -1,    21,    22,    48,    24,    25,    -1,    -1,
      -1,    -1,    -1,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    21,    22,
      48,    24,    25,    -1,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    -1,    21,    22,    48,    24,    25,    -1,    -1,
       3,    -1,    -1,     6,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    21,    22,
      48,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    19,    -1,    21,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    20,    21,
      22,    -1,    24,    25,    -1,    -1,    -1,    -1,     6,     7,
       8,     9,    -1,    11,     6,     7,     8,     9,    40,    41,
      42,    43,    44,    21,    22,    -1,    24,    25,    -1,    21,
      22,    -1,    24,    25,    -1,     6,     7,     8,     9,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    -1,    40,    41,
      42,    43,    44,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    43,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    46,    55,    56,    57,    12,    13,    14,    32,    33,
      34,    35,    36,     0,     1,     4,     5,    10,    15,    16,
      17,    18,    23,    26,    27,    28,    29,    30,    31,    43,
      47,    49,    51,    61,    62,    64,    65,    71,     4,    58,
       3,    37,     3,     5,    60,    37,    59,    37,    58,     3,
       4,     4,    64,    64,    47,    47,    47,    47,     4,    64,
      64,    73,    66,    52,    64,    72,     3,     3,     6,     7,
       8,     9,    21,    22,    24,    25,    40,    41,    42,    43,
      44,     3,     3,     3,     3,     3,     4,     5,    39,    39,
      19,    64,    64,    64,    64,    39,    50,    48,    74,     4,
      26,    31,    47,    77,    50,    52,    53,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
       3,     3,    68,    70,    64,    50,    48,    48,    48,    63,
      64,    75,    65,    47,    76,    77,    67,    72,    64,    64,
      64,    20,    64,    64,    50,    77,    48,    50,    38,    52,
      11,    64,    48,    75,    50,    76,    64,    69,    77,    64,
      48
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 265 "parser.y"
    {
				   app_nesting_depth = 0;
				   destroy();
				   lastinputterm=NULL;
				}
    break;

  case 4:
#line 271 "parser.y"
    {
				   (yyval.root) = (yyvsp[(2) - (2)].root);
				}
    break;

  case 5:
#line 275 "parser.y"
    {
				   printf("no more input");
				   quit = true;
				   YYACCEPT;
				}
    break;

  case 6:
#line 283 "parser.y"
    {
				   if ((yyvsp[(3) - (4)].root) == NULL) (yyvsp[(3) - (4)].root)=lastinputterm;
				   (yyval.root) = (yyvsp[(3) - (4)].root);
				   inspect_driver((yyvsp[(3) - (4)].root));
				   YYACCEPT;
				}
    break;

  case 7:
#line 290 "parser.y"
    {
				   if ((yyvsp[(3) - (5)].root) == NULL) (yyvsp[(3) - (5)].root)=current_pos;
				   current_pos = inspect((yyvsp[(4) - (5)].num_const),(yyvsp[(3) - (5)].root));
				   YYACCEPT;
				}
    break;

  case 8:
#line 296 "parser.y"
    {
				   quit = true;
				   YYACCEPT;
				}
    break;

  case 9:
#line 301 "parser.y"
    {
				  include_file = (yyvsp[(3) - (4)].astring);
				  loading_mode = true;
				  printf("%s", include_file);
				  YYACCEPT;
				}
    break;

  case 10:
#line 308 "parser.y"
    {
				   user();
				   YYACCEPT;
				}
    break;

  case 11:
#line 313 "parser.y"
    {
				   menu((yyvsp[(3) - (4)].num_const));
				   YYACCEPT;
				}
    break;

  case 12:
#line 318 "parser.y"
    {
				   info((yyvsp[(3) - (4)].astring));
				   YYACCEPT;
				}
    break;

  case 13:
#line 323 "parser.y"
    {
				   save((yyvsp[(3) - (4)].astring),lastinputterm,NULL);
                                   free((yyvsp[(3) - (4)].astring));
				   YYACCEPT;
				}
    break;

  case 14:
#line 329 "parser.y"
    {
				   if (defined((yyvsp[(4) - (5)].st_bucket)))
				      save((yyvsp[(3) - (5)].astring),(yyvsp[(4) - (5)].st_bucket)->curr_binding->root,(yyvsp[(4) - (5)].st_bucket)->id);
				   else
				      {
					 signal_error(UNBOUND_VARIABLE);
					 YYACCEPT;
				      }
                                   free((yyvsp[(3) - (5)].astring));
				   YYACCEPT;
				}
    break;

  case 15:
#line 343 "parser.y"
    {
				   (yyval.root) = NULL;
				}
    break;

  case 16:
#line 347 "parser.y"
    {
				   if (defined((yyvsp[(1) - (1)].st_bucket)))
				      (yyval.root) = (yyvsp[(1) - (1)].st_bucket)->curr_binding->root;
				   else
				      {
					 signal_error(UNBOUND_VARIABLE);
					 YYACCEPT;
				      }
				 }
    break;

  case 17:
#line 359 "parser.y"
    {
				   (yyval.astring) = "";
				}
    break;

  case 18:
#line 363 "parser.y"
    {
				   (yyval.astring) = (yyvsp[(1) - (1)].astring);
				}
    break;

  case 19:
#line 368 "parser.y"
    {
				   (yyval.num_const) = -1;
				}
    break;

  case 20:
#line 372 "parser.y"
    {
				   (yyval.num_const) = (yyvsp[(1) - (1)].num_const);
				}
    break;

  case 21:
#line 377 "parser.y"
    {
				  lastinputterm = (yyvsp[(1) - (2)].term)->close(0);
				  current_pos = lastinputterm;
				  (yyval.root) = lastinputterm;
				  reduce_term((yyval.root));
				  YYACCEPT;
				}
    break;

  case 22:
#line 385 "parser.y"
    {
				  no_destroy();
				  YYACCEPT;
				}
    break;

  case 23:
#line 394 "parser.y"
    {
				  app_nesting_depth++;
				}
    break;

  case 24:
#line 398 "parser.y"
    {
				  app_nesting_depth--;
				  lastinputterm = (yyvsp[(5) - (5)].term)->close(1);
				  (yyval.root) = lastinputterm;
				  create_variable_binding((yyvsp[(2) - (5)].st_bucket),(yyval.root));
				}
    break;

  case 25:
#line 407 "parser.y"
    {
                                  (yyval.term) = (yyvsp[(1) - (1)].term);
                                }
    break;

  case 26:
#line 411 "parser.y"
    {
		 		  (yyval.term) = buildminusterm(app_nesting_depth, (yyvsp[(2) - (2)].term));
				}
    break;

  case 27:
#line 415 "parser.y"
    {
				  (yyval.term) = TERM::and_(app_nesting_depth,
						       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
				}
    break;

  case 28:
#line 420 "parser.y"
    {
				  (yyval.term) = buildorterm(app_nesting_depth,
						       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
				}
    break;

  case 29:
#line 425 "parser.y"
    {
		 		  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),LESS);
		 		}
    break;

  case 30:
#line 430 "parser.y"
    {
		 		  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),EQ);
		 		}
    break;

  case 31:
#line 435 "parser.y"
    {
				  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),NOTEQ);
		 		}
    break;

  case 32:
#line 440 "parser.y"
    {
		 		  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),MORE);
				}
    break;

  case 33:
#line 445 "parser.y"
    {
		 		  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),LEQ);
		 		}
    break;

  case 34:
#line 450 "parser.y"
    {
		 		  (yyval.term) = buildrelopterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),MEQ);
		 		}
    break;

  case 35:
#line 455 "parser.y"
    {
				  (yyval.term) = buildmatterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),ADD);
				}
    break;

  case 36:
#line 460 "parser.y"
    {
		 		  (yyval.term) = buildmatterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),SUB);
				}
    break;

  case 37:
#line 465 "parser.y"
    {
		 		  (yyval.term) = buildmatterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),PROD);
				}
    break;

  case 38:
#line 470 "parser.y"
    {
		 		  (yyval.term) = buildmatterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),DIV);
				}
    break;

  case 39:
#line 475 "parser.y"
    {
				  (yyval.term) = buildmatterm(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term),MOD);
		 		}
    break;

  case 40:
#line 482 "parser.y"
    {
				  (yyval.term) = TERM::true_(app_nesting_depth);
				}
    break;

  case 41:
#line 486 "parser.y"
    {
				  (yyval.term) = TERM::false_(app_nesting_depth);
				}
    break;

  case 42:
#line 490 "parser.y"
    {
				  (yyval.term) = buildintterm(app_nesting_depth,(yyvsp[(1) - (1)].num_const));
				}
    break;

  case 43:
#line 494 "parser.y"
    {
				  if (defined((yyvsp[(1) - (1)].st_bucket)))
				     (yyval.term) = buildvarterm(app_nesting_depth,(yyvsp[(1) - (1)].st_bucket));
				  else
				     {
					signal_error(UNBOUND_VARIABLE);
					YYERROR;
				     }
				}
    break;

  case 44:
#line 504 "parser.y"
    {
				  (yyval.term) = (yyvsp[(2) - (3)].term);
				}
    break;

  case 45:
#line 508 "parser.y"
    {
				  push_local_env();
                                  app_nesting_depth++;
				}
    break;

  case 46:
#line 513 "parser.y"
    {
                                  app_nesting_depth--;
                                }
    break;

  case 47:
#line 517 "parser.y"
    {
                                  pattmp=(yyvsp[(3) - (6)].pattern);
                                  (yyval.term) = 
                                    buildplambdaterm(app_nesting_depth,(yyvsp[(3) - (6)].pattern),(yyvsp[(6) - (6)].term));
                                  pattmp->release();
				  pop_local_env();
				}
    break;

  case 48:
#line 525 "parser.y"
    {
				  app_nesting_depth++;
				}
    break;

  case 49:
#line 529 "parser.y"
    {
				  app_nesting_depth--;
				  push_local_env();
				  create_variable_binding((yyvsp[(2) - (6)].st_bucket), NULL);
				}
    break;

  case 50:
#line 535 "parser.y"
    {
				  (yyval.term) = buildletinterm(app_nesting_depth,
						      (yyvsp[(2) - (8)].st_bucket),(yyvsp[(5) - (8)].term),(yyvsp[(8) - (8)].term));
				  pop_local_env();
				}
    break;

  case 51:
#line 541 "parser.y"
    {
				  push_local_env();
				  create_variable_binding((yyvsp[(2) - (3)].st_bucket), NULL);
				  app_nesting_depth++;
				 }
    break;

  case 52:
#line 547 "parser.y"
    {
				  (yyval.term) = build_mu_term(--app_nesting_depth,
						     (yyvsp[(2) - (5)].st_bucket),(yyvsp[(5) - (5)].term));
				  pop_local_env();
				 }
    break;

  case 53:
#line 553 "parser.y"
    {
				  (yyval.term) = buildifelseterm(app_nesting_depth,
						       (yyvsp[(2) - (6)].term),(yyvsp[(4) - (6)].term),(yyvsp[(6) - (6)].term));
				}
    break;

  case 54:
#line 558 "parser.y"
    {
				  (yyval.term) = (yyvsp[(1) - (1)].term);
				}
    break;

  case 55:
#line 563 "parser.y"
    {
				  (yyval.term) = buildnotterm(app_nesting_depth,
		 		  		       (yyvsp[(2) - (2)].term));
				}
    break;

  case 56:
#line 568 "parser.y"
    {
		 		  (yyval.term) = buildlist(app_nesting_depth,
		 		  		       (yyvsp[(3) - (6)].term),(yyvsp[(5) - (6)].term));
		 		}
    break;

  case 57:
#line 573 "parser.y"
    {
		 		  (yyval.term) = TERM::car(app_nesting_depth,
		 		  		       (yyvsp[(3) - (4)].term));
		 		}
    break;

  case 58:
#line 578 "parser.y"
    {
				  (yyval.term) = buildcdrterm(app_nesting_depth,
		 		  		       (yyvsp[(3) - (4)].term));
		 		}
    break;

  case 59:
#line 583 "parser.y"
    {
		 		  (yyval.term) = buildtestnil(app_nesting_depth,
		 		  		       (yyvsp[(3) - (4)].term));
				}
    break;

  case 60:
#line 590 "parser.y"
    {
				  (yyval.term) = buildnillist(app_nesting_depth);
				}
    break;

  case 61:
#line 594 "parser.y"
    {
                                  (yyval.term) = buildnillist(app_nesting_depth);
				}
    break;

  case 62:
#line 598 "parser.y"
    {
				  (yyval.term) = (yyvsp[(2) - (2)].term);
				}
    break;

  case 63:
#line 603 "parser.y"
    {
				  (yyval.term) = buildlist(app_nesting_depth,
						(yyvsp[(1) - (2)].term),NULL);
				}
    break;

  case 64:
#line 608 "parser.y"
    {
				  (yyval.term) = buildlist(app_nesting_depth,
						(yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
				}
    break;

  case 65:
#line 613 "parser.y"
    {
				  (yyval.term) = buildlist(app_nesting_depth,
						 (yyvsp[(1) - (4)].term),(yyvsp[(3) - (4)].term));
				}
    break;

  case 67:
#line 621 "parser.y"
    {
		 		  (yyval.term) = buildlist(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
                                }
    break;

  case 68:
#line 626 "parser.y"
    {
				  app_nesting_depth++;
				}
    break;

  case 69:
#line 630 "parser.y"
    { app_nesting_depth--;
				  (yyval.term) = buildappterm(app_nesting_depth,(yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
				}
    break;

  case 70:
#line 637 "parser.y"
    {
                                  (yyval.term)=(yyvsp[(1) - (1)].term);
                                }
    break;

  case 71:
#line 641 "parser.y"
    {
		 		  (yyval.term) = buildlist(app_nesting_depth,
		 		  		       (yyvsp[(1) - (3)].term),(yyvsp[(3) - (3)].term));
                                }
    break;

  case 72:
#line 648 "parser.y"
    {
                                  (yyval.pattern)=(yyvsp[(1) - (1)].pattern);
                                }
    break;

  case 73:
#line 652 "parser.y"
    {
                                  pattmp=(PATTERN *)malloc(sizeof(PATTERN));
                                  pattmp->term=
                                    buildlist1(app_nesting_depth,
                                              (yyvsp[(1) - (3)].pattern)->term,(yyvsp[(3) - (3)].pattern)->term);
                                  pattmp->var_list=
                                    (yyvsp[(1) - (3)].pattern)->var_list->merge((yyvsp[(3) - (3)].pattern)->var_list);
                                  free((yyvsp[(1) - (3)].pattern));
                                  free((yyvsp[(3) - (3)].pattern));
                                  (yyval.pattern)=pattmp;
                                }
    break;

  case 74:
#line 665 "parser.y"
    {
                                  pattmp=(PATTERN *)malloc(sizeof(PATTERN));
                                  pattmp->term=
                                    buildlist1(app_nesting_depth,
                                              (yyvsp[(3) - (6)].pattern)->term,(yyvsp[(5) - (6)].pattern)->term);
                                  pattmp->var_list=
                                    mergevarlist((yyvsp[(3) - (6)].pattern)->var_list,(yyvsp[(5) - (6)].pattern)->var_list);
                                  free((yyvsp[(3) - (6)].pattern));
                                  free((yyvsp[(5) - (6)].pattern));
                                  (yyval.pattern)=pattmp;
                                }
    break;

  case 75:
#line 677 "parser.y"
    {
                                  pattmp=(PATTERN *)malloc(sizeof(PATTERN));
                                  pattmp->term=buildnillist(app_nesting_depth);
                                  pattmp->var_list=NULL;
                                  (yyval.pattern)=pattmp;
                                }
    break;

  case 76:
#line 684 "parser.y"
    {
                                  (yyval.pattern)=(yyvsp[(2) - (3)].pattern);
                                }
    break;

  case 77:
#line 688 "parser.y"
    {
                                  pattmp=(PATTERN *)malloc(sizeof(PATTERN));
                                  pattmp->term=
                                    buildvoidterm(app_nesting_depth);
                                  create_variable_binding((yyvsp[(1) - (1)].st_bucket), NULL);
                                  pattmp->var_list=
                                    makevarlist((yyvsp[(1) - (1)].st_bucket),pattmp->term);
                                  (yyval.pattern)=pattmp;
                                }
    break;

  case 78:
#line 699 "parser.y"
    {
                                  error_detected = true;
                                  yyerrok;
                                  YYACCEPT;
				}
    break;


/* Line 1267 of yacc.c.  */
#line 2310 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 706 "parser.y"


/* The following function checks if an identifier has been */
/* previously declared */
static bool defined(
	STBUCKET	*st
)
			      /* pointer to the bucket for the */
			      /* identifier */
{
        return st->curr_binding != NULL;
}

