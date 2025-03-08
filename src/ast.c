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
