#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "types.h"

typedef enum jacc_ast_type {
    JACC_IDENT_AST,         // Base Operators
    JACC_CONST_AST,         // Base Operators
    JACC_STRING_AST,        // Base Operators
    JACC_CHAR_AST,          // Base Operators
    JACC_FUNC_AST,
    JACC_ARGS_AST,
    JACC_UN_OP_AST,
    JACC_BIN_OP_AST,
    JACC_TERNARY_AST,
    JACC_STG_CLS_AST,
    JACC_TYPE_SPEC_AST,
    JACC_TYPE_QUAL_AST,
    JACC_TYPE_QUAL_LIST_AST,
    JACC_DECLARATION_SPEC_AST,
    JACC_STRUCT_OR_UNION_SPEC_AST,
    //
    JACC_SPEC_QUAL_AST,
    JACC_SPEC_QUAL_LIST_AST,
    JACC_DECLARATOR_AST,
    JACC_DIRECT_DECLARATOR_AST,
    JACC_POINTER_AST,
    JACC_INIT_DECLARATOR_AST,
    JACC_INIT_DECLARATOR_LIST_AST,
    JACC_DECLARATION_AST
} jacc_ast_type_t;

typedef enum jacc_un_op_type {
    JACC_UN_OP_STD,
    JACC_UN_OP_DEREF,
    JACC_UN_OP_SIZEOF,
    JACC_UN_OP_ADDROF,
    JACC_UN_OP_POSTINC,
    JACC_UN_OP_POSTSUB
} jacc_un_op_type_t;

typedef enum jacc_bin_op_type {
    JACC_BIN_OP_STD,
    JACC_BIN_OP_ASSIGN,
    JACC_BIN_OP_ASSIGN_COMP,
    JACC_BIN_OP_COMPARE,
    JACC_BIN_OP_LOGICAL,
    JACC_BIN_OP_SELECT,
    JACC_BIN_OP_SELECT_INDIR
} jacc_bin_op_type_t;

typedef enum jacc_stg_cls_type {
    JACC_STG_CLS_TYPEDEF,
    JACC_STG_CLS_EXTERN,
    JACC_STG_CLS_STATIC,
    JACC_STG_CLS_AUTO,
    JACC_STG_CLS_REGISTER
} jacc_stg_cls_type_t;

typedef enum jacc_type_spec_type {
    JACC_TYPE_SPEC_VOID,
    JACC_TYPE_SPEC_CHAR,
    JACC_TYPE_SPEC_SHORT,
    JACC_TYPE_SPEC_INT,
    JACC_TYPE_SPEC_LONG,
    JACC_TYPE_SPEC_FLOAT,
    JACC_TYPE_SPEC_DOUBLE,
    JACC_TYPE_SPEC_SIGNED,
    JACC_TYPE_SPEC_UNSIGNED,
    JACC_TYPE_SPEC__BOOL,
    JACC_TYPE_SPEC__COMPLEX,
    JACC_TYPE_SPEC_STRUCT_OR_UNION,
    JACC_TYPE_SPEC_ENUM
} jacc_type_spec_type_t;

typedef enum jacc_type_qual_type {
    JACC_TYPE_QUAL_CONST,
    JACC_TYPE_QUAL_RESTRICT,
    JACC_TYPE_QUAL_VOLATILE
} jacc_type_qual_type_t;

typedef enum jacc_struct_or_union_spec_type {
    JACC_STRUCT_SPEC,
    JACC_UNION_SPEC
} jacc_struct_or_union_spec_type_t;

typedef struct jacc_func_ast {
    struct jacc_ast_node *func;
    struct jacc_ast_node *args;
} jacc_func_ast_t;

typedef struct jacc_args_ast {
    int n_args;
    struct jacc_ast_node *arg;
    struct jacc_ast_node *next_arg;
} jacc_args_ast_t;

typedef struct jacc_un_op_ast {
    jacc_un_op_type_t u_type;
    int operator;
    struct jacc_ast_node *operand;
} jacc_un_op_ast_t;

typedef struct jacc_bin_op_ast {
    jacc_bin_op_type_t b_type;
    int operator;
    struct jacc_ast_node *left;
    struct jacc_ast_node *right;
} jacc_bin_op_ast_t;

typedef struct jacc_ternary_ast {
    struct jacc_ast_node *cond;
    struct jacc_ast_node *t_op;
    struct jacc_ast_node *f_op;
} jacc_ternary_ast_t;

// Non-basic

typedef struct jacc_stg_cls_spec_ast {
    jacc_stg_cls_type_t type;
} jacc_stg_cls_spec_ast_t;

