%{
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void yyerror(const char * s);
int yylex(void);
%}

%define api.value.type { jacc_yystype_t }

%token TOKEOF
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

%%

primary_expression
    : IDENT
    | NUMBER /* WAS CONST */
    | STRING
    | parenthesized_expression
    ;

parenthesized_expression
    : '(' expression ')'
    ;

postfix_expression
    : primary_expression
    | subscript_expression
    | component_selection_expression
    | function_call
    | postincrement_expression
    | postdecrement_expression
    | /* compound_literal */ /* not dealing with for now */
    ;

subscript_expression
    : postfix_expression '[' expression ']'
    ;

component_selection_expression
    : direct_component_selection
    | indirect_component_selection
    ;

direct_component_selection
    : postfix_expression '.' IDENT
    ;

indirect_component_selection
    : postfix_expression INDSEL IDENT
    ;

function_call
    : postfix_expression '(' expression_list ')'
    ;

expression_list
    : assignment_expression
    | expression_list ',' assignment_expression
    ;

postincrement_expression
    : postfix_expression PLUSPLUS
    ;

postdecrement_expression
    : postfix_expression MINUSMINUS
    ;

cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;

unary_expression
    : postfix_expression
    | sizeof_expression
    | unary_minus_expression
    | unary_plus_expression
    | logical_negation_expression
    | bitwise_negation_expression
    | address_expression
    | indirection_expression
    | preincrement_expression
    | predecrement_expression
    ;

sizeof_expression
    : SIZEOF '(' type_name ')'
    | SIZEOF unary_expression
    ;

unary_minus_expression
    : '-' cast_expression
    ;

unary_plus_expression
    : '+' cast_expression
    ;

logical_negation_expression
    : '!' cast_expression
    ;

bitwise_negation_expression
    : '~' cast_expression
    ;

address_expression
    : '&' cast_expression
    ;

indirection_expression
    : '*' cast_expression
    ;

preincrement_expression
    : PLUSPLUS unary_expression
    ;

predecrement_expression
    : MINUSMINUS unary_expression
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression multi_op cast_expression
    ;

multi_op
    : '*'
    | '/'
    | '%'
    ;

additive_expression
    : multiplicative_expression
    | additive_expression add_op multiplicative_expression
    ;

add_op
    : '+'
    | '-'
    ;

shift_expression
    : additive_expression
    | shift_expression shift_op additive_expression
    ;

shift_op
    : SHL
    | SHR
    ;

relational_expression
    : shift_expression
    | relational_expression relational_op shift_expression
    ;

relational_op
    : '<'
    | LTEQ
    | '>'
    | GTEQ
    ;

equality_expression
    : relational_expression
    | equality_expression equality_op relational_expression
    ;

equality_op
    : EQEQ
    | NOTEQ
    ;

// Review bitwise bozos

bitwise_or_expression
    : bitwise_xor_expression
    | bitwise_or_expression '|' bitwise_xor_expression
    ;

bitwise_xor_expression
    : bitwise_and_expression
    | bitwise_xor_expression '^' bitwise_and_expression
    ;

bitwise_and_expression
    : equality_expression
    | bitwise_and_expression '&' equality_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOGOR logical_and_expression
    ;

logical_and_expression
    : bitwise_or_expression
    | logical_and_expression LOGAND bitwise_or_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_op assignment_expression
    ;

assignment_op
    : '='
    | PLUSEQ
    | MINUSEQ
    | TIMESEQ
    | DIVEQ
    | MODEQ
    | SHLEQ
    | SHREQ
    | ANDEQ
    | XOREQ
    | OREQ
    ;

comma_expression
    : assignment_expression
    | comma_expression ',' assignment_expression
    ;

expression:
    comma_expression
    ;

// definitely wrong, temporary until more support added
type_name
    : VOID
    | CHAR
    | SHORT
    | INT
    | LONG
    | FLOAT
    | DOUBLE
    | SIGNED
    | UNSIGNED
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}
