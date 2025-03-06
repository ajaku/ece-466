#include "ast.h"
#include "types.h"

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type, jacc_lex_tok_t lex_tok) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));
    node->lex = lex_tok;

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
        case JACC_FUNC_AST:
            node->ast_type = JACC_FUNC_AST;
            node->generic.operand = operand;
            node->generic.operator = operator;
            break;

        case JACC_SEL_AST:
            node->ast_type = JACC_SEL_AST;
            node->generic.operand = operand;
            node->generic.operator = operator;
            break;

        case JACC_INDIR_SEL_AST:
            node->ast_type = JACC_INDIR_SEL_AST;
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

jacc_ast_node_t* jacc_alloc_binop_node(char op, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_BINOP_AST;
    node->binop.op = op;
    node->binop.operand = operand;
    node->binop.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_arg_node(jacc_ast_node_t *arg) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_ARG_AST;
    node->argument.arg = arg;
    node->argument.next_arg = NULL;
    node->argument.n_args = 1;

    return node;
}

jacc_ast_node_t* jacc_append_arg_node(jacc_ast_node_t *arg_list, jacc_ast_node_t *next_arg) {
    jacc_ast_node_t *iter = arg_list;

    arg_list->argument.n_args++;

    while (iter->argument.next_arg != NULL) {
        iter = iter->argument.next_arg;
    }

    iter->argument.next_arg = next_arg;
    //next_arg->argument.arg = next_arg;

    return arg_list;
}

jacc_ast_node_t* jacc_alloc_conditional_node(jacc_ast_node_t *condition, jacc_ast_node_t *true_op, jacc_ast_node_t *false_op) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_CONDITIONAL_AST;
    node->conditional.condition = condition;
    node->conditional.true_op = true_op;
    node->conditional.false_op = false_op;

    return node;
}

void print_tabs(int indentation) {
    for (int i = 0; i < indentation; i++) {
        printf("\t");
    }
}

void print_lex_data(jacc_lex_tok_t *tok) {
    switch (tok->data_type) {
        case JACC_TYPE_CHARLIT:
            printf("%c\n", tok->data.char_d);
            break;
        case JACC_TYPE_CHAR_ESC:
            printf("(type=int)%d\n", tok->data.char_d);
            break;
        case JACC_TYPE_STRING:
            printf("%s\n", tok->data.string_d);
            break;
        case JACC_TYPE_INT:
            printf("(type=int)%d\n", tok->data.int_d);
            break;
        case JACC_TYPE_UINT:
            printf("(type=unsigned int)%d\n", tok->data.uint_d);
            break;
        case JACC_TYPE_LONG:
            printf("(type=long)%ld\n", tok->data.long_d);
            break;
        case JACC_TYPE_ULONG:
            printf("(type=unsigned long)%ld\n", tok->data.ulong_d);
            break;
        case JACC_TYPE_ULONGLONG:
            printf("(type=unsigned long long)%lld\n", tok->data.ulonglong_d);
            break;

        default:
            exit(1);
    }
}

void print_ast(jacc_ast_node_t *ast) {
    switch (ast->ast_type) {
        // Following have no children
        case JACC_IDENT_AST:
            printf("IDENT ");
            print_lex_data(&ast->lex);
            break;
        case JACC_NUM_AST:
            printf("CONSTANT: ");
            print_lex_data(&ast->lex);
            break;
        case JACC_STRING_AST:
            printf("STRING ");
            print_lex_data(&ast->lex);
            break;
        case JACC_CHAR_AST:
            printf("CONSTANT: ");
            print_lex_data(&ast->lex);
            break;

        case JACC_BINOP_AST:
            printf("BINARY OP %c\n", ast->binop.op);
            print_ast(ast->binop.operand);
            print_ast(ast->binop.operator);
            break;
        case JACC_FUNC_AST:
            printf("FNCALL, %d, arguments", ast->generic.operator->argument.n_args);
            print_ast(ast->generic.operand);
            print_ast(ast->generic.operator);
            break;
        case JACC_ARG_AST:
            print_ast(ast->argument.arg);

            jacc_ast_node_t *iter = ast;

            while (iter->argument.next_arg != NULL) {
                //if (iter->ast_type != JACC_ARG_AST) {
                //    break;
                //}
                iter = iter->argument.next_arg;
                print_ast(iter);
            }
            break;
        case JACC_SEL_AST:
            printf("SELECT, member ");
            print_ast(ast->generic.operator);
            print_ast(ast->generic.operand);
            break;
        case JACC_INDIR_SEL_AST:
            printf("INDIRECT SELECT, member ");
            print_ast(ast->generic.operator);
            print_ast(ast->generic.operand);
            break;
        case JACC_DEREF_AST:
            printf("DEREF\n");
            print_ast(ast->generic.operand);
            break;
        case JACC_POST_OP_AST:
            break;
        case JACC_PRE_OP_AST:
            break;
        case JACC_CAST_AST:
            break;
        case JACC_SIZEOF_AST:
            break;
        case JACC_CONDITIONAL_AST:
            break;
        default:
            exit(1);
    }
}