typedef struct jacc_type_spec_ast {
    jacc_type_spec_type_t type;
    /*
    union {
        struct jacc_struct_or_union_spec_ast *struct_or_union_spec;
        struct jacc_enum_spec_ast *enum_spec;
    };
    */
    struct jacc_struct_or_union_spec_ast *struct_or_union_spec;
} jacc_type_spec_ast_t;

typedef struct jacc_type_qual_ast {
    jacc_type_qual_type_t type;
    struct jacc_type_qual_ast *next_type_qual;
} jacc_type_qual_ast_t;

typedef struct jacc_type_qual_list_ast {
    unsigned *n_type_quals;
    struct jacc_type_qual_ast *list_head;
} jacc_type_qual_list_ast_t;

typedef struct jacc_declaration_spec_ast {
    struct jacc_stg_cls_spec_ast *stg_cls_spec;
    struct jacc_declaration_spec_ast *declaration_spec;
    struct jacc_type_spec_ast *type_spec;
} jacc_declaration_spec_ast_t;

typedef struct jacc_struct_or_union_spec_ast {
    jacc_struct_or_union_spec_type_t type;
    struct jacc_ast_node *ident;
    struct jacc_struct_declaration_ast *struct_declaration_list;
} jacc_struct_or_union_spec_ast_t;

/* Deviating a bit from the standard here
- Rather than having a struct_declaration_list and a seperate struct_declaration,
I'll have a struct_declaration that is a list, like the args_ast earlier
*/

// Each struct_declaration has its own qualifiers
typedef struct jacc_struct_declaration_ast {
    unsigned *n_struct_declarations;
    struct jacc_spec_qual_list_ast *spec_qual_list; // fix in others
    struct jacc_struct_declaration_ast *cur_struct_declaration;
    struct jacc_struct_declaration_ast *next_struct_declaration;
} jacc_struct_declaration_ast_t;

// Slightly deviating
typedef struct jacc_spec_qual_ast {
    // might want to convert to union
    struct jacc_type_spec_ast *type_spec; // should never have both at same time (could be union)
    struct jacc_type_qual_ast *type_qual; // should never have both at same time (could be union)
    struct jacc_spec_qual_ast *next_spec_qual;
} jacc_spec_qual_ast_t;

typedef struct jacc_spec_qual_list_ast {
    unsigned *n_spec_quals;
    struct jacc_spec_qual_ast *list_head;
} jacc_spec_qual_list_ast_t;

// declarators now
typedef struct jacc_declarator_ast {
    struct jacc_pointer_ast *pointer;
    struct jacc_direct_declarator_ast *direct_declarator;
} jacc_declarator_ast_t;

typedef struct jacc_direct_declarator_ast {
    struct jacc_ast_node *ident;
    struct jacc_declarator_ast *declarator;
    struct jacc_direct_declarator_ast *direct_declarator;
    struct jacc_ast_node *number;
} jacc_direct_declarator_ast_t;

typedef struct jacc_pointer_ast {
    struct jacc_type_qual_list_ast *type_qual_list;
    struct jaccc_pointer_ast *pointer;
} jacc_pointer_ast_t;

typedef struct jacc_init_declarator_ast {
    struct jacc_declarator_ast *declarator;
    // struct jacc_initalizer_ast initializer;
    struct jacc_init_declarator_ast *next_init_declarator;
} jacc_init_declarator_ast_t;

typedef struct jacc_init_declarator_list_ast {
    unsigned *n_init_declarators;
    struct jacc_init_declarator_ast *list_head;
} jacc_init_declarator_list_ast_t;

typedef struct jacc_declaration_ast {
    struct jacc_declaration_specifier_ast *declaration_spec;
    struct jacc_init_declarator_list *init_declarator_list;
} jacc_declaration_ast_t;

typedef struct jacc_ast_node {
    jacc_ast_type_t ast_type;

    union {
        jacc_lex_tok_t                      lex;
        jacc_func_ast_t                     func;
        jacc_args_ast_t                     args;
        jacc_un_op_ast_t                    unop;
        jacc_bin_op_ast_t                   binop;
        jacc_ternary_ast_t                  tern;
        jacc_stg_cls_spec_ast_t             stg_cls_spec;
        jacc_type_spec_ast_t                type_spec;
        jacc_type_qual_ast_t                type_qual;
        jacc_type_qual_list_ast_t           type_qual_list;
        jacc_declaration_spec_ast_t         declaration_spec;
        jacc_struct_or_union_spec_ast_t     struct_or_union_spec;
        //
        jacc_spec_qual_list_ast_t           spec_qual_list;
        //
        jacc_declarator_ast_t               declarator;
        jacc_direct_declarator_ast_t        direct_declarator;
        jacc_pointer_ast_t                  pointer;
        jacc_init_declarator_ast_t          init_declarator;
        jacc_init_declarator_list_ast_t     init_declarator_list;
        jacc_declaration_ast_t              declaration;
    };

} jacc_ast_node_t;

