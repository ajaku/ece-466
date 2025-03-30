%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

void yyerror(const char * s);
int yylex(void);

extern FILE *yyin;

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
%token <tok> ';'
%token <tok> '='
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
%type <ast_p> declaration
%type <ast_p> declaration_specifiers 
//%type <ast_p> init_declarator_list
%type <ast_p> storage_class_specificer
%type <ast_p> type_specifier
%type <ast_p> type_qualifier
// %type <ast_p> function_specifier
//%type <ast_p> init_declarator
// %type <ast_p> initializer
%type <ast_p> struct_or_union_specifier
// %type <ast_p> enum_specifier
%type <tok> struct_or_union
%type <ast_p> struct_declaration
%type <ast_p> struct_declaration_list
%type <ast_p> specifier_qualifier_list
// %type <ast_p> struct_declarator_list
// %type <ast_p> struct_declarator
%type <ast_p> declarator
// %type <ast_p> enumerator_list
// %type <ast_p> enumeration_constant
%type <ast_p> pointer
%type <ast_p> direct_declarator
%type <ast_p> type_qualifier_list
// %type <ast_p> parameter_type_list
// %type <ast_p> parameter_list
// %type <ast_p> parameter_declaration
// %type <ast_p> identifier_list
// %type <ast_p> type_name
//%type <ast_p> abstract_declarator
//%type <ast_p> direct_abstract_declarator
// %type <ast_p> initializer_list
// %type <ast_p> designation
// %type <ast_p> designator_list
%type <ast_p> statement

// %start statement
//%start  declaration
%start type_qualifier_list

%%

primary_expression
    : IDENT {
        $$ = jacc_alloc_base(JACC_IDENT_AST, $1);
    }   
    | NUMBER {
        $$ = jacc_alloc_base(JACC_CONST_AST, $1);
    }
    | STRING {
        $$ = jacc_alloc_base(JACC_STRING_AST, $1);
    }
    | CHARLIT {
        $$ = jacc_alloc_base(JACC_CHAR_AST, $1);
    }
    | '(' expression ')' { $$ = $2; }
    ;

postfix_expression
    : primary_expression 
    | postfix_expression '[' expression ']' {
        jacc_ast_node_t *binop_ast = jacc_alloc_binop(JACC_BIN_OP_STD, '+', $1, $3);
        $$ = jacc_alloc_unop(JACC_UN_OP_DEREF, '*',  binop_ast);
    } 
    | postfix_expression '(' argument_expression_list ')' {
        $$ = jacc_alloc_func($1, $3);
    }
    | postfix_expression '(' ')' {
        $$ = jacc_alloc_func($1, NULL);
    }
    | postfix_expression '.' IDENT {
        jacc_ast_node_t *ident_ast = jacc_alloc_base(JACC_IDENT_AST, $3);
        $$ = jacc_alloc_binop(JACC_BIN_OP_SELECT, '.', $1, ident_ast);
    }
    | postfix_expression INDSEL IDENT {
        jacc_ast_node_t *ident_ast = jacc_alloc_base(JACC_IDENT_AST, $3);
        jacc_ast_node_t *indsel_ast = jacc_alloc_unop(JACC_UN_OP_DEREF, '*', $1);
        $$ = jacc_alloc_binop(JACC_BIN_OP_SELECT_INDIR, INDSEL, indsel_ast, ident_ast);
    }
    | postfix_expression PLUSPLUS {

        $$ = jacc_alloc_unop(JACC_UN_OP_POSTINC, '+', $1);
    }
    | postfix_expression MINUSMINUS {
        $$ = jacc_alloc_unop(JACC_UN_OP_POSTSUB, '-', $1);
    }
    // | '(' type_name ')' '{' initializer_list '}'
    // | '(' type_name ')' '{' initializer_list ',' '}'
    ;

