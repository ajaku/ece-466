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

// New stuff

jacc_stg_cls_spec_ast_t *jacc_alloc_stg_cls_spec(jacc_stg_cls_type_t type) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_STG_CLS_AST);
    jacc_stg_cls_spec_ast_t *node_stg_cls_spec = &node->stg_cls_spec;

    node_stg_cls_spec->type = type;

    return node_stg_cls_spec;
}

jacc_type_spec_ast_t* jacc_alloc_type_spec(jacc_type_spec_type_t type,
                                           jacc_ast_node_t *spec) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TYPE_SPEC_AST);
    jacc_type_spec_ast_t *node_type_spec = &node->type_spec;

    node_type_spec->type = type;

    /*
    if (type == JACC_TYPE_SPEC_STRUCT_OR_UNION)
        node_type_spec->struct_or_union_spec = spec;

    if (type == JACC_TYPE_SPEC_ENUM)
        node_type_spec->enum_spec = spec;

    */

    // Temporary fix (still need to add enum support)

    node_type_spec->struct_or_union_spec = &spec->struct_or_union_spec;

    return node_type_spec;
}

jacc_type_qual_ast_t* jacc_alloc_type_qual(jacc_type_qual_type_t type) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TYPE_QUAL_AST);
    jacc_type_qual_ast_t *node_type_qual = &node->type_qual;

    node_type_qual->type = type;

    return node_type_qual;
}

jacc_type_qual_list_ast_t* jacc_alloc_type_qual_list(jacc_type_qual_ast_t *list_head) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_TYPE_QUAL_LIST_AST);
    jacc_type_qual_list_ast_t *node_type_qual_list = &node->type_qual_list;

    unsigned *n_type_quals = malloc(sizeof(unsigned));

    node_type_qual_list->n_type_quals = n_type_quals;
    node_type_qual_list->list_head = list_head;

    (*(node_type_qual_list->n_type_quals))++;

    return node_type_qual_list;
}

jacc_declaration_spec_ast_t* jacc_alloc_declaration_spec(jacc_stg_cls_spec_ast_t *stg_cls_spec, 
                                                         jacc_declaration_spec_ast_t *declaration_spec,
                                                         jacc_type_spec_ast_t *type_spec) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DECLARATION_SPEC_AST);
    jacc_declaration_spec_ast_t *node_declaration_spec = &node->declaration_spec;

    node_declaration_spec->stg_cls_spec = stg_cls_spec;
    node_declaration_spec->declaration_spec = declaration_spec;
    node_declaration_spec->type_spec = type_spec;

    return node_declaration_spec;
}

jacc_struct_or_union_spec_ast_t* jacc_alloc_struct_or_union_spec(jacc_struct_or_union_spec_type_t type,
                                                                 jacc_ast_node_t *ident,
                                                                 jacc_struct_declaration_ast_t *struct_declaration_list) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_STRUCT_OR_UNION_SPEC_AST);
    jacc_struct_or_union_spec_ast_t *node_struct_or_union_spec = &node->struct_or_union_spec;
    node_struct_or_union_spec->type = type;
    node_struct_or_union_spec->ident = ident;
    node_struct_or_union_spec->struct_declaration_list = struct_declaration_list;

    return node_struct_or_union_spec;
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

jacc_spec_qual_list_ast_t* jacc_alloc_spec_qual_list(jacc_type_spec_ast_t *type_spec,
                                                     jacc_type_qual_ast_t *type_qual) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_SPEC_QUAL_LIST_AST);
    jacc_spec_qual_list_ast_t *node_spec_qual_list = &node->spec_qual_list;

    unsigned *n_spec_quals = malloc(sizeof(unsigned));

    jacc_spec_qual_ast_t *spec_qual = malloc(sizeof(jacc_spec_qual_ast_t));

    if (type_spec != NULL) spec_qual->type_spec = type_spec;
    if (type_qual != NULL) spec_qual->type_qual = type_qual;

    node_spec_qual_list->n_spec_quals = n_spec_quals;
    node_spec_qual_list->list_head = spec_qual;

    (*(node_spec_qual_list->n_spec_quals))++;

    return node_spec_qual_list;
}

jacc_declarator_ast_t* jacc_alloc_declarator(jacc_pointer_ast_t *pointer,
                                             jacc_direct_declarator_ast_t *direct_declarator) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DECLARATOR_AST);
    jacc_declarator_ast_t *node_declarator = &node->declarator;

    node_declarator->pointer = pointer;
    node_declarator->direct_declarator = direct_declarator;

    return node_declarator;
}

