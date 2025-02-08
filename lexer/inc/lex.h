#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens-manual.h"

#define MAX_READABLE_ASCII 127
#define MIN_READABLE_ASCII 32

#define MAX_CHAR_BYTE_SIZE 255

#define STRING_TOK_IDX(IDX) (IDX - 256)

typedef union {
    char *string_literal;
    char char_literal;
    int integer;
} YYSTYPE;

extern YYSTYPE yylval;

extern const char *string_tokens[];

#define LEX_SINGLE_TOK(YYTEXT) \
                printf("%s\t%d\t%s\n", filename, line_ct, YYTEXT); \
                yylval.string_literal = strdup(YYTEXT); \
                return (int)(*YYTEXT);

#define LEX_MULTI_TOK(OP) \
                printf("%s\t%d\t%s\n", filename, line_ct, string_tokens[STRING_TOK_IDX(OP)]); \
                yylval.string_literal = strdup(yytext); \
                return OP;

int lex_append_str(size_t *size, char **a, char **b);

char lex_handle_esc(const char *s);