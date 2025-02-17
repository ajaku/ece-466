#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#include "jacc_parser.tab.h"


#define MAX_READABLE_ASCII 127
#define MIN_READABLE_ASCII 32

#define STRING_TOK_IDX(IDX) (IDX - 256)

extern const char *string_tokens[];

#define LEX_SINGLE_TOK(YYTEXT) \
                yylval.token.type = STRING; \
                yylval.token.data.string_literal = strdup(YYTEXT); \
                return (int)(*YYTEXT);

#define LEX_MULTI_TOK(OP) \
                yylval.token.type = STRING; \
                yylval.token.data.string_literal = strdup(yytext); \
                return OP;

int lex_append_str(size_t *size, char **a, char **b);
int lex_append_char(size_t *size, char **a, char b);
int lex_handle_integers(char *yytext, YYSTYPE *yylval);
char *lex_return_str_esc(char esc);
char lex_handle_esc_char(char *s);
char lex_handle_esc_str(char *s);