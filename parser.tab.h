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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    NUM = 258,                     /* NUM  */
    ID = 259,                      /* ID  */
    OPERADOR_MULTIPLICATIVO = 260, /* OPERADOR_MULTIPLICATIVO  */
    OR = 261,                      /* OR  */
    MENOS = 262,                   /* MENOS  */
    MAIS = 263,                    /* MAIS  */
    OPERADOR_RELACIONAL = 264,     /* OPERADOR_RELACIONAL  */
    OPERADOR_ATRIBUICAO = 265,     /* OPERADOR_ATRIBUICAO  */
    DO = 266,                      /* DO  */
    WHILE = 267,                   /* WHILE  */
    ELSE = 268,                    /* ELSE  */
    THEN = 269,                    /* THEN  */
    IF = 270,                      /* IF  */
    END = 271,                     /* END  */
    BEGIN_TOKEN = 272,             /* BEGIN_TOKEN  */
    DOIS_PONTOS = 273,             /* DOIS_PONTOS  */
    PONTO_VIRGULA = 274,           /* PONTO_VIRGULA  */
    FECHA_PARENTESES = 275,        /* FECHA_PARENTESES  */
    ABRE_PARENTESES = 276,         /* ABRE_PARENTESES  */
    PROCEDURE = 277,               /* PROCEDURE  */
    FUNCTION = 278,                /* FUNCTION  */
    REAL = 279,                    /* REAL  */
    INTEIRO = 280,                 /* INTEIRO  */
    VAR = 281,                     /* VAR  */
    PONTO_FINAL = 282,             /* PONTO_FINAL  */
    PROGRAM = 283,                 /* PROGRAM  */
    EOL = 284                      /* EOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