jacc_ast_node_t* jacc_alloc_base(jacc_ast_type_t type,
                                 jacc_lex_tok_t lex);

jacc_ast_node_t* jacc_alloc_func(jacc_ast_node_t *func,
                                 jacc_ast_node_t *args);

jacc_ast_node_t* jacc_alloc_args(jacc_ast_node_t *arg);

jacc_ast_node_t* jacc_alloc_unop(jacc_un_op_type_t u_type,
                                 int operator,
                                 jacc_ast_node_t *operand);

jacc_ast_node_t* jacc_alloc_binop(jacc_bin_op_type_t b_type,
                                  int operator,
                                  jacc_ast_node_t *left,
                                  jacc_ast_node_t *right);

jacc_ast_node_t* jacc_alloc_ternary(jacc_ast_node_t *cond, 
                                    jacc_ast_node_t *t_op,
                                    jacc_ast_node_t *f_op);

jacc_stg_cls_spec_ast_t* jacc_alloc_stg_cls_spec(jacc_stg_cls_type_t type);

jacc_type_spec_ast_t* jacc_alloc_type_spec(jacc_type_spec_type_t type,
                                      jacc_ast_node_t *spec);

jacc_type_qual_ast_t* jacc_alloc_type_qual(jacc_type_qual_type_t type);

jacc_type_qual_list_ast_t* jacc_alloc_type_qual_list(jacc_type_qual_ast_t *list_head);

jacc_declaration_spec_ast_t* jacc_alloc_declaration_spec(jacc_stg_cls_spec_ast_t *stg_cls_spec, 
                                                         jacc_declaration_spec_ast_t *declaration_spec,
                                                         jacc_type_spec_ast_t *type_spec);

jacc_struct_or_union_spec_ast_t* jacc_alloc_struct_or_union_spec(jacc_struct_or_union_spec_type_t type,
                                                                 jacc_ast_node_t *ident,
                                                                 jacc_struct_declaration_ast_t *struct_declaration_list);

jacc_spec_qual_list_ast_t* jacc_alloc_spec_qual_list(jacc_type_spec_ast_t *type_spec,
                                                     jacc_type_qual_ast_t *type_qual);

jacc_declarator_ast_t* jacc_alloc_declarator(jacc_pointer_ast_t *pointer,
                                             jacc_direct_declarator_ast_t *direct_declarator);

jacc_direct_declarator_ast_t* jacc_alloc_direct_declarator(jacc_ast_node_t *ident,
                                                           jacc_declarator_ast_t *declarator,
                                                           jacc_direct_declarator_ast_t *direct_declarator,
                                                           jacc_ast_node_t *number);

jacc_pointer_ast_t* jacc_alloc_pointer(jacc_type_qual_list_ast_t *type_qual_list);

jacc_init_declarator_ast_t* jacc_alloc_init_declarator(jacc_declarator_ast_t *declarator);

jacc_init_declarator_list_ast_t* jacc_alloc_init_declarator_list(jacc_init_declarator_ast_t *init_declarator);

jacc_declaration_ast_t* jacc_alloc_declaration(jacc_declaration_spec_ast_t *declaration_spec,
                                               jacc_init_declarator_list_ast_t *init_declarator_list);

jacc_init_declarator_list_ast_t* jacc_append_init_declarator_list(jacc_init_declarator_list_ast_t *init_declarator_list,
                                                                  jacc_init_declarator_ast_t *init_declarator);

jacc_pointer_ast_t* jacc_append_pointer(jacc_type_qual_ast_t *type_qual_list, 
                                        jacc_pointer_ast_t *pointer);

jacc_type_qual_list_ast_t* jacc_append_type_qual_list(jacc_type_qual_list_ast_t *type_qual_list,
                                                      jacc_type_qual_ast_t *type_qual);

jacc_spec_qual_list_ast_t* jacc_append_spec_qual_list(jacc_spec_qual_list_ast_t *spec_qual_list,
                                                      jacc_type_spec_ast_t *type_spec,
                                                      jacc_type_qual_ast_t *type_qual);

jacc_ast_node_t* jacc_append_arg(jacc_ast_node_t *arg_list,
                                 jacc_ast_node_t *next_arg);

void jacc_print_ast(int *ident, jacc_ast_node_t *ast);
