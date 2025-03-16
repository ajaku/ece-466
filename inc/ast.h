#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_type {
    JACC_BASE_AST,
    /*
    JACC_IDENT_AST,         // Base Operators
    JACC_CONST_AST,         // Base Operators
    JACC_STRING_AST,        // Base Operators
    JACC_CHAR_AST,          // Base Operators
    */
    JACC_FUNC_AST,
    JACC_ARGS_AST,
    JACC_UN_OP_AST,         // Unary Operators
    /*
    JACC_DEREF_AST,         // Unary Operators
    JACC_SIZEOF_AST,        // Unary Operators
    JACC_ADDROF_AST,        // Unary Operators
    JACC_POST_INC_AST,      // Unary Operators
    JACC_POST_SUB_AST,      // Unary Operators
    */
    JACC_BIN_OP_AST,        // Binary Operators
    /*
    JACC_ASSIGN_AST,        // Binary Operators
    JACC_ASSIGN_COMP_AST,   // Binary Operators
    JACC_CMPRE_OP_AST,      // Binary Operators
    JACC_LOG_OP_AST,        // Binary Operators
    JACC_SEL_AST,           // Binary Operators
    JACC_INDIR_SEL_AST,     // Binary Operators
    */
    JACC_TERNARY_AST
} jacc_ast_type_t;

typedef enum jacc_base_type {
    JACC_BASE_IDENT,
    JACC_BASE_CONST,
    JACC_BASE_STRING,
    JACC_BASE_CHAR
} jacc_base_type_t;

typedef enum jacc_un_op_type {
    JACC_UN_OP_STD,
    JACC_UN_OP_DEREF,
    JACC_UN_OP_SIZEOF,
    JACC_UN_OP_ADDROF,
    JACC_UN_OP_POSTINC,
    JACC_UN_OP_POSTSUB
} jacc_un_op_type_t;

typedef enum jacc_bin_op_ast_type {
    JACC_BIN_OP_STD,
    JACC_BIN_OP_ASSIGN,
    JACC_BIN_OP_ASSIGN_COMP,
    JACC_BIN_OP_COMPARE,
    JACC_BIN_OP_LOGICAL,
    JACC_BIN_OP_SELECT,
    JACC_BIN_OP_SELECT_INDIR
} jacc_bin_op_type_t;

typedef struct jacc_func_ast {
    struct jacc_ast_node *func;
    struct jacc_ast_node *args;
} jacc_func_ast_t;

typedef struct jacc_args_ast {
    int n_args;
    struct jacc_ast_node *arg;
    struct jacc_ast_node *next_arg;
} jacc_args_ast_t;

/*

typedef struct jacc_deref_ast {
    struct jacc_ast_node *operand;
} jacc_deref_ast_t;

typedef struct jacc_sizeof_ast {
    struct jacc_ast_node *operand;
} jacc_sizeof_ast_t;

typedef struct jacc_assign_ast {
    struct jacc_ast_node *operand;
} jacc_assign_ast_t;

typedef struct jacc_addrof_ast {
    struct jacc_ast_node *operand;
} jacc_addrof_ast_t;

typedef struct jacc_unary_op_ast {
    int operator;
    struct jacc_ast_node *operand;
} jacc_unary_op_ast_t;

typedef struct jacc_assign_comp_ast {
    int operator;
    struct jacc_ast_node *operand;
} jacc_assign_comp_ast_t;
*/

typedef struct jacc_unary_op_ast {
    jacc_un_op_type_t u_type;
    int operator;
    struct jacc_ast_node *operand;
} jacc_unary_op_ast_t;


typedef struct jacc_binary_op_ast {
    jacc_bin_op_type_t b_type;
    int operator;
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_binary_op_ast_t;

/*
typedef struct jacc_cmpre_op_ast {
    int operator;
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_cmpre_op_ast_t;

typedef struct jacc_log_op_ast {
    int operator;
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_log_op_ast_t;

typedef struct jacc_sel_ast {
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_sel_ast_t;

typedef struct jacc_indir_sel_ast {
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_indir_sel_ast_t;
*/

typedef struct jacc_ternary_ast {
    struct jacc_ast_node *cond;
    struct jacc_ast_node *t_op;
    struct jacc_ast_node *f_op;
} jacc_ternary_ast_t;


typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;

    union {
        jacc_lex_tok_t          lex;
        jacc_func_ast_t         func;
        jacc_args_ast_t         args;
        jacc_unary_op_ast_t     unop;
        jacc_binary_op_ast_t    binop;
        jacc_ternary_ast_t      tern;
    };

    //void (*print_ast)(int *, struct jacc_ast_node_t*);

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base(jacc_lex_tok_t lex);
jacc_ast_node_t* jacc_alloc_func(jacc_ast_node_t *func, jacc_ast_node_t *args);
jacc_ast_node_t* jacc_alloc_args(jacc_ast_node_t *arg);
jacc_ast_node_t* jacc_alloc_unop(jacc_un_op_type_t u_type, int operator, jacc_ast_node_t *operand);
jacc_ast_node_t* jacc_alloc_binop(jacc_bin_op_type_t b_type, int operator, jacc_ast_node_t *left, jacc_ast_node_t *right);
jacc_ast_node_t* jacc_alloc_ternary(jacc_ast_node_t *cond, jacc_ast_node_t *t_op, jacc_ast_node_t *f_op);

jacc_ast_node_t* jacc_append_arg(jacc_ast_node_t *arg_list, jacc_ast_node_t *next_arg);

void jacc_print_ast(int *ident, jacc_ast_node_t *ast);
