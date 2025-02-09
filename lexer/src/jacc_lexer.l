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

NUM_EXTENSIONS              ((U|L|UL|LU|ULL|LUL|LLU)?)

%x CHARLIT
%x STRING

%%
#.*                                         {
                                                if (filename == NULL) {
                                                    filename = (char *)malloc(sizeof(char *) * strlen(yytext) - 6);
                                                    strncpy(filename, yytext + 5, strlen(yytext) - 6);
                                                } 
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
                                                lex_handle_integers(yytext, &yylval, filename, line_ct);
                                                return NUMBER;
                                            }
([1-9][0-9]*){NUM_EXTENSIONS}                  { 
                                                lex_handle_integers(yytext, &yylval, filename, line_ct);
                                                return NUMBER;
                                            }
(0[xX][0-9a-fA-F]+){NUM_EXTENSIONS}         { 
                                                lex_handle_integers(yytext, &yylval, filename, line_ct);
                                                return NUMBER;
                                            }
^(L\')|(\')                                 { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'                { 
                                                yylval.char_literal = lex_handle_esc(yytext + 1);
                                                printf("%s %d\tCHARLIT\t%.2s\n", filename, line_ct, yytext); 
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHARLIT>[^\\n]'                            { 
                                                yylval.char_literal = yytext[0];
                                                printf("%s %d\tCHARLIT\t%c\n", filename, line_ct, yylval.char_literal); 
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHARLIT>[^\\n][^']+                        { 
                                                yylval.char_literal = yytext[0];
                                                printf("%s %d\tCHARLIT\t%c\n", filename, line_ct, yylval.char_literal); 
                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+            { 
                                                char *temp = (char *)malloc(sizeof(char *));
                                                temp[0] = yytext[1];
                                                yylval.char_literal = lex_handle_esc(temp);
                                                free(temp);
                                                printf("%s %d\tCHARLIT\t%.2s\n", filename, line_ct, yytext); 
                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }

^(L\")|(\")                                 {  
                                                BEGIN(STRING);
                                                str_size = 0;
                                                str = (char *)malloc(sizeof(char *));
                                                printf("%s\t%d\tSTRING\t", filename, line_ct);
                                            }
<STRING>.                                   { 
                                                lex_append_str(&str_size, &str, &yytext);
                                                printf("%s", yytext);
                                            }
<STRING>\\[xX][0-9a-fA-F]+                  { 
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
<STRING>\\[0-7]{1,3}                        { 
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
<STRING>\\\?                                { 
                                                lex_append_str(&str_size, &str, &yytext);
                                                printf("\?"); 
                                            }
<STRING>\"$                                 { 
                                                yylval.string_literal = str;
                                                printf("\n");
                                                BEGIN(INITIAL);
                                                return STRING;
                                            }


[_A-Za-z]+[_A-Za-z0-9]*                     { printf("%s\t%d\tIDENT\t%s\n", filename, line_ct, yytext); }
.                                           { fprintf(stdout, "%s:%d:Error:Uncrecognized term %s\n", filename, line_ct, yytext); }
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
