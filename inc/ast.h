#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_node_type {
    JACC_IDENT_AST,
    JACC_NUM_AST,
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
    JACC_TERNARY_AST,
    JACC_CONDITIONAL_AST
} jacc_ast_type_t;

typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;

    struct jacc_lex_tok lex;

    union {
        struct {
            char *op;
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } binop;

        struct {
            char *op;
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } logical;

        struct {
            char *op;
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } compare;
        struct {
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } generic;

        struct {
            struct jacc_ast_node *operand;
        } addr_of;

        struct {
            char *op;
            struct jacc_ast_node *operand;
        } unary_op;

        struct {
            struct jacc_ast_node *operand;
        } sizeof_op;

        struct {
            char *cmpd;
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } asgn_cmpd;
        struct {
            int n_args;
            struct jacc_ast_node *arg;
            struct jacc_ast_node *next_arg;
        } argument;
        struct {
            struct jacc_ast_node *condition;
            struct jacc_ast_node *true_op;
            struct jacc_ast_node *false_op;
        } conditional;
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type, jacc_lex_tok_t tok);
jacc_ast_node_t* jacc_alloc_binop_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_logical_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_compare_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_generic_node(jacc_ast_type_t type, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_asgn_cmpd_node(char *cmpd, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_arg_node(jacc_ast_node_t *arg);
jacc_ast_node_t* jacc_alloc_unary_op_node(jacc_lex_tok_t tok, jacc_ast_node_t *operand);
jacc_ast_node_t* jacc_alloc_addr_of_node(jacc_ast_node_t *operand);
jacc_ast_node_t* jacc_alloc_sizeof_node(jacc_ast_node_t *operand);
jacc_ast_node_t* jacc_alloc_ternary_node(jacc_ast_node_t *condition, jacc_ast_node_t *true_op, jacc_ast_node_t *false_op);

jacc_ast_node_t* jacc_append_arg_node(jacc_ast_node_t *arg_list, jacc_ast_node_t *next_arg);

void print_ast(int *rec_lvl, jacc_ast_node_t *ast);