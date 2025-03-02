#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_node_type {
    JACC_BINOP_NODE,
    JACC_NUM_NODE,
    JACC_IDENT_NODE
} jacc_ast_node_type_t;

typedef struct jacc_ast_binop {
    char *op;
    struct jacc_ast_node *left, *right;
} jacc_ast_binop_t;

typedef struct jacc_ast_num {
   struct jacc_yystruct token;
} jacc_ast_num_t;

typedef struct jacc_ast_ident {
    struct jacc_yystruct token;
} jacc_ast_ident_t;

typedef struct jacc_ast_node {
    int type;

    union {
        jacc_ast_binop_t binop;    
        jacc_ast_num_t num;    
        jacc_ast_ident_t ident;    
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_binop_node(union jacc_yystype *yytok, jacc_ast_node_t *left, jacc_ast_node_t *right);
jacc_ast_node_t* jacc_alloc_num_node(union jacc_yystype *yytok);
jacc_ast_node_t* jacc_alloc_ident_node(union jacc_yystype *yytok);
