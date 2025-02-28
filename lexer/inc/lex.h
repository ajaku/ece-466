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

typedef enum {
	JACC_TYPE_CHARLIT,
	JACC_TYPE_CHAR_ESC,
	JACC_TYPE_STRING,
	JACC_TYPE_INT,
	JACC_TYPE_UINT,
	JACC_TYPE_LONG,
	JACC_TYPE_ULONG,
	JACC_TYPE_ULONGLONG,
    // Not supported ATM
	JACC_TYPE_FLOAT,
	JACC_TYPE_DOUBLE,
	JACC_TYPE_LDOUBLE
} jacc_data_type_t;


typedef union {
    char char_literal;
    char *string_literal;

    /* Integer Numbers */
    int int_type;
    unsigned int uint_type;
    long long_type;
    unsigned long ulong_type;
    unsigned long long ulonglong_type;

    // Not supported ATM
    /* Real Numbers */
    float float_type;
    double double_type;
    long double ldouble_type;

} jacc_stored_data_t;

typedef struct {
    jacc_data_type_t    type;
    jacc_stored_data_t  data;
    size_t             size;
} jacc_yystype_t;

typedef union {
    jacc_yystype_t token;
} YYSTYPE;

//extern YYSTYPE yylval;

extern const char *string_tokens[];

#define LEX_SINGLE_TOK(YYTEXT) \
                printf("%s\t%d\t%s\n", filename, line_ct, YYTEXT); \
                yylval.token.type = JACC_TYPE_STRING; \
                yylval.token.data.string_literal = strdup(YYTEXT); \
                return (int)(*YYTEXT);

#define LEX_MULTI_TOK(OP) \
                yylval.token.type = JACC_TYPE_STRING; \
                yylval.token.data.string_literal = strdup(yytext); \
                return OP;

int lex_append_str(size_t *size, char **a, char **b);
int lex_append_char(size_t *size, char **a, char b);
int lex_handle_integers(char *yytext, YYSTYPE *yylval);
char *lex_return_str_esc(char esc);
char lex_handle_esc_char(char *s);
char lex_handle_esc_str(char *s);