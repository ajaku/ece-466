#include "ast.h"
#include "jacc_parser.tab.h"
#include "types.h"

const char *ast_to_str[] = {
    "IDENT",
    "CONST",
    "STRING",
    "CHAR",
    "FUNC",
    "ARGS",
    "UNARY OP",
    "BINARY OP",
    "TERNARY OP"
};

const char *un_op_to_str[] = {
    "",
    "DEREF",
    "SIZEOF",
    "ADDROF",
    "POSTINC",
    "POSTSUB"
};

const char *bin_op_to_str[] = {
    "",
    "ASSIGNMENT",
    "ASSIGNMENT COMPOUND",
    "COMPARE OP",
    "LOGICAL OP",
    "SELECT",
    "SELECT INDIR"
};

jacc_ast_node_t* jacc_alloc_node(jacc_ast_type_t type) {
    jacc_ast_node_t *node = calloc(1, sizeof(jacc_ast_node_t));

    if (!node) {
        fprintf(stderr, "Failed to allocated AST node.\n");
        exit(1);
    }

    node->ast_type = type;
    return node;
}

jacc_ast_node_t* jacc_alloc_base(jacc_ast_type_t type,
                                 jacc_lex_tok_t lex) {
    jacc_ast_node_t *node = jacc_alloc_node(type);
    node->lex = lex;

    return node;
}

jacc_ast_node_t* jacc_alloc_func(jacc_ast_node_t *func,
                                 jacc_ast_node_t *args) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_FUNC_AST);
    node->func.func = func;
    node->func.args = args;

    return node;
}

jacc_ast_node_t* jacc_alloc_args(jacc_ast_node_t *arg) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_ARGS_AST);
    node->args.n_args = 1;
    node->args.arg = arg;
    node->args.next_arg = NULL;

    return node;
}

jacc_ast_node_t* jacc_alloc_unop(jacc_un_op_type_t u_type,
                                 int operator,
                                 jacc_ast_node_t *operand) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_UN_OP_AST);
    node->unop.u_type = u_type;
    node->unop.operator = operator;
    node->unop.operand = operand;

    return node;
}

jacc_ast_node_t* jacc_alloc_binop(jacc_bin_op_type_t b_type,
                                  int operator,
                                  jacc_ast_node_t *left,
                                  jacc_ast_node_t *right) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_BIN_OP_AST);
    node->binop.b_type = b_type;
    node->binop.operator = operator;
    node->binop.left = left;
    node->binop.right = right;

    return node;
}

jacc_ast_node_t* jacc_alloc_ternary(jacc_ast_node_t *cond,
                                    jacc_ast_node_t *t_op,
                                    jacc_ast_node_t *f_op) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TERNARY_AST);
    node->tern.cond = cond;
    node->tern.t_op = t_op;
    node->tern.f_op = f_op;

    return node;
}

jacc_ast_node_t *jacc_alloc_stg_cls_spec(jacc_stg_cls_type_t type) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_STG_CLS_AST);
    node->stg_cls_spec.type = type;

    return node;
}

jacc_ast_node_t* jacc_alloc_type_spec(jacc_type_spec_type_t type,
                                      jacc_ast_node_t *spec) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TYPE_SPEC_AST);
    node->type_spec.type = type;

    if (type == JACC_TYPE_SPEC_STRUCT_OR_UNION)
        node->type_spec.struct_or_union_spec = spec;

    if (type == JACC_TYPE_SPEC_ENUM)
        node->type_spec.enum_spec = spec;

    return node;
}

jacc_ast_node_t* jacc_alloc_type_qual(jacc_type_qual_type_t type) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TYPE_QUAL_LIST_AST);

    unsigned *n_type_quals = malloc(sizeof(unsigned));

    node->type_qual_list.type = type;
    node->type_qual_list.n_type_quals = n_type_quals;
    node->type_qual_list.list_head = node;

    (*(node->type_qual_list.n_type_quals))++;

    node->type_qual_list.type = type;

    return node;
}

