#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens-manual.h"

#define MAX_READABLE_ASCII 127
#define MIN_READABLE_ASCII 32

#define STRING_TOK_IDX(IDX) (IDX - 256)

/* Don't support real numbers yet
I am confused as to why:

0x100p-7 -> 2 (double)
017e+8 -> 1.7e09 (double)

but

0x100e-2 -> 4110 (int)


Have some lexer elements set up to support it in the future (moving on for now)
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)         {
                                            yylval.double_type = (double) strtold(yytext, NULL);
                                            printf("%s %d\tNUMBER\tREAL\t%Lg\tDOUBLE\n", filename, line_ct, yylval.double_type); 
                                            return NUMBER;
                                        }
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)(L)$     {
                                            yylval.ldouble_type = strtold(yytext, NULL);
                                            printf("%s %d\tNUMBER\tREAL\t%s\tDOUBLE, LONG\n", filename, line_ct, yytext); 
                                            return NUMBER;
                                        }
(0[xX][0-9a-fA-F]+)([pe][+-]?[0-9]+)+(L)?   {}

*/

typedef union {
    char *string_literal;
    char char_literal;

    /* Integer Numbers */
    int int_type;
    unsigned int uint_type;
    long long_type;
    unsigned long ulong_type;
    unsigned long long ulonglong_type;

    /* Real Numbers */
    float float_type;
    double double_type;
    long double ldouble_type;
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

void lex_handle_integers(const char *yytext, YYSTYPE *yylval, const char *filename, int line_ct);