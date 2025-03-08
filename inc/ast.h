#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_type {
    JACC_IDENT_AST,
    JACC_CONST_AST,
    JACC_STRING_AST,
    JACC_CHAR_AST,
    JACC_BINOP_AST,
    JACC_LOG_OP_AST,
    JACC_COMPARE_AST,
    // JACC_ARR_AST, gets replaced by DEREF
    JACC_FUNC_AST,
    JACC_ARG_AST,
    JACC_SEL_AST,
    JACC_INDIR_SEL_AST,
    JACC_DEREF_AST,
    JACC_POSTINC_OP_AST,
    JACC_POSTSUB_OP_AST,
    JACC_ASSIGN_COMP_AST,
    JACC_ADDR_OF_AST,
    JACC_UNARY_OP_AST,
    JACC_CAST_AST,
    JACC_SIZEOF_AST,
    JACC_TERNARY_AST
} jacc_ast_type_t;


typedef enum jacc_union_type {
    JACC_NO_TYPE_U,
    JACC_OPERAND_U,
    JACC_ATTR_OPERAND_U,
    JACC_OPERAND_OPERATOR_U,
    JACC_ATTR_OPERAND_OPERATOR_U,
    JACC_ARG_U,
    JACC_TERNAY_U
} jacc_union_type_t;

typedef struct {
    struct jacc_ast_node *operand;
} operand_ast_t ;

typedef struct {
    char *attr;
    struct jacc_ast_node *operand;
} attr_operand_ast_t;
typedef struct {
    struct jacc_ast_node *operand;
    struct jacc_ast_node *operator;
} operand_operator_ast_t;

typedef struct {
    char *attr;
    struct jacc_ast_node *operand;
    struct jacc_ast_node *operator;
} attr_operand_operator_ast_t;

typedef struct {
    int n_arg;
    struct jacc_ast_node *arg;
    struct jacc_ast_node *next_arg;
} arg_ast_t ;

typedef struct {
    struct jacc_ast_node *cond;
    struct jacc_ast_node *t_op;
    struct jacc_ast_node *f_op;
} ternary_ast_t ;

typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;
    jacc_union_type_t union_type;

    struct jacc_lex_tok lex;

    union {
        operand_ast_t operand_ast;
        attr_operand_ast_t attr_operand_ast;
        operand_operator_ast_t operand_operator_ast;
        attr_operand_operator_ast_t attr_operand_operator_ast;
        arg_ast_t arg_ast;
        ternary_ast_t ternary_ast;
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type,
                                      jacc_lex_tok_t lex);

jacc_ast_node_t *jacc_alloc_operand_node(jacc_ast_type_t type,
                                        jacc_ast_node_t *operand);

jacc_ast_node_t *jacc_alloc_attr_operand_node(jacc_ast_type_t type,
                                             char *attr,
                                             jacc_ast_node_t *operand);

jacc_ast_node_t *jacc_alloc_operand_operator_node(jacc_ast_type_t type,
                                                 jacc_ast_node_t *operand,
                                                 jacc_ast_node_t *operator);

jacc_ast_node_t *jacc_alloc_attr_operand_operator_node(jacc_ast_type_t type,
                                                      char *attr,
                                                      jacc_ast_node_t *operand,
                                                      jacc_ast_node_t *operator);

jacc_ast_node_t *jacc_alloc_arg_node(jacc_ast_type_t type,
                                    jacc_ast_node_t *arg);


jacc_ast_node_t* jacc_alloc_ternary_node(jacc_ast_type_t type,
                                         jacc_ast_node_t *cond, 
                                         jacc_ast_node_t *t_op,
                                         jacc_ast_node_t *f_op);

jacc_ast_node_t* jacc_append_arg_node(jacc_ast_node_t *arg_list,   
                                      jacc_ast_node_t *next_arg);

void jacc_print_ast(int *ident, jacc_ast_node_t *ast);