jacc_ast_node_t* jacc_alloc_declaration_spec(jacc_ast_node_t *stg_cls_spec, 
                                             jacc_ast_node_t *declaration_spec,
                                             jacc_ast_node_t *type_spec) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DECLARATION_SPEC_AST);
    node->declaration_spec.stg_cls_spec = stg_cls_spec;
    node->declaration_spec.declaration_spec = declaration_spec;
    node->declaration_spec.type_spec = type_spec;


    return node;
}

jacc_ast_node_t* jacc_alloc_struct_or_union_spec(jacc_struct_or_union_spec_type_t type,
                                                 jacc_ast_node_t *ident,
                                                 jacc_ast_node_t *struct_declaration_list) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_STRUCT_OR_UNION_SPEC_AST);
    node->struct_or_union_spec.type = type;
    node->struct_or_union_spec.ident = ident;
    node->struct_or_union_spec.struct_declaration_list = struct_declaration_list;

    return node;
}

/*
jacc_ast_node_t *jacc_alloc_struct_declaration_list_ast() {

    jacc_ast_node_t *node = jacc_alloc_node(JACC_STRUCT_OR_UNION_SPEC_AST);
    node->struct_or_union_spec.type = type;
    node->struct_or_union_spec.ident = ident;
    node->struct_or_union_spec.struct_declaration_list = struct_declaration_list;

    return node;

}
    */

jacc_ast_node_t* jacc_alloc_spec_qual_list(jacc_ast_node_t *type_spec,
                                           jacc_ast_node_t *type_qual) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_SPEC_QUAL_LIST_AST);

    unsigned *n_spec_quals = malloc(sizeof(unsigned));

    node->spec_qual_list.n_spec_quals = n_spec_quals;
    node->spec_qual_list.list_head = node;
    node->spec_qual_list.type_spec = type_spec;
    node->spec_qual_list.type_qual = type_qual;

    (*(node->spec_qual_list.n_spec_quals))++;

    return node;
}

jacc_ast_node_t* jacc_alloc_declarator(jacc_ast_node_t *pointer,
                                       jacc_ast_node_t *direct_declarator) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DECLARATOR_AST);

    node->declarator.pointer = pointer;
    node->declarator.direct_declarator = direct_declarator;

    return node;
}

jacc_ast_node_t* jacc_alloc_direct_declarator(jacc_ast_node_t *ident,
                                              jacc_ast_node_t *declarator,
                                              jacc_ast_node_t *direct_declarator,
                                              jacc_ast_node_t *number) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DIRECT_DECLARATOR_AST);
    node->direct_declarator.ident = ident;
    node->direct_declarator.declarator = declarator;
    node->direct_declarator.direct_declarator = direct_declarator;
    node->direct_declarator.number = number;

    return node;
}

jacc_ast_node_t* jacc_alloc_pointer(jacc_ast_node_t *type_qual_list) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_POINTER_AST);
    
    //node->pointer.type_qual_list = ident;

    return node;
}

jacc_ast_node_t* jacc_append_type_qual_list(jacc_ast_node_t *type_qual_list,
                                            jacc_ast_node_t *type_qual) {
    (*(type_qual_list->type_qual_list.n_type_quals))++;
    type_qual_list->type_qual_list.next_type_qual = type_qual;

    return type_qual;
}

jacc_ast_node_t* jacc_append_spec_qual_list(jacc_ast_node_t *spec_qual_list,
                                            jacc_ast_node_t *type_spec,
                                            jacc_ast_node_t *type_qual) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_SPEC_QUAL_LIST_AST);

    node->spec_qual_list.n_spec_quals = spec_qual_list->spec_qual_list.n_spec_quals;
    node->spec_qual_list.list_head = spec_qual_list->spec_qual_list.list_head;
    node->spec_qual_list.type_spec = type_spec;
    node->spec_qual_list.type_qual = type_qual;

    (*(node->spec_qual_list.n_spec_quals))++;

    spec_qual_list->spec_qual_list.next_spec_qual = node;

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

