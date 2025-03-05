%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(const char * s);
int yylex(void);

jacc_ast_node_t final_ast;

%}

%union {
    struct jacc_lex_tok tok;
    struct jacc_ast_node *ast_p;
}

%token <tok> IDENT
%token <tok> CHARLIT        // byte (we don't support multibyte)
%token <tok> STRING         // "<here>"
%token <tok> NUMBER         // Numerical value - will be specified as INT, DOUBLE, ETC.
%token <tok> '&'
%token <tok> '*'
%token <tok> '+'
%token <tok> '-'
%token <tok> '~'
%token <tok> '!'
%token <tok> INDSEL         // ->
%token <tok> PLUSPLUS       // ++
%token <tok> MINUSMINUS     // --
%token <tok> SHL            // <<
%token <tok> SHR            // >>
%token <tok> LTEQ           // <=
%token <tok> GTEQ           // >=
%token <tok> EQEQ           // ==
%token <tok> NOTEQ          // !=
%token <tok> LOGAND         // &&
%token <tok> LOGOR          // ||
%token <tok> ELLIPSIS       // ...
%token <tok> TIMESEQ        // *=
%token <tok> DIVEQ          // /=
%token <tok> MODEQ          // %=
%token <tok> PLUSEQ         // +=
%token <tok> MINUSEQ        // -=
%token <tok> SHLEQ          // <<=
%token <tok> SHREQ          // >>=
%token <tok> ANDEQ          // &=
%token <tok> OREQ           // |=
%token <tok> XOREQ          // ^=
%token <tok> AUTO           // auto
%token <tok> BREAK          // break
%token <tok> CASE           // case
%token <tok> CHAR           // char
%token <tok> CONST          // const
%token <tok> CONTINUE       // continue
%token <tok> DEFAULT        // default
%token <tok> DO             // do
%token <tok> DOUBLE         // double
%token <tok> ELSE           // else
%token <tok> ENUM           // enum
%token <tok> EXTERN         // extern
%token <tok> FLOAT          // float
%token <tok> FOR            // for
%token <tok> GOTO           // goto
%token <tok> IF             // if
%token <tok> INLINE         // inline
%token <tok> INT            // int
%token <tok> LONG           // long
%token <tok> REGISTER       // register
%token <tok> RESTRICT       // restrict
%token <tok> RETURN         // return
%token <tok> SHORT          // short
%token <tok> SIGNED         // signed
%token <tok> SIZEOF         // sizeof
%token <tok> STATIC         // static
%token <tok> STRUCT         // struct
%token <tok> SWITCH         // switch
%token <tok> TYPEDEF        // typedef
%token <tok> UNION          // union
%token <tok> UNSIGNED       // unsigned
%token <tok> VOID           // void
%token <tok> VOLATILE       // volatile
%token <tok> WHILE          // while
%token <tok> _BOOL          // _bool
%token <tok> _COMPLEX       // _complex
%token <tok> _IMAGINARY      // _imaginary

%type <ast_p> primary_expression
%type <ast_p> postfix_expression
%type <ast_p> argument_expression_list
%type <ast_p> assignment_expression
%type <ast_p> unary_expression

%type <tok> unary_operator
%type <tok> assignment_operator 

%type <ast_p> cast_expression
%type <ast_p> multiplicative_expression
%type <ast_p> additive_expression
%type <ast_p> shift_expression
%type <ast_p> relational_expression
%type <ast_p> equality_expression
%type <ast_p> and_expression
%type <ast_p> exclusive_or_expression
%type <ast_p> inclusive_or_expression
%type <ast_p> logical_and_expression
%type <ast_p> logical_or_expression
%type <ast_p> conditional_expression
%type <ast_p> expression
%type <ast_p> statement

%start statement

%%

primary_expression
    : IDENT {
        $$ = jacc_alloc_base_node(JACC_IDENT_AST, &$1);
    }   
    | NUMBER {
        $$ = jacc_alloc_base_node(JACC_NUM_AST, &$1);
    }
    | STRING {
        $$ = jacc_alloc_base_node(JACC_STRING_AST, &$1);
    }
    | CHARLIT {
        $$ = jacc_alloc_base_node(JACC_STRING_AST, &$1);
    }
    | '(' expression ')' { $$ = $2; }
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']' {
        jacc_ast_node_t *binop_ast = jacc_alloc_binop_node('+', $1, $3);
        $$ = jacc_alloc_generic_node(JACC_DEREF_AST, binop_ast, NULL);
        jacc_ast_node_t *temp = $$;
        printf("Hi\n");
    } 
    | postfix_expression '(' argument_expression_list ')' {
        $$ = jacc_alloc_generic_node(JACC_FCALL_AST, $1, $3);
    }
    | postfix_expression '.' IDENT {
        jacc_ast_node_t *ident_ast = jacc_alloc_base_node(JACC_IDENT_AST, &$3);
        $$ = jacc_alloc_generic_node(JACC_SEL_AST, $1, ident_ast);
    }
    | postfix_expression INDSEL IDENT {
        jacc_ast_node_t *ident_ast = jacc_alloc_base_node(JACC_IDENT_AST, &$3);
        jacc_ast_node_t *generic_ast = jacc_alloc_generic_node(JACC_DEREF_AST, $1, NULL);
        $$ = jacc_alloc_generic_node(JACC_SEL_AST, generic_ast, ident_ast);
    }
    | postfix_expression PLUSPLUS {

        jacc_ast_node_t *plus_plus_ast = jacc_alloc_base_node(JACC_NUM_AST, &$2);
        $$ = jacc_alloc_generic_node(JACC_POST_OP_AST, $1, plus_plus_ast);
    }
    | postfix_expression MINUSMINUS {
        jacc_ast_node_t *minus_minus_ast = jacc_alloc_base_node(JACC_NUM_AST, &$2);
        $$ = jacc_alloc_generic_node(JACC_POST_OP_AST, $1, minus_minus_ast);
    }
    // '(' type_name ')' '{' initializer_list '}'
    // '(' type_name ')' '{' initializer_list, '}'
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression {
        $$ = jacc_alloc_binop_node(',', $1, $3);
    }
    ;