argument_expression_list
    : assignment_expression {
        $$ = jacc_alloc_args($1);
    }
    | argument_expression_list ',' assignment_expression {
        $$ = jacc_append_arg($1, $3);
    }
    ;

unary_expression
    : postfix_expression
    | PLUSPLUS unary_expression {
        jacc_ast_node_t *one = jacc_alloc_base(JACC_CONST_AST, lex_alloc_int(1));
        $$ = jacc_alloc_binop(JACC_BIN_OP_ASSIGN_COMP, '+', $2, one);
    }
    | MINUSMINUS unary_expression {
        jacc_ast_node_t *one = jacc_alloc_base(JACC_CONST_AST, lex_alloc_int(1));
        $$ = jacc_alloc_binop(JACC_BIN_OP_ASSIGN_COMP, '-', $2, one);
    }
    | unary_operator cast_expression {
        if ($1.data.char_d == '&') {
            $$ = jacc_alloc_unop(JACC_UN_OP_ADDROF, $1.data.int_d, $2);
        } else if ($1.data.char_d == '*') {
            $$ = jacc_alloc_unop(JACC_UN_OP_DEREF, $1.data.int_d, $2);
        } else {
            $$ = jacc_alloc_unop(JACC_UN_OP_STD, $1.data.int_d, $2);
        }
    }
    | SIZEOF unary_expression {
        $$ = jacc_alloc_unop(JACC_UN_OP_SIZEOF, SIZEOF, $2);
    }
    // Double check this
    //| SIZEOF '(' type_name ')' {
    //    $$ = jacc_alloc_unop(JACC_UN_OP_SIZEOF, SIZEOF, $3);
    //}
    ;

unary_operator
    : '&' { $$ = lex_alloc_int('&'); }
    | '*' { $$ = lex_alloc_int('*'); }
    | '+' { $$ = lex_alloc_int('+'); }
    | '-' { $$ = lex_alloc_int('-'); }
    | '~' { $$ = lex_alloc_int('~'); }
    | '!' { $$ = lex_alloc_int('!'); }
    ;

cast_expression
    : unary_expression
    // Placeholder
    //| '(' type_name ')' cast_expression { $$ = $2; }
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '*', $1, $3);
    }
    | multiplicative_expression '/' cast_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '/', $1, $3);
    }
    | multiplicative_expression '%' cast_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '%', $1, $3);
    }
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '+', $1, $3);
    }
    | additive_expression '-' multiplicative_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '-', $1, $3);
    }
    ;

shift_expression
    : additive_expression
    | shift_expression SHL additive_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, SHL, $1, $3);
    }
    | shift_expression SHR additive_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, SHR, $1, $3);
    }
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '<', $1, $3);
    }
    | relational_expression '>' shift_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '>', $1, $3);
    }
    | relational_expression LTEQ shift_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_COMPARE, LTEQ, $1, $3);
    }
    | relational_expression GTEQ shift_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_COMPARE, GTEQ, $1, $3);
    }
    ;

equality_expression
    : relational_expression
    | equality_expression EQEQ relational_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_COMPARE, EQEQ, $1, $3);
    }
    | equality_expression NOTEQ relational_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_COMPARE, NOTEQ, $1, $3);
    }
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression  {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '&', $1, $3);
    }
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '^', $1, $3);
    }
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, '|', $1, $3);
    }
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression LOGAND inclusive_or_expression  {
        $$ = jacc_alloc_binop(JACC_BIN_OP_LOGICAL, LOGAND, $1, $3);
    }
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOGOR logical_and_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_LOGICAL, LOGOR, $1, $3);
    }
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression {
        $$ = jacc_alloc_ternary($1, $3, $5);
    }
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression {
        if ($2.data.char_d == '=') {
            $$ = jacc_alloc_binop(JACC_BIN_OP_ASSIGN, $2.data.int_d, $1, $3);
        } else {
            $$ = jacc_alloc_binop(JACC_BIN_OP_ASSIGN_COMP, $2.data.int_d, $1, $3);
        }
    }
    ;

