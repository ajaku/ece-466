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


// Precedence = 16
%left '(' ')' '[' ']'  '.' INDSEL PLUSPLUS MINUSMINUS
// Precedence = 15
%right SIZEOF '~' // '|' '-' '+' '&' '*'
// Precedence = 14
// %left type_name
// Precedence = 13
%left '*' '/' '%'
// Precedence = 12
%left '+' '-'
// Precedence = 11
%left SHL SHR
// Precedence = 10
%left '<' '>' LTEQ GTEQ
// Precedence = 9
%left EQEQ NOTEQ
// Precedence = 8
%left '&'
// Precedence = 7
%left '^'
// Precedence = 6
%left '|'
// Precedence = 5
%left LOGAND
// Precedence = 4
%left LOGOR
// Precedence = 3
%right '?' ':'
// Precedence = 2
%right '=' PLUSEQ MINUSEQ TIMESEQ DIVEQ MODEQ SHLEQ SHREQ ANDEQ XOREQ OREQ
// Precedence = 1
%left ','

%%

expression
    : IDENT /* 6.5.1 Primary Expressions */
    { printf("IDENT\n"); }

    | NUMBER
    { printf("NUMBER\n"); }

    | CHARLIT
    { printf("CHARLIT\n"); }

    | STRING
    { printf("IDENT\n"); }

    | '(' expression ')'
    { printf("(expression)\n"); }

    /* 6.5.2 Postfix Operators */
    | expression '[' expression ']'
    { printf("expression[expression]\n"); }

    | expression '(' ')'
    { printf("expression()\n"); }

    | expression '(' expression ')'
    { printf("expression(argument)\n"); }

    | expression '.' IDENT
    { printf("expression.ident\n"); }

    | expression INDSEL IDENT
    { printf("expressiont->ident\n"); }

    | expression PLUSPLUS
    { printf("expression++\n"); }

    | expression MINUSMINUS
    { printf("expression--\n"); }

    /* 6.5.3 Unary Operators */
    | PLUSPLUS expression // right-to left (conflicts with earlier def, what do we do!)
    { printf("++expression\n"); }

    | MINUSMINUS expression // right-to left (conflicts with earlier def, what do we do!)
    { printf("--expression\n"); }

    // Would be cast operations, but no type support atm

    | SIZEOF expression
    { printf("SIZEOF expression\n"); }

    // Would be SIZEOF type name, but no types

    /* 6.5.5 Multiplicative Operators */
    | expression '*' expression
    { printf("expression * expression\n"); }
     
    | expression '/' expression
    { printf("expression / expression\n"); }

    | expression '%' expression
    { printf("expression %% expression\n"); }

    /* 6.5.6 Additive Operators */
    | expression '+' expression
    { printf("expression + expression\n"); }

    | expression '-' expression
    { printf("expression - expression\n"); }

    /* 6.5.7 Bitwise Shift Operators */
    | expression SHL expression
    { printf("expression << expression\n"); }

    | expression SHR expression
    { printf("expression >> expression\n"); }

    /* 6.5.8 Relational Operators */
    | expression '<' expression
    { printf("expression < expression\n"); }

    | expression '>' expression
    { printf("expression > expression\n"); }

    | expression LTEQ expression
    { printf("expression <= expression\n"); }

    | expression GTEQ expression
    { printf("expression >= expression\n"); }

    /* 6.5.9 Equality Operators */
    | expression EQEQ expression
    { printf("expression == expression\n"); }

    | expression NOTEQ expression
    { printf("expression != expression\n"); }

    /* 6.5.10 Bitwise AND Operator */
    | expression '&' expression
    { printf("expression & expression\n"); }

    /* 6.5.11 Bitwise XOR Operator */
    | expression '^' expression
    { printf("expression ^ expression\n"); }

    /* 6.5.12 Bitwise OR Operator */
    | expression '|' expression
    { printf("expression | expression\n"); }

    /* 6.5.13 Logical AND Operator */
    | expression LOGAND expression
    { printf("expression && expression\n"); }

    /* 6.5.14 Logical OR Operator */
    | expression LOGOR expression
    { printf("expression || expression\n"); }

    /* 6.5.15 Conditional Operators */
    | expression '?' expression
    { printf("expression ? expression\n"); }

    | expression ':' expression
    { printf("expression : expression\n"); }

    /* 6.5.16 Assignment Operators */
    | expression '=' expression
    { printf("expression = expression\n"); }

    | expression PLUSEQ expression
    { printf("expression += expression\n"); }

    | expression MINUSEQ expression
    { printf("expression -= expression\n"); }

    | expression TIMESEQ expression
    { printf("expression *= expression\n"); }

    | expression DIVEQ expression
    { printf("expression \\= expression\n"); }

    | expression MODEQ expression
    { printf("expression %%= expression\n"); }

    | expression SHLEQ expression
    { printf("expression <<= expression\n"); }

    | expression SHREQ expression
    { printf("expression >>= expression\n"); }

    | expression ANDEQ expression
    { printf("expression &= expression\n"); }

    | expression XOREQ expression
    { printf("expression ^= expression\n"); }

    | expression OREQ expression
    { printf("expression |= expression\n"); }

    /* 6.5.17 Comma Operator */
    | expression ',' expression
    { printf("expression , expression\n"); }
    ;

%%

void yyerror(const char *s) {
    //fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}