jacc_direct_declarator_ast_t* jacc_alloc_direct_declarator(jacc_ast_node_t *ident,
                                                           jacc_declarator_ast_t *declarator,
                                                           jacc_direct_declarator_ast_t *direct_declarator,
                                                           jacc_ast_node_t *number) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_DIRECT_DECLARATOR_AST);
    jacc_direct_declarator_ast_t *node_direct_declarator = &node->direct_declarator;

    node_direct_declarator->ident = ident;
    node_direct_declarator->declarator = declarator;
    node_direct_declarator->direct_declarator = direct_declarator;
    node_direct_declarator->number = number;

    return node_direct_declarator;
}

jacc_pointer_ast_t* jacc_alloc_pointer(jacc_type_qual_list_ast_t *type_qual_list) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_POINTER_AST);
    jacc_pointer_ast_t *node_pointer = &node->pointer;

    node_pointer->type_qual_list = type_qual_list;
    
    return node_pointer;
}

jacc_init_declarator_ast_t* jacc_alloc_init_declarator(jacc_declarator_ast_t *declarator) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_INIT_DECLARATOR_AST);
    jacc_init_declarator_ast_t *node_init_declarator = &node->init_declarator;

    node_init_declarator->declarator = declarator;
    
    return node_init_declarator;
}

jacc_init_declarator_list_ast_t* jacc_alloc_init_declarator_list(jacc_init_declarator_ast_t *init_declarator) {
    jacc_ast_node_t *node = jacc_alloc_node(JACC_INIT_DECLARATOR_LIST_AST);
    jacc_init_declarator_list_ast_t *node_init_declarator_list = &node->init_declarator_list;

    unsigned *n_init_declarators = malloc(sizeof(unsigned));

    node_init_declarator_list->n_init_declarators = n_init_declarators;
    node_init_declarator_list->list_head = init_declarator;

    (*(node_init_declarator_list->n_init_declarators))++;
    
    return node_init_declarator_list;
}

jacc_declaration_ast_t* jacc_alloc_declaration(jacc_declaration_spec_ast_t *declaration_spec,
                                               jacc_init_declarator_list_ast_t *init_declarator_list) {

    jacc_ast_node_t *node = jacc_alloc_node(JACC_DECLARATION_AST);
    jacc_declaration_ast_t *node_declaration = &node->declaration;

    node_declaration->declaration_spec = declaration_spec;
    node_declaration->init_declarator_list = init_declarator_list;
    
    return node_declaration;
}


jacc_init_declarator_list_ast_t* jacc_append_init_declarator_list(jacc_init_declarator_list_ast_t *init_declarator_list,
                                                                  jacc_init_declarator_ast_t *init_declarator) {
    (*init_declarator_list->n_init_declarators)++;                                        

    jacc_init_declarator_ast_t **runner = &(init_declarator_list->list_head);

    // iterate from head to the most recent one and append
    while (*runner != NULL) {
        runner = &((*runner)->next_init_declarator);
        // Could assert that sizes match up or look based on n_type_quals
        // right now I only have it for debugging purposes
    }

    // runner is now next_type_qual
    (*runner) = init_declarator;
    return init_declarator_list;
}

/*
jacc_pointer_ast_t *jacc-append_pointer(jacc_type_qual_ast_t *type_qual_list,
                                        jacc_pointer_ast_t *pointer) {

    (*type_qual_list->n_type_quals)++;                                        

    jacc_type_qual_ast_t **runner = &(type_qual_list->list_head);
}
    */

jacc_type_qual_list_ast_t* jacc_append_type_qual_list(jacc_type_qual_list_ast_t *type_qual_list,
                                                      jacc_type_qual_ast_t *type_qual) {
    (*type_qual_list->n_type_quals)++;                                        

    jacc_type_qual_ast_t **runner = &(type_qual_list->list_head);

    // iterate from head to the most recent one and append
    while (*runner != NULL) {
        runner = &((*runner)->next_type_qual);
        // Could assert that sizes match up or look based on n_type_quals
        // right now I only have it for debugging purposes
    }

    // runner is now next_type_qual
    (*runner) = type_qual;
    return type_qual_list;
}

jacc_spec_qual_list_ast_t* jacc_append_spec_qual_list(jacc_spec_qual_list_ast_t *spec_qual_list,
                                                      jacc_type_spec_ast_t *type_spec,
                                                      jacc_type_qual_ast_t *type_qual) {
    (*spec_qual_list->n_spec_quals)++;

    jacc_spec_qual_ast_t **runner = &(spec_qual_list->list_head);

    while (*runner != NULL) {
        runner = &((*runner)->next_spec_qual);
    }

    // check for malloc errors
    jacc_spec_qual_ast_t *spec_qual = calloc(1, sizeof(jacc_spec_qual_ast_t));
    spec_qual->type_spec = type_spec;
    spec_qual->type_qual = type_qual;

    (*runner) = spec_qual;

    return spec_qual_list;
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