assignment_operator
    : '='       { $$ = lex_alloc_int('=');      }
    | TIMESEQ   { $$ = lex_alloc_int(TIMESEQ);  }
    | DIVEQ     { $$ = lex_alloc_int(DIVEQ);    }
    | MODEQ     { $$ = lex_alloc_int(MODEQ);    }
    | PLUSEQ    { $$ = lex_alloc_int(PLUSEQ);   }
    | MINUSEQ   { $$ = lex_alloc_int(MINUSEQ);  }
    | SHLEQ     { $$ = lex_alloc_int(SHLEQ);    }
    | SHREQ     { $$ = lex_alloc_int(SHREQ);    }
    | ANDEQ     { $$ = lex_alloc_int(ANDEQ);    }
    | XOREQ     { $$ = lex_alloc_int(XOREQ);    }
    | OREQ      { $$ = lex_alloc_int(OREQ);     }
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression {
        $$ = jacc_alloc_binop(JACC_BIN_OP_STD, ',', $1, $3);
    }
    ;

constant_expression
    : conditional_expression
    ;

statement
    : expression ';' {
        int indent = -1;
        jacc_print_ast(&indent, $1);
    }
    | statement expression ';' {
        int indent = -1;
        jacc_print_ast(&indent, $2);
    }
    ;

// declarations are the types / stuff before the variable
declaration
    // : declaration_specifiers init_declarator_list
    : declaration_specifiers {
        $$ = $1; 
        jacc_ast_node_t *temp = $$;
        printf("Created a declaration\n");
    }
    ;

declaration_specifiers
    : storage_class_specificer declaration_specifiers {
        $$ = jacc_alloc_declaration_spec($1, $2, NULL);
    }
    | storage_class_specificer {
        $$ = jacc_alloc_declaration_spec($1, NULL, NULL);
    }
    | type_specifier declaration_specifiers {
        $$ = jacc_alloc_declaration_spec(NULL, $2, $1);
    }
    | type_specifier {
        $$ = jacc_alloc_declaration_spec(NULL, NULL, $1);
    }
    // | function_specifier declaration_specifiers
    // | function_specifier 
    ;

/*
init_declarator_list
    : init_declarator
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : declarator
    //| declarator '=' initalizer
    ;
*/

storage_class_specificer
    : TYPEDEF   { $$ = jacc_alloc_stg_cls_spec(JACC_STG_CLS_TYPEDEF);   }
    | EXTERN    { $$ = jacc_alloc_stg_cls_spec(JACC_STG_CLS_EXTERN);    }
    | STATIC    { $$ = jacc_alloc_stg_cls_spec(JACC_STG_CLS_STATIC);    }
    | AUTO      { $$ = jacc_alloc_stg_cls_spec(JACC_STG_CLS_AUTO);      }
    | REGISTER  { $$ = jacc_alloc_stg_cls_spec(JACC_STG_CLS_REGISTER);  }
    ;

type_specifier
    : VOID      { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_VOID, NULL);       }
    | CHAR      { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_CHAR, NULL);       }
    | SHORT     { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_SHORT, NULL);      }
    | INT       { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_INT, NULL);        }
    | LONG      { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_LONG, NULL);       }
    | FLOAT     { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_FLOAT, NULL);      }
    | DOUBLE    { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_DOUBLE, NULL);     }
    | SIGNED    { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_SIGNED, NULL);     }
    | UNSIGNED  { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_UNSIGNED, NULL);   }
    | _BOOL     { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC__BOOL, NULL);      }
    | _COMPLEX  { $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC__COMPLEX, NULL);   }
    | struct_or_union_specifier {
        $$ = jacc_alloc_type_spec(JACC_TYPE_SPEC_STRUCT_OR_UNION, $1);
    }
    //| enum_specifier
    // | typedef-name
    ;

