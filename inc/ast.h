#pragma once

struct astnode_binop;
struct astnode_num;
struct astnode_ident;

struct astnode {
    int node_type;

    union astnodes {
        struct astnode_binop {
            int node_type;
            int operator;
            struct astnode *left, *right;
        } binop;
        struct astnode_num {
            int node_type;
            int number;
        } num;
        struct astnode_ident {
            int node_type;
            char *ident;
        } ident;
    } u;
};
