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

#ifndef YY_YY_JACC_PARSER_TAB_H_INCLUDED
# define YY_YY_JACC_PARSER_TAB_H_INCLUDED
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
    TOKEOF = 258,                  /* TOKEOF  */
    IDENT = 259,                   /* IDENT  */
    CHARLIT = 260,                 /* CHARLIT  */
    STRING = 261,                  /* STRING  */
    NUMBER = 262,                  /* NUMBER  */
    INDSEL = 263,                  /* INDSEL  */
    PLUSPLUS = 264,                /* PLUSPLUS  */
    MINUSMINUS = 265,              /* MINUSMINUS  */
    SHL = 266,                     /* SHL  */
    SHR = 267,                     /* SHR  */
    LTEQ = 268,                    /* LTEQ  */
    GTEQ = 269,                    /* GTEQ  */
    EQEQ = 270,                    /* EQEQ  */
    NOTEQ = 271,                   /* NOTEQ  */
    LOGAND = 272,                  /* LOGAND  */
    LOGOR = 273,                   /* LOGOR  */
    ELLIPSIS = 274,                /* ELLIPSIS  */
    TIMESEQ = 275,                 /* TIMESEQ  */
    DIVEQ = 276,                   /* DIVEQ  */
    MODEQ = 277,                   /* MODEQ  */
    PLUSEQ = 278,                  /* PLUSEQ  */
    MINUSEQ = 279,                 /* MINUSEQ  */
    SHLEQ = 280,                   /* SHLEQ  */
    SHREQ = 281,                   /* SHREQ  */
    ANDEQ = 282,                   /* ANDEQ  */
    OREQ = 283,                    /* OREQ  */
    XOREQ = 284,                   /* XOREQ  */
    AUTO = 285,                    /* AUTO  */
    BREAK = 286,                   /* BREAK  */
    CASE = 287,                    /* CASE  */
    CHAR = 288,                    /* CHAR  */
    CONST = 289,                   /* CONST  */
    CONTINUE = 290,                /* CONTINUE  */
    DEFAULT = 291,                 /* DEFAULT  */
    DO = 292,                      /* DO  */
    DOUBLE = 293,                  /* DOUBLE  */
    ELSE = 294,                    /* ELSE  */
    ENUM = 295,                    /* ENUM  */
    EXTERN = 296,                  /* EXTERN  */
    FLOAT = 297,                   /* FLOAT  */
    FOR = 298,                     /* FOR  */
    GOTO = 299,                    /* GOTO  */
    IF = 300,                      /* IF  */
    INLINE = 301,                  /* INLINE  */
    INT = 302,                     /* INT  */
    LONG = 303,                    /* LONG  */
    REGISTER = 304,                /* REGISTER  */
    RESTRICT = 305,                /* RESTRICT  */
    RETURN = 306,                  /* RETURN  */
    SHORT = 307,                   /* SHORT  */
    SIGNED = 308,                  /* SIGNED  */
    SIZEOF = 309,                  /* SIZEOF  */
    STATIC = 310,                  /* STATIC  */
    STRUCT = 311,                  /* STRUCT  */
    SWITCH = 312,                  /* SWITCH  */
    TYPEDEF = 313,                 /* TYPEDEF  */
    UNION = 314,                   /* UNION  */
    UNSIGNED = 315,                /* UNSIGNED  */
    VOID = 316,                    /* VOID  */
    VOLATILE = 317,                /* VOLATILE  */
    WHILE = 318,                   /* WHILE  */
    _BOOL = 319,                   /* _BOOL  */
    _COMPLEX = 320,                /* _COMPLEX  */
    _IMAGINARY = 321               /* _IMAGINARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  jacc_yystype_t  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_JACC_PARSER_TAB_H_INCLUDED  */