struct_or_union_specifier
    : struct_or_union IDENT '{' struct_declaration_list '}' {
        jacc_lex_tok_t struct_or_union_tok = $1;
        jacc_struct_or_union_spec_type_t type = JACC_STRUCT_SPEC;
        if (!strcmp(struct_or_union_tok.data.string_d, "UNION")) {
            type = JACC_UNION_SPEC;
        }
        jacc_ast_node_t *ident = jacc_alloc_base(JACC_IDENT_AST, $1);

        $$ = jacc_alloc_struct_or_union_spec(s_or_u, ident, $4); 
    }
    | struct_or_union '{' struct_declaration_list '}' {
        jacc_lex_tok_t struct_or_union_tok = $1;
        jacc_struct_or_union_spec_type_t type = JACC_STRUCT_SPEC;
        if (!strcmp(struct_or_union_tok.data.string_d, "UNION")) {
            type = JACC_UNION_SPEC;
        }

        $$ = jacc_alloc_struct_or_union_spec(type, NULL, $3); 
    }
    | struct_or_union IDENT {
        jacc_lex_tok_t struct_or_union_tok = $1;
        jacc_struct_or_union_spec_type_t type = JACC_STRUCT_SPEC;
        if (!strcmp(struct_or_union_tok.data.string_d, "UNION")) {
            type = JACC_UNION_SPEC;
        }

        jacc_ast_node_t *ident = jacc_alloc_base(JACC_IDENT_AST, $1);
        $$ = jacc_alloc_struct_or_union_spec(type, ident, NULL); 
    }
    ;

struct_or_union
    : STRUCT
    | UNION
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list struct_declaration_list ';'
    ;

// order of specifiers gets flipped
specifier_qualifier_list
    : type_specifier specifier_qualifier_list {
        $$ = jacc_append_spec_qual_list($2, $1, NULL);
    }
    | type_specifier {
        $$ = jacc_alloc_spec_qual_list($1, NULL);
    }
    | type_qualifier specifier_qualifier_list {
        $$ = jacc_append_spec_qual_list($2, NULL, $1);
    }
    | type_qualifier {
        $$ = jacc_alloc_spec_qual_list(NULL, $1);
    }
    ;

/*
struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;

struct_declarator
    : declarator
    | declarator ':' constant_expression
    | ':' constant_expression
    ;

enum_specifier
    : ENUM IDENT '{' enumerator_list '}'
    | ENUM '{' enumerator_list ',' '}'
    | ENUM IDENT
    ;

enumerator_list
    : enumerator
    | enumerator_list ',' enumerator
    ;

enumerator
    : enumeration_constant
    | enumeration_constant '=' constant_expression
    ;

enumeration_constant
    : IDENT
    ;
*/

type_qualifier
    : CONST     { printf("Hi\n"); $$ = jacc_alloc_type_qual(JACC_TYPE_QUAL_CONST);      }
    | RESTRICT  { $$ = jacc_alloc_type_qual(JACC_TYPE_QUAL_RESTRICT);   }
    | VOLATILE  { $$ = jacc_alloc_type_qual(JACC_TYPE_QUAL_VOLATILE);   }
    ;

/*
function_specifier
    : INLINE
    ;
*/

// declarators are the variables / stuff after the declaration
declarator
    : pointer direct_declarator {
        $$ = jacc_alloc_declarator($1, $2);
    }
    | direct_declarator {
        $$ = jacc_alloc_declarator(NULL, $1);
    }
    ;