unary_expression
    : postfix_expression
    | PLUSPLUS unary_expression {
        jacc_lex_tok_t *plus_one_tok = malloc(sizeof(jacc_lex_tok_t));
        plus_one_tok->data_type = JACC_TYPE_INT;
        plus_one_tok->size = sizeof(int);
        plus_one_tok->data.int_d = 1;

        jacc_ast_node_t *plus_one_ast = jacc_alloc_base_node(JACC_NUM_AST, plus_one_tok);

        $$ = jacc_alloc_binop_node('+', $2, plus_one_ast);
    }
    | MINUSMINUS unary_expression {
        jacc_lex_tok_t *minus_one_tok = malloc(sizeof(jacc_lex_tok_t));
        minus_one_tok->data_type = JACC_TYPE_INT;
        minus_one_tok->size = sizeof(int);
        minus_one_tok->data.int_d = -1;

        jacc_ast_node_t *minus_one_ast = jacc_alloc_base_node(JACC_NUM_AST, minus_one_tok);

        $$ = jacc_alloc_binop_node('-', $2, minus_one_ast);
    }
    | unary_operator cast_expression {
        //$$ = jacc_alloc_generic_node(JACC_CAST_AST, $1, $2);
        printf("When does this occur\n");

    }
    | SIZEOF unary_expression {
        $$ = jacc_alloc_generic_node(JACC_SIZEOF_AST, $2, NULL);
    }
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

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression {
        $$ = jacc_alloc_binop_node('*', $1, $3);
    }
    | multiplicative_expression '/' cast_expression {
        $$ = jacc_alloc_binop_node('/', $1, $3);
    }
    | multiplicative_expression '%' cast_expression {
        $$ = jacc_alloc_binop_node('/', $1, $3);
    }
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression {
        $$ = jacc_alloc_binop_node('+', $1, $3);
    }
    | additive_expression '-' multiplicative_expression {
        $$ = jacc_alloc_binop_node('-', $1, $3);
    }
    ;

shift_expression
    : additive_expression
    | shift_expression SHL additive_expression {
        $$ = jacc_alloc_binop_node(SHL, $1, $3);
    }
    | shift_expression SHR additive_expression {
        $$ = jacc_alloc_binop_node(SHR, $1, $3);
    }
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression {
        $$ = jacc_alloc_binop_node('<', $1, $3);
    }
    | relational_expression '>' shift_expression {
        $$ = jacc_alloc_binop_node('>', $1, $3);
    }
    | relational_expression LTEQ shift_expression {
        $$ = jacc_alloc_binop_node(LTEQ, $1, $3);
    }
    | relational_expression GTEQ shift_expression {
        $$ = jacc_alloc_binop_node(GTEQ, $1, $3);
    }
    ;

equality_expression
    : relational_expression
    | equality_expression EQEQ relational_expression {
        $$ = jacc_alloc_binop_node(EQEQ, $1, $3);
    }
    | equality_expression NOTEQ relational_expression {
        $$ = jacc_alloc_binop_node(NOTEQ, $1, $3);
    }
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression  {
        $$ = jacc_alloc_binop_node('&', $1, $3);
    }
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression {
        $$ = jacc_alloc_binop_node('^', $1, $3);
    }
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression {
        $$ = jacc_alloc_binop_node('|', $1, $3);
    }
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression LOGAND inclusive_or_expression  {
        $$ = jacc_alloc_binop_node(LOGAND, $1, $3);
    }
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOGOR logical_and_expression {
        $$ = jacc_alloc_binop_node(LOGOR, $1, $3);
    }
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression {
        $$ = jacc_alloc_conditional_node($1, $3, $5);
    }
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression {
        $$ = jacc_alloc_binop_node(1, $1, $3);
    }
    ;

assignment_operator
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
    | expression ',' assignment_expression {
        $$ = jacc_alloc_binop_node(',', $1, $3);
    }
    ;

statement
    : expression ';'  {
        $$ = $1;
        final_ast = *$1;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(void) {
    printf("Enter an expression: ");
    fprintf(stderr, "Did I fail?: %d\n", yyparse());
    printf("Let's see!\n");
    return 0;
}
