#include "ast.h"
#include "types.h"

const char *ast_type_to_string[] = {
    "IDENT",
    "CONST",
    "STRING",
    "CHAR",
    "BINARY_OP",
    "LOGICAL_OP",
    "COMPARE_OP",
    "FUNCTION",
    "ARG",
    "SELECT",
    "INDIR_SEL",
    "DEREF",
    "POSTINC_OP",
    "POSTSUB_OP",
    "ASSIGN_COMP",
    "ADDR_OF_OP",
    "UNARY_OP",
    "CAST",
    "SIZEOF",
    "TERNARY_OP"
};

jacc_ast_node_t* jacc_alloc_node(jacc_ast_type_t type) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    if (!node) {
        fprintf(stderr, "Failed to allocated AST node.\n");
        exit(1);
    }

    node->ast_type = type;
    return node;
}

jacc_ast_node_t* jacc_alloc_base_node(jacc_ast_type_t type,
                                      jacc_lex_tok_t lex) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_NO_TYPE_U;
    node->lex = lex;

    return node;
}

jacc_ast_node_t* jacc_alloc_operand_node(jacc_ast_type_t type,
                                         jacc_ast_node_t *operand) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_OPERAND_U;
    node->operand_ast.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_attr_operand_node(jacc_ast_type_t type,
                                              char *attr,
                                              jacc_ast_node_t *operand) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_ATTR_OPERAND_U;
    node->attr_operand_ast.attr = attr;
    node->attr_operand_ast.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_operand_operator_node(jacc_ast_type_t type,
                                                  jacc_ast_node_t *operand,
                                                  jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_OPERAND_OPERATOR_U;
    node->operand_operator_ast.operand = operand;
    node->operand_operator_ast.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_attr_operand_operator_node(jacc_ast_type_t type,
                                                       char *attr,
                                                       jacc_ast_node_t *operand,
                                                       jacc_ast_node_t *operator) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_ATTR_OPERAND_OPERATOR_U;
    node->attr_operand_operator_ast.attr = attr;
    node->attr_operand_operator_ast.operand = operand;
    node->attr_operand_operator_ast.operator = operator;

    return node;
}

jacc_ast_node_t* jacc_alloc_arg_node(jacc_ast_type_t type,
                                     jacc_ast_node_t *arg) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_ARG_U;
    node->arg_ast.n_arg = 1;
    node->arg_ast.arg = arg;
    node->arg_ast.next_arg = NULL;

    return node;
}

jacc_ast_node_t* jacc_append_arg_node(jacc_ast_node_t *arg_list, 
                                      jacc_ast_node_t *next_arg) {
    jacc_ast_node_t *iter = arg_list;

    jacc_ast_node_t *cpy = malloc(sizeof(jacc_ast_node_t));

    arg_list->arg_ast.n_arg++;

    while (iter->arg_ast.next_arg != NULL) {
        iter = iter->arg_ast.next_arg;
    }

    // Store a local copy of the argument to prevent referencing modified version while prenting
    *cpy = *next_arg;

    iter->arg_ast.next_arg = cpy;
    iter->arg_ast.next_arg->arg_ast.arg = next_arg;
    iter->arg_ast.next_arg->arg_ast.next_arg = NULL;

    return arg_list;
}

jacc_ast_node_t* jacc_alloc_ternary_node(jacc_ast_type_t type,
                                         jacc_ast_node_t *cond, 
                                         jacc_ast_node_t *t_op,
                                         jacc_ast_node_t *f_op) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->union_type = JACC_TERNAY_U;
    node->ternary_ast.cond = cond;
    node->ternary_ast.t_op = t_op;
    node->ternary_ast.f_op = f_op;

    return node;
}

void jacc_print_lex_data(jacc_lex_tok_t *tok) {
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

void jacc_auto_indent(int *indent, jacc_ast_node_t *node, void(*func)(int*, jacc_ast_node_t*)) {
    (*indent)++;
    (*func)(indent, node);
    (*indent)--;
}

void jacc_print_ident(int *ident) {
    for (int i = 0; i < *ident; i++) {
        printf(" ");
    }
}

void jacc_print_no_type_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s ", ast_type_to_string[node->ast_type]);
    jacc_print_lex_data(&node->lex);
}

void jacc_print_operand_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s\n", ast_type_to_string[node->ast_type]);
    jacc_print_ast(ident, node->operand_ast.operand);
}

void jacc_print_attr_operand_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s ", ast_type_to_string[node->ast_type]);
    printf("%s\n", node->attr_operand_ast.attr);
    jacc_print_ast(ident, node->attr_operand_ast.operand);
}

void jacc_print_operand_operator_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s\n", ast_type_to_string[node->ast_type]);
    jacc_print_ast(ident, node->operand_operator_ast.operand);
    jacc_print_ast(ident, node->operand_operator_ast.operator);
}

void jacc_print_attr_operand_operator_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s ", ast_type_to_string[node->ast_type]);
    printf("%s\n", node->attr_operand_operator_ast.attr);
    jacc_print_ast(ident, node->attr_operand_operator_ast.operand);
    jacc_print_ast(ident, node->attr_operand_operator_ast.operator);
}

void jacc_print_arg_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("arg #1=\n");
    jacc_print_ast(ident, node->arg_ast.arg);

    jacc_ast_node_t *iter = node;
    int i = 1;
    while (iter->arg_ast.next_arg != NULL) {
        i++;
        iter = iter->arg_ast.next_arg;
        jacc_print_ident(ident);
        printf("arg #%d=\n", i);
        jacc_print_ast(ident, iter->arg_ast.arg);
    }
}

void jacc_print_ternary_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s IF:\n", ast_type_to_string[node->ast_type]);
    jacc_print_ast(ident, node->ternary_ast.cond);
    printf("THEN:\n");
    jacc_print_ast(ident, node->ternary_ast.t_op);
    printf("ELSE:\n");
    jacc_print_ast(ident, node->ternary_ast.f_op);
}

void jacc_print_ast(int *indent, jacc_ast_node_t *node) {
    switch (node->union_type) {
        case JACC_NO_TYPE_U:
            jacc_auto_indent(indent, node, jacc_print_no_type_ast);
            break;
        case JACC_OPERAND_U:
            jacc_auto_indent(indent, node, jacc_print_operand_ast);
            break;
        case JACC_ATTR_OPERAND_U:
            jacc_auto_indent(indent, node, jacc_print_attr_operand_ast);
            break;
        case JACC_OPERAND_OPERATOR_U:
            jacc_auto_indent(indent, node, jacc_print_operand_operator_ast);
            break;
        case JACC_ATTR_OPERAND_OPERATOR_U:
            jacc_auto_indent(indent, node, jacc_print_attr_operand_operator_ast);
            break;
        case JACC_ARG_U:
            jacc_print_arg_ast(indent, node);
            break;
        case JACC_TERNAY_U:
            jacc_auto_indent(indent, node, jacc_print_ternary_ast);
            break;
        default:
            exit(1);
    }
}


/*
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
*/