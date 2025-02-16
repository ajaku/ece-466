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
YYSTYPE yylval;

%}

NUM_EXTENSIONS              ((U|L|UL|LU|ULL|LUL|LLU)?)

%x CHARLIT
%x MULTIBYTE
%x STRING

%%
#.*                                         {
                                                if (filename == NULL) {
                                                    filename = (char *)malloc(sizeof(char *) * strlen(yytext) - 6);
                                                    strncpy(filename, yytext + 5, strlen(yytext) - 6);
                                                } 
                                                line_ct--;
                                            }

[\n]                                        { line_ct++; }

!|%|^|&|\*|-|\+|=|~|\||\.|<|>|\/|\?|:|;|,   { LEX_SINGLE_TOK(yytext); }
\[|\]                                       { LEX_SINGLE_TOK(yytext); }
\{|\}                                       { LEX_SINGLE_TOK(yytext); }
\(|\)                                       { LEX_SINGLE_TOK(yytext); }
->                                          { LEX_MULTI_TOK(INDSEL); }
\+\+                                        { LEX_MULTI_TOK(PLUSPLUS); }
--                                          { LEX_MULTI_TOK(MINUSMINUS); } 
\<\<                                        { LEX_MULTI_TOK(SHL); }
>>                                          { LEX_MULTI_TOK(SHR); }
\<=                                         { LEX_MULTI_TOK(LTEQ); }
>=                                          { LEX_MULTI_TOK(GTEQ); }
==                                          { LEX_MULTI_TOK(EQEQ); }
!=                                          { LEX_MULTI_TOK(NOTEQ); }
&&                                          { LEX_MULTI_TOK(LOGAND); }
\|\|                                        { LEX_MULTI_TOK(LOGOR); }
\.\.\.                                      { LEX_MULTI_TOK(ELLIPSIS); }
\*=                                         { LEX_MULTI_TOK(TIMESEQ); }
\/=                                         { LEX_MULTI_TOK(DIVEQ); }
\%=                                         { LEX_MULTI_TOK(MODEQ); }
\+=                                         { LEX_MULTI_TOK(PLUSEQ); }
-=                                          { LEX_MULTI_TOK(MINUSEQ); }
\<\<=                                       { LEX_MULTI_TOK(SHLEQ); }
>>=                                         { LEX_MULTI_TOK(SHREQ); }
&=                                          { LEX_MULTI_TOK(ANDEQ); }
\|=                                         { LEX_MULTI_TOK(OREQ); }
\^=                                         { LEX_MULTI_TOK(XOREQ); }

auto                                        { LEX_MULTI_TOK(AUTO); }
break                                       { LEX_MULTI_TOK(BREAK); }
case                                        { LEX_MULTI_TOK(CASE); }
char                                        { LEX_MULTI_TOK(CHAR); }
const                                       { LEX_MULTI_TOK(CONST); }
continue                                    { LEX_MULTI_TOK(CONTINUE); }
default                                     { LEX_MULTI_TOK(DEFAULT); }
do                                          { LEX_MULTI_TOK(DO); }
double                                      { LEX_MULTI_TOK(DOUBLE); }
else                                        { LEX_MULTI_TOK(ELSE); }
enum                                        { LEX_MULTI_TOK(ENUM); }
extern                                      { LEX_MULTI_TOK(EXTERN); }
float                                       { LEX_MULTI_TOK(FLOAT); }
for                                         { LEX_MULTI_TOK(FOR); }
goto                                        { LEX_MULTI_TOK(GOTO); }
if                                          { LEX_MULTI_TOK(IF); }
inline                                      { LEX_MULTI_TOK(INLINE); }
int                                         { LEX_MULTI_TOK(INT); }
long                                        { LEX_MULTI_TOK(LONG); }
register                                    { LEX_MULTI_TOK(REGISTER); }
restrict                                    { LEX_MULTI_TOK(RESTRICT); }
return                                      { LEX_MULTI_TOK(RETURN); }
short                                       { LEX_MULTI_TOK(SHORT); }
signed                                      { LEX_MULTI_TOK(SIGNED); }
sizeof                                      { LEX_MULTI_TOK(SIZEOF); }
static                                      { LEX_MULTI_TOK(STATIC); }
struct                                      { LEX_MULTI_TOK(STRUCT); }
switch                                      { LEX_MULTI_TOK(SWITCH); }
typedef                                     { LEX_MULTI_TOK(TYPEDEF); }
union                                       { LEX_MULTI_TOK(UNION); }
unsigned                                    { LEX_MULTI_TOK(UNSIGNED); }
void                                        { LEX_MULTI_TOK(VOID); }
volatile                                    { LEX_MULTI_TOK(VOLATILE); }
while                                       { LEX_MULTI_TOK(WHILE); }
_Bool                                       { LEX_MULTI_TOK(_BOOL); }
_Complex                                    { LEX_MULTI_TOK(_COMPLEX); }
_Imaginary                                  { LEX_MULTI_TOK(_IMAGINARY); }

