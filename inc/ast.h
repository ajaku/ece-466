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
    // JACC_ARR_AST, gets replaced by DEREF
    JACC_FCALL_AST,
    JACC_SEL_AST,
    JACC_DEREF_AST,
    JACC_POST_OP_AST,
    JACC_PRE_OP_AST,
    JACC_CAST_AST,
    JACC_SIZEOF_AST,
    JACC_CONDITIONAL_AST
} jacc_ast_type_t;
typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;

    struct jacc_lex_tok *lex_ptr;

    union {
        struct {
            int op;
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } binop;
        struct {
            struct jacc_ast_node *operand;
            struct jacc_ast_node *operator;
        } generic;
        struct {
            struct jacc_ast_node *condition;
            struct jacc_ast_node *true_op;
            struct jacc_ast_node *false_op;
        } conditional;
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type, jacc_lex_tok_t *tok);
jacc_ast_node_t* jacc_alloc_binop_node(int op, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_generic_node(jacc_ast_type_t type, jacc_ast_node_t *operand, jacc_ast_node_t *operator);
jacc_ast_node_t* jacc_alloc_conditional_node(jacc_ast_node_t *condition, jacc_ast_node_t *true_op, jacc_ast_node_t *false_op);