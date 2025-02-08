%option noyywrap
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lex.h"

int line_ct;
char *filename = NULL;
char *str = NULL;
size_t str_size = 0;
YYSTYPE yylval;

%}

%x PREPROC
%x CHARLIT
%x STRING

%%
#[" "]+[0]+[" "]\"                      { BEGIN(PREPROC); }
<PREPROC>([^\"\n]+)                     { if (filename == NULL) { filename = strdup(yytext); } }
<PREPROC>\"$                            { BEGIN(INITIAL); }

[\n]                                    { line_ct++; }

!|%|^|&|\*|-|\+|=|~|\||\.|<|>|\/|\?|;   { LEX_SINGLE_TOK(yytext); }
\[|\]                                   { LEX_SINGLE_TOK(yytext); }
\{|\}                                   { LEX_SINGLE_TOK(yytext); }
\(|\)                                   { LEX_SINGLE_TOK(yytext); }
->                                      { LEX_MULTI_TOK(INDSEL); }
\+\+                                    { LEX_MULTI_TOK(PLUSPLUS); }
--                                      { LEX_MULTI_TOK(MINUSMINUS); } 
\<\<                                    { LEX_MULTI_TOK(SHL); }
>>                                      { LEX_MULTI_TOK(SHR); }
\<=                                     { LEX_MULTI_TOK(LTEQ); }
>=                                      { LEX_MULTI_TOK(GTEQ); }
==                                      { LEX_MULTI_TOK(EQEQ); }
!=                                      { LEX_MULTI_TOK(NOTEQ); }
&&                                      { LEX_MULTI_TOK(LOGAND); }
\|\|                                    { LEX_MULTI_TOK(LOGOR); }
\.\.\.                                  { LEX_MULTI_TOK(ELLIPSIS); }
\*=                                     { LEX_MULTI_TOK(TIMESEQ); }
\/=                                     { LEX_MULTI_TOK(DIVEQ); }
\%=                                     { LEX_MULTI_TOK(MODEQ); }
\+=                                     { LEX_MULTI_TOK(PLUSEQ); }
-=                                      { LEX_MULTI_TOK(MINUSEQ); }
\<\<=                                   { LEX_MULTI_TOK(SHLEQ); }
>>=                                     { LEX_MULTI_TOK(SHREQ); }
&=                                      { LEX_MULTI_TOK(ANDEQ); }
\|=                                     { LEX_MULTI_TOK(OREQ); }
\^=                                     { LEX_MULTI_TOK(XOREQ); }

AUTO                                    { LEX_MULTI_TOK(AUTO); }
BREAK                                   { LEX_MULTI_TOK(BREAK); }
CASE                                    { LEX_MULTI_TOK(CASE); }
CHAR                                    { LEX_MULTI_TOK(CHAR); }
CONST                                   { LEX_MULTI_TOK(CONST); }
CONTINUE                                { LEX_MULTI_TOK(CONTINUE); }
DEFAULT                                 { LEX_MULTI_TOK(DEFAULT); }
DO                                      { LEX_MULTI_TOK(DO); }
DOUBLE                                  { LEX_MULTI_TOK(DOUBLE); }
ELSE                                    { LEX_MULTI_TOK(ELSE); }
ENUM                                    { LEX_MULTI_TOK(ENUM); }
EXTERN                                  { LEX_MULTI_TOK(EXTERN); }
FLOAT                                   { LEX_MULTI_TOK(FLOAT); }
FOR                                     { LEX_MULTI_TOK(FOR); }
GOTO                                    { LEX_MULTI_TOK(GOTO); }
IF                                      { LEX_MULTI_TOK(IF); }
INLINE                                  { LEX_MULTI_TOK(INLINE); }
INT                                     { LEX_MULTI_TOK(INT); }
LONG                                    { LEX_MULTI_TOK(LONG); }
REGISTER                                { LEX_MULTI_TOK(REGISTER); }
RESTRICT                                { LEX_MULTI_TOK(RESTRICT); }
RETURN                                  { LEX_MULTI_TOK(RETURN); }
SHORT                                   { LEX_MULTI_TOK(SHORT); }
SIGNED                                  { LEX_MULTI_TOK(SIGNED); }
SIZEOF                                  { LEX_MULTI_TOK(SIZEOF); }
STATIC                                  { LEX_MULTI_TOK(STATIC); }
STRUCT                                  { LEX_MULTI_TOK(STRUCT); }
SWITCH                                  { LEX_MULTI_TOK(SWITCH); }
TYPEDEF                                 { LEX_MULTI_TOK(TYPEDEF); }
UNION                                   { LEX_MULTI_TOK(UNION); }
UNSIGNED                                { LEX_MULTI_TOK(UNSIGNED); }
VOID                                    { LEX_MULTI_TOK(VOID); }
VOLATILE                                { LEX_MULTI_TOK(VOLATILE); }
WHILE                                   { LEX_MULTI_TOK(WHILE); }
_BOOL                                   { LEX_MULTI_TOK(_BOOL); }
_COMPLEX                                { LEX_MULTI_TOK(_COMPLEX); }
_IMAGINARY                              { LEX_MULTI_TOK(_IMAGINARY); }

[0-9]+                                  { 
                                            yylval.int_type = atoi(yytext);
                                            printf("%s %d\tNUMBER\tINTEGER\t%d\tINT\n", filename, line_ct, yylval.int_type); 
                                            return NUMBER;
                                        }
[0-9]+U$                                { 
                                            yylval.uint_type = (unsigned int) atol(yytext);
                                            lex_handle_nums(yytext, &yylval, filename, line_ct);
                                            printf("%s %d\tNUMBER\tINTEGER\t%u\tLONG\n", filename, line_ct, yylval.uint_type); 
                                            return NUMBER;
                                        }
[0-9]+L$                                { 
                                            yylval.long_type = atol(yytext);
                                            printf("%s %d\tNUMBER\tINTEGER\t%ld\tLONG\n", filename, line_ct, yylval.long_type); 
                                            return NUMBER;
                                        }
[0-9]+(UL|LU)$                          { 
                                            yylval.ulong_type = strtoul(yytext, NULL, 10);
                                            lex_handle_nums(yytext, &yylval, filename, line_ct);
                                            printf("%s %d\tNUMBER\tINTEGER\t%lu\tUNSIGNED, LONG\n", filename, line_ct, yylval.ulong_type); 
                                            return NUMBER;
                                        }
[0-9]+(LLU|ULL|LUL)$                    { 
                                            yylval.ulonglong_type = strtoull(yytext, NULL, 10);
                                            printf("%s %d\tNUMBER\tINTEGER\t%llu\tUNSIGNED, LONGLONG\n", filename, line_ct, yylval.ulonglong_type); 
                                            return NUMBER;
                                        }
0[xX][0-9a-fA-F]+                       { 
                                            sscanf(yytext, "%x", &yylval.int_type);
                                            printf("%s %d\tNUMBER\tINTEGER\t%d\n", filename, line_ct, yylval.int_type); 
                                            return NUMBER;
                                        }
0[xX][0-9a-fA-F]+(L)$                   { 
                                            yytext[strlen(yytext) - 1] = '\0';
                                            sscanf(yytext, "%lx", &yylval.long_type);
                                            printf("%s %d\tNUMBER\tINTEGER\t%ld\tLONG\n", filename, line_ct, yylval.long_type); 
                                            return NUMBER;
                                        }
0[xX][0-9a-fA-F]+(U)$                   { 
                                            yytext[strlen(yytext) - 1] = '\0';
                                            sscanf(yytext, "%x", &yylval.uint_type);
                                            printf("%s %d\tNUMBER\tINTEGER\t%u\tUNSIGNED\n", filename, line_ct, yylval.uint_type); 
                                            return NUMBER;
                                        }
0[xX][0-9a-fA-F]+(UL|LU)$                   { 
                                            yytext[strlen(yytext) - 2] = '\0';
                                            sscanf(yytext, "%lx", &yylval.ulong_type);
                                            printf("%s %d\tNUMBER\tINTEGER\t%lu\tUNSIGNED, LONG\n", filename, line_ct, yylval.ulong_type); 
                                            return NUMBER;
                                        }
0[xX][0-9a-fA-F]+(ULL|LUL|LLU)$         { 
                                            yytext[strlen(yytext) - 3] = '\0';
                                            sscanf(yytext, "%llx", &yylval.ulonglong_type);
                                            printf("%s %d\tNUMBER\tINTEGER\t%llu\tUNSIGNED, LONGLONG\n", filename, line_ct, yylval.ulonglong_type); 
                                            return NUMBER;
                                        }
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)         {
                                            yylval.double_type = strtod(yytext, NULL);
                                            printf("%s %d\tNUMBER\tREAL\t%s\tDOUBLE\n", filename, line_ct, yytext); 
                                            return NUMBER;
                                        }
([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)(L)$     {
                                            yylval.ldouble_type = strtold(yytext, NULL);
                                            printf("%s %d\tNUMBER\tREAL\t%s\tDOUBLE, LONG\n", filename, line_ct, yytext); 
                                            return NUMBER;
                                        }

^(L\')|(\')                             { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'            { 
                                            yylval.char_literal = lex_handle_esc(yytext + 1);
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, line_ct, yytext); 
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n]'                        { 
                                            yylval.char_literal = yytext[0];
                                            printf("%s %d\tCHARLIT\t%c\n", filename, line_ct, yylval.char_literal); 
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n][^']+                    { 
                                            yylval.char_literal = yytext[0];
                                            printf("%s %d\tCHARLIT\t%c\n", filename, line_ct, yylval.char_literal); 
                                            fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+        { 
                                            char *temp = (char *)malloc(sizeof(char *));
                                            temp[0] = yytext[1];
                                            yylval.char_literal = lex_handle_esc(temp);
                                            free(temp);
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, line_ct, yytext); 
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }

^(L\")|(\")                             {  
                                            BEGIN(STRING);
                                            str_size = 0;
                                            str = (char *)malloc(sizeof(char *));
                                            printf("%s\t%d\tSTRING\t", filename, line_ct);
                                        }
<STRING>.                               { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            printf("%s", yytext);
                                        }
<STRING>\\[xX][0-9a-fA-F]+              { 
                                            lex_append_str(&str_size, &str, &yytext);

                                            unsigned int dec_rep;
                                            sscanf(yytext + 2, "%x", &dec_rep);

                                            if (dec_rep < MAX_READABLE_ASCII && dec_rep  > MIN_READABLE_ASCII) {
                                                printf("%c", dec_rep);
                                            } else {
                                                // Hex is getting stored in a char, which is max 1 byte (8 bits => 377 octal => 255 dec)
                                                if (dec_rep > UCHAR_MAX)
                                                {
                                                    dec_rep = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }
                                                printf("\\%03o", dec_rep);
                                            }
                                        }
<STRING>\\[0-7]{1,3}                    { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            size_t yytext_size = strlen(yytext);
                                            if (yytext_size == 2 && yytext[1] == '0') {
                                                printf("\\0"); 
                                            }
                                            else if (yytext_size == 2) {
                                                printf("\\00%d", 
                                                        yytext[1] - '0');
                                            }
                                            else if (yytext_size == 3) {
                                                printf("\\0%d%d",
                                                        yytext[1] - '0',
                                                        yytext[2] - '0');
                                            }
                                            else if (yytext_size == 4) {
                                                printf("\\%d%d%d", 
                                                        yytext[1] - '0',
                                                        yytext[2] - '0',
                                                        yytext[3] - '0');
                                            }
                                        }
<STRING>\\\?                            { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            printf("\?"); 
                                        }
<STRING>\"$                             { 
                                            yylval.string_literal = str;
                                            printf("\n");
                                            BEGIN(INITIAL);
                                            return STRING;
                                        }


[_A-Za-z]+[_A-Za-z0-9]*                { printf("IDENT: %s\n", yytext); }
.                                      { printf("unrecognized %s\n", yytext); }

%%

int main (int argc, char **argv) {
    ++argv, --argc;

    if( argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }

    int  t;
    while (t = yylex()) {
       switch(t) {
       } 
    }
}