void jacc_print_operator(int operator) {
    if (operator <= 126) {
        printf("%c\n", operator);
        return;
    }

    switch (operator) {
        case INDSEL:
            printf(".\n");
            break;
        case SHL:
            printf("<<\n");
            break;
        case SHR:
            printf(">>\n");
            break;
        case LTEQ:
            printf("<=\n");
            break;
        case GTEQ:
            printf(">=\n");
            break;
        case EQEQ:
            printf("==\n");
            break;
        case NOTEQ:
            printf("!=\n");
            break;
        case LOGAND:
            printf("&&\n");
            break;
        case LOGOR:
            printf("||\n");
            break;
        case TIMESEQ:
            printf("*=\n");
            break;
        case DIVEQ:
            printf("/=\n");
            break;
        case MODEQ:
            printf("%%=\n");
            break;
        case PLUSEQ:
            printf("+=\n");
            break;
        case MINUSEQ:
            printf("-=\n");
            break;
        case SHLEQ:
            printf("<<=\n");
            break;
        case SHREQ:
            printf(">>=\n");
            break;
        case ANDEQ:
            printf("&=\n");
            break;
        case XOREQ:
            printf("^=\n");
            break;
        case OREQ:
            printf("|=\n");
            break;
        default:
            printf("\n");
            break;
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
    printf("%s ", ast_to_str[node->ast_type]);
    jacc_print_lex_data(&node->lex);
}

void jacc_print_un_op(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("%s ", ast_to_str[node->ast_type]);
    printf("%s ", un_op_to_str[node->unop.u_type]);
    jacc_print_operator(node->unop.operator);
    jacc_print_ast(indent, node->unop.operand);
}

void jacc_print_bin_op(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    if (node->binop.b_type == JACC_BIN_OP_STD) {
        printf("%s ", ast_to_str[node->ast_type]);
    }
    printf("%s ", bin_op_to_str[node->binop.b_type]);
    jacc_print_operator(node->binop.operator);
    jacc_print_ast(indent, node->binop.left);
    jacc_print_ast(indent, node->binop.right);
}

void jacc_print_func(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("%s\n", ast_to_str[node->ast_type]);
    jacc_print_ast(indent, node->func.func);
    jacc_print_ast(indent, node->func.args);
}

void jacc_print_args(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("arg #1=\n");
    jacc_print_ast(indent, node->args.arg);

    jacc_ast_node_t *iter = node;
    int i = 1;
    while (iter->args.next_arg != NULL) {
        i++;
        iter = iter->args.next_arg;
        jacc_print_indent(indent);
        printf("arg #%d=\n", i);
        jacc_print_ast(indent, iter->args.arg);
    }
}

void jacc_print_ternary(int *indent, jacc_ast_node_t *node) {
    jacc_print_indent(indent);
    printf("%s IF:\n", ast_to_str[node->ast_type]);
    jacc_print_ast(indent, node->tern.cond);
    printf("THEN:\n");
    jacc_print_ast(indent, node->tern.t_op);
    printf("ELSE:\n");
    jacc_print_ast(indent, node->tern.f_op);
}

void jacc_print_ast(int *indent, jacc_ast_node_t *node) {
    switch (node->ast_type) {
        case JACC_IDENT_AST:
        case JACC_CONST_AST:
        case JACC_STRING_AST:
        case JACC_CHAR_AST:
            jacc_auto_indent(indent, node, jacc_print_base);
            break;
        case JACC_FUNC_AST:
            jacc_auto_indent(indent, node, jacc_print_func);
            break;
        case JACC_ARGS_AST:
            jacc_auto_indent(indent, node, jacc_print_args);
            break;
        case JACC_UN_OP_AST:
            jacc_auto_indent(indent, node, jacc_print_un_op);
            break;
        case JACC_BIN_OP_AST:
            jacc_auto_indent(indent, node, jacc_print_bin_op);
            break;
        case JACC_TERNARY_AST:
            jacc_auto_indent(indent, node, jacc_print_ternary);
            break;
        default:
            printf("None\n");
    }
}
