%{
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void yyerror(const char * s);
int yylex(void);
%}

%define api.value.type { jacc_yystype_t }

%token IDENT
%token CHARLIT        // `byte` (we don't support multibyte)
%token STRING         // "<here>"
%token NUMBER         // Numerical value - will be specified as INT, DOUBLE, ETC.
%token INDSEL         // ->
%token PLUSPLUS       // ++
%token MINUSMINUS     // --
%token SHL            // <<
%token SHR            // >>
%token LTEQ           // <=
%token GTEQ           // >=
%token EQEQ           // ==
%token NOTEQ          // !=
%token LOGAND         // &&
%token LOGOR          // ||
%token ELLIPSIS       // ...
%token TIMESEQ        // *=
%token DIVEQ          // /=
%token MODEQ          // %=
%token PLUSEQ         // +=
%token MINUSEQ        // -=
%token SHLEQ          // <<=
%token SHREQ          // >>=
%token ANDEQ          // &=
%token OREQ           // |=
%token XOREQ          // ^=
%token AUTO           // auto
%token BREAK          // break
%token CASE           // case
%token CHAR           // char
%token CONST          // const
%token CONTINUE       // continue
%token DEFAULT        // default
%token DO             // do
%token DOUBLE         // double
%token ELSE           // else
%token ENUM           // enum
%token EXTERN         // extern
%token FLOAT          // float
%token FOR            // for
%token GOTO           // goto
%token IF             // if
%token INLINE         // inline
%token INT            // int
%token LONG           // long
%token REGISTER       // register
%token RESTRICT       // restrict
%token RETURN         // return
%token SHORT          // short
%token SIGNED         // signed
%token SIZEOF         // sizeof
%token STATIC         // static
%token STRUCT         // struct
%token SWITCH         // switch
%token TYPEDEF        // typedef
%token UNION          // union
%token UNSIGNED       // unsigned
%token VOID           // void
%token VOLATILE       // volatile
%token WHILE          // while
%token _BOOL          // _bool
%token _COMPLEX       // _complex
%token _IMAGINARY      // _imaginary

%start statement

%%

primary_expression
    : IDENT
    | NUMBER
    | STRING
    | '(' expression ')'
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENT
    | postfix_expression INDSEL IDENT
    | postfix_expression PLUSPLUS
    | postfix_expression MINUSMINUS
    // '(' type_name ')' '{' initializer_list '}'
    // '(' type_name ')' '{' initializer_list, '}'
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression
    | PLUSPLUS unary_expression
    | MINUSMINUS unary_expression
    | unary_operator cast_expression
    | SIZEOF unary_expression
    // | SIZEOF '(' type_name ')'
    ;

unary_operator
    : '&'
    | '*'
    | '+'
    | '-'
    | '~'
    | '!'
    ;

cast_expression
    : unary_expression
//    | '(' type_name ')' cast_expression
    ;

multiplicative_expressions
    : cast_expression
    | multiplicative_expressions '*' cast_expression
    | multiplicative_expressions '/' cast_expression
    | multiplicative_expressions '%' cast_expression

additive_expression
    : multiplicative_expressions
    | additive_expression '+' multiplicative_expressions { printf("PLEASE!\n"); }
    | additive_expression '-' multiplicative_expressions
    ;

shift_expression
    : additive_expression
    | shift_expression SHL additive_expression
    | shift_expression SHR additive_expression
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LTEQ shift_expression
    | relational_expression GTEQ shift_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQEQ relational_expression
    | equality_expression NOTEQ relational_expression
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression 
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression LOGAND inclusive_or_expression 
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOGOR logical_and_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignemnt_operator assignment_expression
    ;

assignemnt_operator
    : '='
    | TIMESEQ
    | DIVEQ
    | MODEQ
    | PLUSEQ
    | MINUSEQ
    | SHLEQ
    | SHREQ
    | ANDEQ
    | XOREQ
    | OREQ
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression
    ;

statement
    : expression ';'
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    fprintf(stderr, "Did I fail?: %d\n", yyparse());
    return 0;
}
