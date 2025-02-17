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

%left '+' '-'
%left '*' '/'

%%

exp: NUMBER {
    $$ = $1;  // Correct: Directly passing token
}
| exp '+' exp {
    $$.token.data.int_type = $1.token.data.int_type + $3.token.data.int_type;
    printf("Value: %d\n", $$.token.data.int_type);
}

%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    yyparse();
    return 0;
}
