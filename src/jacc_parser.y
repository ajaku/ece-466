%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(const char * s);
int yylex(void);

%}

%union {
    struct jacc_ast_node *ast_p;
}

%token <ast_p> IDENT
%token <ast_p> CHARLIT        // `byte` (we don't support multibyte)
%token <ast_p> STRING         // "<here>"
%token <ast_p> NUMBER         // Numerical value - will be specified as INT, DOUBLE, ETC.
%token <ast_p> INDSEL         // ->
%token <ast_p> PLUSPLUS       // ++
%token <ast_p> MINUSMINUS     // --
%token <ast_p> SHL            // <<
%token <ast_p> SHR            // >>
%token <ast_p> LTEQ           // <=
%token <ast_p> GTEQ           // >=
%token <ast_p> EQEQ           // ==
%token <ast_p> NOTEQ          // !=
%token <ast_p> LOGAND         // &&
%token <ast_p> LOGOR          // ||
%token <ast_p> ELLIPSIS       // ...
%token <ast_p> TIMESEQ        // *=
%token <ast_p> DIVEQ          // /=
%token <ast_p> MODEQ          // %=
%token <ast_p> PLUSEQ         // +=
%token <ast_p> MINUSEQ        // -=
%token <ast_p> SHLEQ          // <<=
%token <ast_p> SHREQ          // >>=
%token <ast_p> ANDEQ          // &=
%token <ast_p> OREQ           // |=
%token <ast_p> XOREQ          // ^=
%token <ast_p> AUTO           // auto
%token <ast_p> BREAK          // break
%token <ast_p> CASE           // case
%token <ast_p> CHAR           // char
%token <ast_p> CONST          // const
%token <ast_p> CONTINUE       // continue
%token <ast_p> DEFAULT        // default
%token <ast_p> DO             // do
%token <ast_p> DOUBLE         // double
%token <ast_p> ELSE           // else
%token <ast_p> ENUM           // enum
%token <ast_p> EXTERN         // extern
%token <ast_p> FLOAT          // float
%token <ast_p> FOR            // for
%token <ast_p> GOTO           // goto
%token <ast_p> IF             // if
%token <ast_p> INLINE         // inline
%token <ast_p> INT            // int
%token <ast_p> LONG           // long
%token <ast_p> REGISTER       // register
%token <ast_p> RESTRICT       // restrict
%token <ast_p> RETURN         // return
%token <ast_p> SHORT          // short
%token <ast_p> SIGNED         // signed
%token <ast_p> SIZEOF         // sizeof
%token <ast_p> STATIC         // static
%token <ast_p> STRUCT         // struct
%token <ast_p> SWITCH         // switch
%token <ast_p> TYPEDEF        // typedef
%token <ast_p> UNION          // union
%token <ast_p> UNSIGNED       // unsigned
%token <ast_p> VOID           // void
%token <ast_p> VOLATILE       // volatile
%token <ast_p> WHILE          // while
%token <ast_p> _BOOL          // _bool
%token <ast_p> _COMPLEX       // _complex
%token <ast_p> _IMAGINARY      // _imaginary

//%start statement

%type <ast_p> primary_expression

%%

primary_expression
    : IDENT     
    | NUMBER    
    | STRING    
    | CHARLIT   
    //| '(' IDENT ')' 
    | IDENT IDENT IDENT
    {
        jacc_ast_node_t *temp1 = $1;
        jacc_ast_node_t *temp2 = $2;
        jacc_ast_node_t *temp3 = $3;
    }
    ;

/*
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
    */

%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    fprintf(stderr, "Did I fail?: %d\n", yyparse());
    return 0;
}