// Have simple declarators (int a - where a is the declarator)
// Have more complicated ones like (int *x[])
// Going to simplify them for now
direct_declarator
    : IDENT {
        $$ = jacc_alloc_direct_declarator(ident, NULL, NULL, NULL);
    }
    | '{' declarator '}' {
        $$ = jacc_alloc_direct_declarator(NULL, $2, NULL, NULL);
    }
    // | direct_declarator '[' type_qualifier_list assignment_expression']'
    // | direct_declarator '[' assignment_expression']'
    // | direct_declarator '[' type_qualifier_list ']'
    // | direct_declarator '[' ']'
    // | direct_declarator '[' STATIC type_qualifier_list assignment_expression']'
    // | direct_declarator '[' STATIC assignment_expression']'
    // | direct_declarator '[' type_qualifier_list STATIC assignment_expression']'
    // | direct_declarator '[' type_qualifier_list '*' ']'
    // | direct_declarator '[' '*' ']'
    // | direct_declarator '[' parameter_type_list ']'
    // | direct_declarator '[' identifier_list ']'
    | direct_declarator '[' ']' {
        $$ = jacc_alloc_direct_declarator(NULL, NULL, $1, NULL);
    }
    | direct_declarator '[' NUMBER ']' {
        $$ = jacc_alloc_direct_declarator(NULL, NULL, $1, $3);
    }
    ;

pointer
    : '*' type_qualifier_list {
        // $$ = jacc_alloc_pointer($2);
        jacc_ast_node_t *test = $2;
        printf("hi\n");
    }
    | '*' {
        // $$ = jacc_alloc_pointer(NULL);
    }
    | '*' type_qualifier_list pointer {
        // $$ = jacc_append_pointer($2, $3);
        jacc_ast_node_t *test = $2;
        printf("hi\n");
    }
    | '*' pointer { 
        // $$ = jacc_append_pointer(NULL, $3);
        jacc_ast_node_t *test = $2;
        printf("hi\n");
    }
    ;

type_qualifier_list
    : type_qualifier
    | type_qualifier_list type_qualifier {
        jacc_ast_node_t *a1 = $1;
        jacc_ast_node_t *a2 = $2;
        $$ = jacc_append_type_qual_list($1, $2);
        jacc_ast_node_t *test = $$;
        printf("hi\n");
    }
    ;

/*
parameter_type_list
    : parameter_list 
    | parameter_list ',' ELLIPSIS
    ;

parameter_list
    : parameter_declaration
    | parameter_list ',' parameter_declaration
    ;

parameter_declaration
    : declaration_specifiers declarator
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;

identifier_list
    : IDENT
    | identifier_list ',' IDENT
    ;

/*
type_name
    : specifier_qualifier_list abstract_declarator
    | specifier_qualifier_list
    ;
*/

/*
abstract_declarator
    : pointer
    | pointer direct_abstract_declarator
    | direct_abstract_declarator
    ;
*/

// Same as normal declarators, except don't have ident
// Ex: (int **[])
/*
direct_abstract_declarator
    : '(' abstract_declarator ')'
    | direct_abstract_declarator '[' assignment_expression ']'
    | direct_abstract_declarator '[' ']'
    | '[' assignment_expression ']'
    | '[' ']'
    | direct_abstract_declarator '[' '*' ']'
    | '[' '*' ']'
    | direct_abstract_declarator '(' parameter_type_list ')'
    | direct_abstract_declarator '(' ')' 
    | '(' parameter_type_list ')'
    | '(' ')'
    ;
*/

//typedef_name
//  : IDENT

/*
initializer
    : assignment_expression
    | '{' initializer_list '}'
    | '{' initializer_list ',' '}'
    ;

initializer_list
    : designation initializer
    | initializer
    | initializer_list ',' designation initializer
    | initializer_list ',' initializer
    ;

designation
    : designator_list '='
    ;

designator_list
    : designator
    | designator_list designator
    ;

designator
    : '[' constant_expression ']'
    | '.' IDENT
    ;
*/


%%

void yyerror(const char *s) {
    fprintf(stderr, "HELP Error: %s\n", s);
}

int main(int argc, char **argv) {
    FILE *file;
    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Failed to open file");
            return 1;
        }
        yyin = file;
    } else {
        yyin = stdin;
    }


    fprintf(stderr, "Parser fail status: %d\n", yyparse());

    if (yyin != stdin) {
        fclose(file);
    }
    return 0;
}
