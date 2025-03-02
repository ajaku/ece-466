#include "ast.h"
#include "types.h"

jacc_ast_node_t* jacc_alloc_binop_node(union jacc_yystype *yytok, jacc_ast_node_t *left, jacc_ast_node_t *right) {
    jacc_ast_node_t* node = malloc(sizeof(jacc_ast_node_t));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate node\n");
    }

    node->type = JACC_BINOP_NODE;

    node->binop.op = yytok->token.data.string_literal;
    node->binop.left = left;
    node->binop.right = right;

    return node;
}

jacc_ast_node_t* jacc_alloc_num_node(union jacc_yystype *yytok) {
    jacc_ast_node_t* node = malloc(sizeof(jacc_ast_node_t));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate node\n");
    }

    node->type = JACC_NUM_NODE;
    node->num.token = yytok->token;

    return node;
}

jacc_ast_node_t* jacc_alloc_ident_node(union jacc_yystype *yytok) {
    jacc_ast_node_t* node = malloc(sizeof(jacc_ast_node_t));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate node\n");
    }

    node->type = JACC_IDENT_NODE;
    node->ident.token = yytok->token;

    return node;
}