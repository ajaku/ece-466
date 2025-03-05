#include "ast.h"
#include "types.h"

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type, jacc_lex_tok_t *lex_tok) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));
    node->lex_ptr = lex_tok;

    switch (type) {
        case JACC_IDENT_AST:
            node->ast_type = JACC_IDENT_AST;
            break;

        case JACC_NUM_AST:
            node->ast_type = JACC_NUM_AST;
            break;

        case JACC_STRING_AST:
            node->ast_type = JACC_STRING_AST;
            break;

        case JACC_CHAR_AST:
            node->ast_type = JACC_CHAR_AST;
            break;

        default:
            exit(1);
    };

    return node;
}

jacc_ast_node_t* jacc_alloc_generic_node(jacc_ast_type_t type, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    switch (type) {
        case JACC_FCALL_AST:
            node->ast_type = JACC_FCALL_AST;
            node->generic.operand = operand;
            node->generic.operator = operator;
            break;

        case JACC_SEL_AST:
            node->ast_type = JACC_SEL_AST;
            node->generic.operand = operand;
            node->generic.operator = operator;
            break;

        case JACC_DEREF_AST:
            node->ast_type = JACC_DEREF_AST;
            node->generic.operand = operand;
            break;

        default:
            exit(1);

    }

    return node;
}

jacc_ast_node_t* jacc_alloc_binop_node(int op, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_BINOP_AST;
    node->binop.op = op;
    node->binop.operand = operand;
    node->binop.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_conditional_node(jacc_ast_node_t *condition, jacc_ast_node_t *true_op, jacc_ast_node_t *false_op) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_CONDITIONAL_AST;
    node->conditional.condition = condition;
    node->conditional.true_op = true_op;
    node->conditional.false_op = false_op;

    return node;
}

void print_ast(jacc_ast_node_t ast) {
    int parsed = 0;

    jacc_ast_node_t *iter = &ast;
    while (!parsed) {
        switch (iter->ast_type) {
            case JACC_IDENT_AST:
                break;
            case JACC_NUM_AST:
                break;
            case JACC_STRING_AST:
                break;
            case JACC_CHAR_AST:
                break;
            case JACC_BINOP_AST:
                break;
            case JACC_FCALL_AST:
                break;
            case JACC_SEL_AST:
                break;
            case JACC_DEREF_AST:
                break;
            case JACC_POST_OP_AST:
                break;
            case JACC_PRE_OP_AST:
                break;
            case JACC_CAST_AST:
                break;
            case JACC_SIZEOF_AST:
                break;
            case JACC_CONDITIONAL_AS:
                break;
            default:
                exit(1);
        }
    }
}
