/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 144 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

