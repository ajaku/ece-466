#include "ast.h"
#include "types.h"

const char *ast_to_str[] = {
    "BASE",
    "FUNC",
    "ARGS",
    "UNARY OP",
    "BINARY OP",
    "TERNARY"
};

const char *base_to_str[] = {
  "CHAR",
  "CHAR",
  "STRING",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT",
  "CONSTANT"
};

const char *un_op_to_str[] = {
    "STANDARD",
    "DEREF",
    "SIZEOF",
    "ADDROF",
    "POSTINC"
    "POSTSUB"
};

const char *bin_op_to_str[] = {
    "STANDARD",
    "ASSIGNMENT",
    "ASSIGNMENT COMPOUND",
    "COMPARE",
    "LOGICAL",
    "SELECT",
    "SELECT INDIR"
};

/*
const char *ast_type_to_string[] = {
    "IDENT",
    "CONST",
    "STRING",
    "CHAR",
    "FUNC",
    "ARGS",
    "UNARY OP",
    "DEREF",
    "SIZEOF",
    "ADDRESS OF",
    "UNARY OP POSTINC",
    "UNARY OP POSTSUB",
    "BINARY OPERATOR",
    "ASSIGNMENT",
    "ASSIGNMENT COMPOUND",
    "COMPARE OP",
    "LOGICAL OPERATOR",
    "SELECT",
    "INDIRECT SELECT",
    "TERNARY"
};
*/

jacc_ast_node_t* jacc_alloc_node(jacc_ast_type_t type) {
    jacc_ast_node_t *node = malloc(sizeof(jacc_ast_node_t));

    if (!node) {
        fprintf(stderr, "Failed to allocated AST node.\n");
        exit(1);
    }

    node->ast_type = type;
    return node;
}

jacc_ast_node_t* jacc_alloc_base(jacc_lex_tok_t lex) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_BASE_AST);
    node->lex = lex;

    return node;
}

jacc_ast_node_t* jacc_alloc_func(jacc_ast_node_t *func, jacc_ast_node_t *args) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_FUNC_AST);
    node->func.func = func;
    node->func.args = args;

    return node;
}

jacc_ast_node_t* jacc_alloc_args(jacc_ast_node_t *arg) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_ARGS_AST);
    node->args.n_args   = 1;
    node->args.arg      = arg;
    node->args.next_arg = NULL;

    return node;
}

jacc_ast_node_t* jacc_alloc_unop(jacc_un_op_type_t u_type, int operator, jacc_ast_node_t *operand) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_UN_OP_AST);
    node->unop.u_type = u_type;
    node->unop.operator = operator;
    node->unop.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_binop(jacc_bin_op_type_t b_type, int operator, jacc_ast_node_t *left, jacc_ast_node_t *right) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_BIN_OP_AST);
    node->binop.b_type = b_type;
    node->binop.operator = operator;
    node->binop.left = left;
    node->binop.right = right;

    return node;
}

jacc_ast_node_t* jacc_alloc_ternary(jacc_ast_node_t *cond, jacc_ast_node_t *t_op, jacc_ast_node_t *f_op) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TERNARY_AST);
    node->tern.cond = cond;
    node->tern.t_op = t_op;
    node->tern.f_op = f_op;

    return node;
}

jacc_ast_node_t* jacc_append_arg(jacc_ast_node_t *arg_list, jacc_ast_node_t *next_arg) {
    jacc_ast_node_t *iter = arg_list;

    jacc_ast_node_t *cpy = malloc(sizeof(jacc_ast_node_t));

    arg_list->args.n_args++;

    while (iter->args.next_arg != NULL) {
        iter = iter->args.next_arg;
    }

    // Store a local copy of the argument to prevent referencing modified version while prenting
    *cpy = *next_arg;

    iter->args.next_arg = cpy;
    iter->args.next_arg->args.arg = next_arg;
    iter->args.next_arg->args.next_arg = NULL;

    return arg_list;
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

void jacc_print_indent(int *ident) {
    for (int i = 0; i < *ident; i++) {
        printf(" ");
    }
}

void jacc_print_base(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("%s ", base_to_str[node->lex.data_type]);
    jacc_print_lex_data(&node->lex);
}

void jacc_print_unary_op(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("%s ", base_to_str[node->ast_type]);
    printf("%s ", un_op_to_str[node->unop.u_type]);
    //printf("%c", node->unop.operator);
    //jacc_print_lex_data(&node->unop.lex);
}

/*
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
    if (node->operand_operator_ast.operator != NULL) {
        jacc_print_ast(ident, node->operand_operator_ast.operator);
    }
}

void jacc_print_attr_operand_operator_ast(int *ident, jacc_ast_node_t *node) {
    jacc_print_ident(ident);
    printf("%s ", ast_type_to_string[node->ast_type]);
    printf("%s\n", node->attr_operand_operator_ast.attr);
    jacc_print_ast(ident, node->attr_operand_operator_ast.operand);
    jacc_print_ast(ident, node->attr_operand_operator_ast.operator);
}

void jacc_print_arg_ast(int *ident, jacc_ast_node_t *node) {
    printf("woah\n");
    if (node == NULL) {
        printf("nope\n");
        return;
    }
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
*/

void jacc_print_ast(int *indent, jacc_ast_node_t *node) {
    jacc_auto_indent(indent, node, jacc_print_base);
    /*
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
            */
}
