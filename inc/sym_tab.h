#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_HASH_SIZE 101

typedef enum jacc_scope_type {
    JACC_FILE_SCOPE,
    JACC_BLOCK_SCOPE,
    JACC_FUNCTION_SCOPE,
    JACC_PROTOTYPE_SCOPE
} jacc_scope_type_t;

typedef enum jacc_namespace_type {
    JACC_DEFAULT_NS,
    JACC_LABEL_NS,
    JACC_TAG_NS,
    JACC_STRUCT_OR_UNION_NS
} jacc_namespace_type_t;

typedef struct jacc_sym {
    char *sym_name;
    char *sym_type;
    char *sym_file_name;
    int sym_line_num;

    // Required for hash table collissions
    struct jacc_sym *next_sym;
} jacc_sym_t;

typedef struct jacc_sym_list {
    jacc_namespace_type_t list_namespace_type;  
    char *list_namespace;
    int list_size;

    unsigned int hash_table_size;
    unsigned int hash_table_capacity;
    struct jacc_sym **hash_table;
} jacc_sym_list_t;

typedef struct jacc_sym_tab {
    jacc_scope_type_t sym_tab_scope_type;

    // pointer to next symbol table on stack
    struct jacc_sym_tab *next_sym_tab;

    // array of symbol lists for different namespaces
    struct jacc_sym_list *list_arr;
} jacc_sym_tab_t;

int jacc_hash_table_create(jacc_sym_list_t *list);
int jacc_hash_table_destroy(jacc_sym_list_t *list);
jacc_sym_t* jacc_hash_table_lookup(jacc_sym_list_t *list, const char *str);
int jacc_hash_table_enter(jacc_sym_list_t *list, jacc_sym_t *sym);
int jacc_rehash_table(jacc_sym_list_t *list);