(0[0-7]*){NUM_EXTENSIONS}                   { 
                                                return lex_handle_integers(yytext, &yylval);
                                            }
([1-9][0-9]*){NUM_EXTENSIONS}               { 
                                                return lex_handle_integers(yytext, &yylval);
                                            }
(0[xX][0-9a-fA-F]+){NUM_EXTENSIONS}         { 
                                                return lex_handle_integers(yytext, &yylval);
                                            }
^(L\')|(\')                                 { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'                { 
                                                yylval.token.type = JACC_TYPE_CHAR_ESC;
                                                yylval.token.data.char_literal = lex_handle_esc(yytext + 1);
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHARLIT>[^\\]'                             { 
                                                yylval.token.type = JACC_TYPE_CHARLIT;
                                                yylval.token.data.char_literal = yytext[0];
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHARLIT>[^\\][^']+                         { 
                                                yylval.token.type = JACC_TYPE_CHARLIT;
                                                yylval.token.data.char_literal = yytext[0];
                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(MULTIBYTE);
                                                return CHARLIT;
                                            }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+            { 
                                                char *temp = (char *)malloc(sizeof(char *));
                                                temp[0] = yytext[1];
                                                yylval.token.type = JACC_TYPE_CHAR_ESC;
                                                yylval.token.data.char_literal = lex_handle_esc(temp);
                                                free(temp);
                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(MULTIBYTE);
                                                return CHARLIT;
                                            }
<MULTIBYTE>'$                               { BEGIN(INITIAL); } 

^(L\")|(\")                                 {  
                                                BEGIN(STRING);
                                                yylval.token.size = 0;
                                                yylval.token.data.string_literal = (char *)malloc(sizeof(char *));
                                            }
<STRING>.                                   { 
                                                lex_append_str(&yylval.token.size, &yylval.token.data.string_literal, &yytext);
                                            }
<STRING>\\[xX][0-9a-fA-F]+                  { 
                                                char *hex_str = (char *)malloc(sizeof(char *));
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 2, "%x", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                sprintf(hex_str, "\\%03o", hex_val);
                                                lex_append_str(&yylval.token.size, &yylval.token.data.string_literal, &hex_str);

                                            }
<STRING>\\[0-7]{1,3}                        { 
                                                char *hex_str = (char *)malloc(sizeof(char *));
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 2, "%o", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                sprintf(hex_str, "\\%03o", hex_val);
                                                lex_append_str(&yylval.token.size, &yylval.token.data.string_literal, &hex_str);
                                            }
<STRING>\\\?                                { 
                                                lex_append_str(&yylval.token.size, &yylval.token.data.string_literal, &yytext);
                                            }

<STRING>\\\0                                { 
                                                lex_append_str(&yylval.token.size, &yylval.token.data.string_literal, &yytext);
                                            }
<STRING>\"$                                 { 
                                                BEGIN(INITIAL);
                                                return STRING;
                                            }


[_A-Za-z]+[_A-Za-z0-9]*                     { printf("%s\t%d\tIDENT\t%s\n", filename, line_ct, yytext); return IDENT; }
.                                           { fprintf(stderr, "%s:%d:Error:Uncrecognized term %s\n", filename, line_ct, yytext); }
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
        switch (t) {
            case TOKEOF:
                break;

            case IDENT:
                break;

            case CHARLIT:
                char *esc_seq;
                if (yylval.token.type == JACC_TYPE_CHAR_ESC) {
                    if (yylval.token.data.char_literal == '\0') {
                        esc_seq = "\\0";
                    }
                    if (yylval.token.data.char_literal == '\a') {
                        esc_seq = "\\a";
                    }
                    if (yylval.token.data.char_literal == '\b') {
                        esc_seq = "\\b";
                    }
                    if (yylval.token.data.char_literal == '\f') {
                        esc_seq = "\\f";
                    }
                    if (yylval.token.data.char_literal == '\n') {
                        esc_seq = "\\n";
                    }
                    if (yylval.token.data.char_literal == '\r') {
                        esc_seq = "\\r";
                    }
                    if (yylval.token.data.char_literal == '\t') {
                        esc_seq = "\\t";
                    }
                    if (yylval.token.data.char_literal == '\v') {
                        esc_seq = "\\v";
                    }
                    if (yylval.token.data.char_literal == '\'') {
                        esc_seq = "\\\'";
                    }
                    if (yylval.token.data.char_literal == '\"') {
                        esc_seq = "\\\"";
                    }
                    if (yylval.token.data.char_literal == '\\') {
                        esc_seq = "\\\\";
                    }
                    printf("%s %d\tCHARLIT\t%s\n", filename, line_ct, esc_seq); 
                }

                if (yylval.token.type == JACC_TYPE_CHARLIT) {
                    printf("%s %d\tCHARLIT\t%c\n", filename, line_ct, yylval.token.data.char_literal); 
                }
                break;

            case STRING:
                printf("%s\t%d\tSTRING\t%s\n", filename, line_ct, yylval.token.data.string_literal); 
                break;

            case NUMBER:
                if (yylval.token.type == JACC_TYPE_INT) {
                    printf("%s\t%d\tNUMBER\tINTEGER\t%lld\tINT\n", filename, line_ct, yylval.token.data.int_type); 
                }
                if (yylval.token.type == JACC_TYPE_UINT) {
                    printf("%s\t%d\tNUMBER\tINTEGER\t%lld\tUNSIGNED, INT\n", filename, line_ct, yylval.token.data.uint_type); 
                }
                if (yylval.token.type == JACC_TYPE_LONG) {
                    printf("%s\t%d\tNUMBER\tINTEGER\t%lld\tLONG\n", filename, line_ct, yylval.token.data.long_type); 
                }
                if (yylval.token.type == JACC_TYPE_ULONG) {
                    printf("%s\t%d\tNUMBER\tINTEGER\t%lld\tUNSIGNED, LONG\n", filename, line_ct, yylval.token.data.ulong_type); 
                }
                if (yylval.token.type == JACC_TYPE_ULONGLONG) {
                    printf("%s\t%d\tNUMBER\tINTEGER\t%lld\tUNSIGNED, LONGLONG\n", filename, line_ct, yylval.token.data.ulonglong_type); 
                }
                break;


            // Case for single byte operator is handled within LEX_SINGLE_TOK

            // Operators 
            case INDSEL:
            case PLUSPLUS:
            case MINUSMINUS:
            case SHL:
            case SHR:
            case LTEQ:
            case GTEQ:
            case EQEQ:
            case NOTEQ:
            case LOGAND:
            case LOGOR:
            case ELLIPSIS:
            case TIMESEQ:
            case DIVEQ:
            case MODEQ:
            case PLUSEQ:
            case MINUSEQ:
            case SHLEQ:
            case SHREQ:
            case ANDEQ:
            case OREQ:
            case XOREQ:

            // Keywords
            case AUTO:
            case BREAK:
            case CASE:
            case CHAR:
            case CONST:
            case CONTINUE:
            case DEFAULT:
            case DO:
            case DOUBLE:
            case ELSE:
            case ENUM:
            case EXTERN:
            case FLOAT:
            case FOR:
            case GOTO:
            case IF:
            case INLINE:
            case INT:
            case LONG:
            case REGISTER:
            case RESTRICT:
            case RETURN:
            case SHORT:
            case SIGNED:
            case SIZEOF:
            case STATIC:
            case STRUCT:
            case SWITCH:
            case TYPEDEF:
            case UNION:
            case UNSIGNED:
            case VOID:
            case VOLATILE:
            case WHILE:
            case _BOOL:
            case _COMPLEX:
            case _IMAGINARY:
                printf("%s\t%d\t%s\n", filename, line_ct, string_tokens[STRING_TOK_IDX(t)]);
                break;

            default:
                break;
        }
    }
}
