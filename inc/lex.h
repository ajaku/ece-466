#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "types.h"
#include "jacc_parser.tab.h"

#define STRING_TOK_IDX(IDX) (IDX - 256)

extern const char *string_tokens[];

#define LEX_SINGLE_TOK(YYTEXT)                      \
    yylval.tok.data_type = JACC_TYPE_STRING;    \
    yylval.tok.data.string_d = strdup(YYTEXT);  \
    return (int)(*YYTEXT);

#define LEX_MULTI_TOK(OP, YYTEXT)                   \
    yylval.tok.data_type = JACC_TYPE_STRING;    \
    yylval.tok.data.string_d = strdup(YYTEXT);  \
    return OP;

int lex_append_str(size_t *size, char **a, char **b);
int lex_append_char(size_t *size, char **a, char b);
int lex_handle_integers(char *yytext, jacc_lex_tok_t *token);
char *lex_return_str_esc(char esc);
char lex_handle_esc_char(char *s);
char lex_handle_esc_str(char *s);
jacc_lex_tok_t lex_alloc_int(int val);
