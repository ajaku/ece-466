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

jacc_ast_node_t* jacc_alloc_binop_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_BINOP_AST;
    node->binop.op = op;
    node->binop.operand = operand;
    node->binop.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_logical_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_LOG_OP_AST;
    node->logical.op = op;
    node->logical.operand = operand;
    node->logical.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_compare_node(char *op, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_COMPARE_AST;
    node->compare.op = op;
    node->compare.operand = operand;
    node->compare.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_generic_node(jacc_ast_type_t type, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = type;
    node->generic.operand = operand;
    node->generic.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_asgn_cmpd_node(char *cmpd, jacc_ast_node_t *operand, jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_ASSIGN_COMP_AST;
    node->asgn_cmpd.cmpd = cmpd;
    node->asgn_cmpd.operand = operand;
    node->asgn_cmpd.operator = operator;

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

    jacc_ast_node_t *cpy = malloc(sizeof(jacc_ast_node_t));

    arg_list->argument.n_args++;

    while (iter->argument.next_arg != NULL) {
        iter = iter->argument.next_arg;
    }

    // Store a local copy of the argument to prevent referencing modified version while prenting
    *cpy = *next_arg;

    iter->argument.next_arg = cpy;
    iter->argument.next_arg->argument.arg = next_arg;
    iter->argument.next_arg->argument.next_arg = NULL;

    return arg_list;
}

jacc_ast_node_t* jacc_alloc_unary_op_node(jacc_lex_tok_t tok, jacc_ast_node_t *operand) { 
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_UNARY_OP_AST;
    node->unary_op.op = tok.data.string_d;
    node->unary_op.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_addr_of_node(jacc_ast_node_t *operand) { 
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_ADDR_OF_AST;
    node->addr_of.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_sizeof_node(jacc_ast_node_t *operand) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));
    node->ast_type = JACC_SIZEOF_AST;
    node->sizeof_op.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_ternary_node(jacc_ast_node_t *condition, jacc_ast_node_t *true_op, jacc_ast_node_t *false_op) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    node->ast_type = JACC_TERNARY_AST;
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

void print_tab(int *rec_lvl) {
    for (int i = 0; i < *rec_lvl; i++) {
        printf("\t");
    }
}

void print_ast(int *rec_lvl, jacc_ast_node_t *ast) {
    switch (ast->ast_type) {
        // Following have no children
        case JACC_IDENT_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("IDENT ");
            print_lex_data(&ast->lex);
            *rec_lvl -= 1; break;
        case JACC_NUM_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("CONSTANT: ");
            print_lex_data(&ast->lex);
            *rec_lvl -= 1; break;
        case JACC_STRING_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("STRING ");
            print_lex_data(&ast->lex);
            *rec_lvl -= 1; break;
        case JACC_CHAR_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("CONSTANT: ");
            print_lex_data(&ast->lex);
            *rec_lvl -= 1; break;
        case JACC_BINOP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("BINARY OP %s\n", ast->binop.op);
            print_ast(rec_lvl, ast->binop.operand);
            print_ast(rec_lvl, ast->binop.operator);
            *rec_lvl -= 1; break;
        case JACC_LOG_OP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("LOGICAL OP %s\n", ast->logical.op);
            print_ast(rec_lvl, ast->logical.operand);
            print_ast(rec_lvl, ast->logical.operator);
            *rec_lvl -= 1; break;
        case JACC_COMPARE_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("COMPARISON OP %s\n", ast->compare.op);
            print_ast(rec_lvl, ast->compare.operand);
            print_ast(rec_lvl, ast->compare.operator);
            *rec_lvl -= 1; break;
        case JACC_FUNC_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("FNCALL, %d, arguments\n", ast->generic.operator->argument.n_args);
            print_ast(rec_lvl, ast->generic.operand);
            print_ast(rec_lvl, ast->generic.operator);
            *rec_lvl -= 1; break;
        case JACC_ARG_AST:
            print_tab(rec_lvl);
            printf("arg #1=\n");
            print_ast(rec_lvl, ast->argument.arg);

            jacc_ast_node_t *iter = ast;

            int i = 1;
            while (iter->argument.next_arg != NULL) {
                i++;
                iter = iter->argument.next_arg;
                printf("arg #%d=\n", i);
                print_ast(rec_lvl, iter->argument.arg);
            }
            break;
        case JACC_SEL_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("SELECT, member ");
            i = -1;
            print_ast(&i, ast->generic.operator);
            print_ast(rec_lvl, ast->generic.operand);
            *rec_lvl -= 1; break;
        case JACC_INDIR_SEL_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("INDIRECT SELECT, member ");
            i = 0;
            print_ast(&i, ast->generic.operator);
            print_ast(rec_lvl, ast->generic.operand);
            *rec_lvl -= 1; break;
        case JACC_DEREF_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("DEREF\n");
            print_ast(rec_lvl, ast->generic.operand);
            *rec_lvl -= 1; break;
        case JACC_POSTINC_OP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("UNARY OP POSTINC\n");
            print_ast(rec_lvl, ast->generic.operand);
            *rec_lvl -= 1; break;
        case JACC_POSTSUB_OP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("UNARY OP POSTSUB\n");
            print_ast(rec_lvl, ast->generic.operand);
            *rec_lvl -= 1; break;
        case JACC_ASSIGN_COMP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("ASSIGNMENT COMPOUND (%s)\n", ast->asgn_cmpd.cmpd);
            print_ast(rec_lvl, ast->asgn_cmpd.operand);
            print_ast(rec_lvl, ast->asgn_cmpd.operator);
            *rec_lvl -= 1; break;
        case JACC_UNARY_OP_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("UNARY OP %s\n", ast->unary_op.op);
            print_ast(rec_lvl, ast->unary_op.operand);
            *rec_lvl -= 1; break;
        case JACC_ADDR_OF_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("ADDRESSOF\n");
            print_ast(rec_lvl, ast->addr_of.operand);
            *rec_lvl -= 1; break;
        case JACC_CAST_AST:
            *rec_lvl += 1;
            *rec_lvl -= 1; break;
        case JACC_SIZEOF_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("SIZEOF\n");
            print_ast(rec_lvl, ast->sizeof_op.operand);
            *rec_lvl -= 1; break;
        case JACC_TERNARY_AST:
            *rec_lvl += 1;
            print_tab(rec_lvl);
            printf("TERNARY OP, IF:\n");
            print_ast(rec_lvl, ast->conditional.condition);
            printf("THEN:\n");
            print_ast(rec_lvl, ast->conditional.true_op);
            printf("ELSE:\n");
            print_ast(rec_lvl, ast->conditional.false_op);
            *rec_lvl -= 1; break;
        default:
            exit(1);
    }
}
