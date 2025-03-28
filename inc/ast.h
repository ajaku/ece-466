#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_type {
    JACC_IDENT_AST,         // Base Operators
    JACC_CONST_AST,         // Base Operators
    JACC_STRING_AST,        // Base Operators
    JACC_CHAR_AST,          // Base Operators
    JACC_FUNC_AST,
    JACC_ARGS_AST,
    JACC_UN_OP_AST,
    JACC_BIN_OP_AST,
    JACC_TERNARY_AST,
    JACC_STG_CLS_AST
} jacc_ast_type_t;

typedef enum jacc_un_op_type {
    JACC_UN_OP_STD,
    JACC_UN_OP_DEREF,
    JACC_UN_OP_SIZEOF,
    JACC_UN_OP_ADDROF,
    JACC_UN_OP_POSTINC,
    JACC_UN_OP_POSTSUB
} jacc_un_op_type_t;

typedef enum jacc_bin_op_type {
    JACC_BIN_OP_STD,
    JACC_BIN_OP_ASSIGN,
    JACC_BIN_OP_ASSIGN_COMP,
    JACC_BIN_OP_COMPARE,
    JACC_BIN_OP_LOGICAL,
    JACC_BIN_OP_SELECT,
    JACC_BIN_OP_SELECT_INDIR
} jacc_bin_op_type_t;

typedef enum jacc_stg_cls_type {
    JACC_STG_CLS_TYPEDEF,
    JACC_STG_CLS_EXTERN,
    JACC_STG_CLS_STATIC,
    JACC_STG_CLS_AUTO,
    JACC_STG_CLS_REGISTER
} jacc_stg_cls_type_t;

typedef enum jacc_type_spec_type {
    JACC_TYPE_SPEC_VOID,
    JACC_TYPE_SPEC_CHAR,
    JACC_TYPE_SPEC_SHORT,
    JACC_TYPE_SPEC_INT,
    JACC_TYPE_SPEC_LONG,
    JACC_TYPE_SPEC_FLOAT,
    JACC_TYPE_SPEC_DOUBLE,
    JACC_TYPE_SPEC_SIGNED,
    JACC_TYPE_SPEC_UNSIGNED,
    JACC_TYPE_SPEC__BOOL,
    JACC_TYPE_SPEC__COMPLEX,
    JACC_TYPE_SPEC_STRUCT_OR_UNION,
    JACC_TYPE_SPEC_ENUM
} jacc_type_spec_type_t;

typedef enum jacc_type_qual_type {
    JACC_TYPE_QUAL_CONST,
    JACC_TYPE_QUAL_RESTRICT,
    JACC_TYPE_QUAL_VOLATILE
} jacc_type_qual_type_t;

typedef struct jacc_func_ast {
    struct jacc_ast_node *func;
    struct jacc_ast_node *args;
} jacc_func_ast_t;

typedef struct jacc_args_ast {
    int n_args;
    struct jacc_ast_node *arg;
    struct jacc_ast_node *next_arg;
} jacc_args_ast_t;

typedef struct jacc_un_op_ast {
    jacc_un_op_type_t u_type;
    int operator;
    struct jacc_ast_node *operand;
} jacc_un_op_ast_t;

typedef struct jacc_bin_op_ast {
    jacc_bin_op_type_t b_type;
    int operator;
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_bin_op_ast_t;

typedef struct jacc_ternary_ast {
    struct jacc_ast_node *cond;
    struct jacc_ast_node *t_op;
    struct jacc_ast_node *f_op;
} jacc_ternary_ast_t;

typedef struct jacc_stg_cls_spec_ast {
    jacc_stg_cls_type_t type;
} jacc_stg_cls_spec_ast_t;

typedef struct jacc_type_spec_ast {
    jacc_type_spec_type_t type;
} jacc_type_spec_ast_t;

typedef struct jacc_type_qual_ast {
    jacc_type_qual_type_t type;
} jacc_type_qual_ast_t;

typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;

    union {
        jacc_lex_tok_t          lex;
        jacc_func_ast_t         func;
        jacc_args_ast_t         args;
        jacc_un_op_ast_t        unop;
        jacc_bin_op_ast_t       binop;
        jacc_ternary_ast_t      tern;
        jacc_stg_cls_spec_ast_t stg_cls_spec;
        jacc_type_spec_ast_t    type_spec;
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base(jacc_ast_type_t type, jacc_lex_tok_t lex);
jacc_ast_node_t* jacc_alloc_func(jacc_ast_node_t *func, jacc_ast_node_t *args);
jacc_ast_node_t* jacc_alloc_args(jacc_ast_node_t *arg);
jacc_ast_node_t* jacc_alloc_unop(jacc_un_op_type_t u_type, int operator, jacc_ast_node_t *operand);
jacc_ast_node_t* jacc_alloc_binop(jacc_bin_op_type_t b_type, int operator, jacc_ast_node_t *left, jacc_ast_node_t *right);
jacc_ast_node_t* jacc_alloc_ternary(jacc_ast_node_t *cond, jacc_ast_node_t *t_op, jacc_ast_node_t *f_op);

jacc_ast_node_t* jacc_append_arg(jacc_ast_node_t *arg_list, jacc_ast_node_t *next_arg);

void jacc_print_ast(int *ident, jacc_ast_node_t *ast);
