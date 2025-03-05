%option noyywrap
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lex.h"
#include "ast.h"

int line_ct;
char *filename = NULL;
char *str = NULL;

%}

NUM_EXTENSIONS              ((U|L|UL|LU|ULL|LUL|LLU)?)

%x CHAR_TOK
%x MULTIBYTE
%x STR_TOK

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
->                                          { LEX_MULTI_TOK(INDSEL, yytext); }
\+\+                                        { LEX_MULTI_TOK(PLUSPLUS, yytext); }
--                                          { LEX_MULTI_TOK(MINUSMINUS, yytext); } 
\<\<                                        { LEX_MULTI_TOK(SHL, yytext); }
>>                                          { LEX_MULTI_TOK(SHR, yytext); }
\<=                                         { LEX_MULTI_TOK(LTEQ, yytext); }
>=                                          { LEX_MULTI_TOK(GTEQ, yytext); }
==                                          { LEX_MULTI_TOK(EQEQ, yytext); }
!=                                          { LEX_MULTI_TOK(NOTEQ, yytext); }
&&                                          { LEX_MULTI_TOK(LOGAND, yytext); }
\|\|                                        { LEX_MULTI_TOK(LOGOR, yytext); }
\.\.\.                                      { LEX_MULTI_TOK(ELLIPSIS, yytext); }
\*=                                         { LEX_MULTI_TOK(TIMESEQ, yytext); }
\/=                                         { LEX_MULTI_TOK(DIVEQ, yytext); }
\%=                                         { LEX_MULTI_TOK(MODEQ, yytext); }
\+=                                         { LEX_MULTI_TOK(PLUSEQ, yytext); }
-=                                          { LEX_MULTI_TOK(MINUSEQ, yytext); }
\<\<=                                       { LEX_MULTI_TOK(SHLEQ, yytext); }
>>=                                         { LEX_MULTI_TOK(SHREQ, yytext); }
&=                                          { LEX_MULTI_TOK(ANDEQ, yytext); }
\|=                                         { LEX_MULTI_TOK(OREQ, yytext); }
\^=                                         { LEX_MULTI_TOK(XOREQ, yytext); }

auto                                        { LEX_MULTI_TOK(AUTO, yytext); }
break                                       { LEX_MULTI_TOK(BREAK, yytext); }
case                                        { LEX_MULTI_TOK(CASE, yytext); }
char                                        { LEX_MULTI_TOK(CHAR, yytext); }
const                                       { LEX_MULTI_TOK(CONST, yytext); }
continue                                    { LEX_MULTI_TOK(CONTINUE, yytext); }
default                                     { LEX_MULTI_TOK(DEFAULT, yytext); }
do                                          { LEX_MULTI_TOK(DO, yytext); }
double                                      { LEX_MULTI_TOK(DOUBLE, yytext); }
else                                        { LEX_MULTI_TOK(ELSE, yytext); }
enum                                        { LEX_MULTI_TOK(ENUM, yytext); }
extern                                      { LEX_MULTI_TOK(EXTERN, yytext); }
float                                       { LEX_MULTI_TOK(FLOAT, yytext); }
for                                         { LEX_MULTI_TOK(FOR, yytext); }
goto                                        { LEX_MULTI_TOK(GOTO, yytext); }
if                                          { LEX_MULTI_TOK(IF, yytext); }
inline                                      { LEX_MULTI_TOK(INLINE, yytext); }
int                                         { LEX_MULTI_TOK(INT, yytext); }
long                                        { LEX_MULTI_TOK(LONG, yytext); }
register                                    { LEX_MULTI_TOK(REGISTER, yytext); }
restrict                                    { LEX_MULTI_TOK(RESTRICT, yytext); }
return                                      { LEX_MULTI_TOK(RETURN, yytext); }
short                                       { LEX_MULTI_TOK(SHORT, yytext); }
signed                                      { LEX_MULTI_TOK(SIGNED, yytext); }
sizeof                                      { LEX_MULTI_TOK(SIZEOF, yytext); }
static                                      { LEX_MULTI_TOK(STATIC, yytext); }
struct                                      { LEX_MULTI_TOK(STRUCT, yytext); }
switch                                      { LEX_MULTI_TOK(SWITCH, yytext); }
typedef                                     { LEX_MULTI_TOK(TYPEDEF, yytext); }
union                                       { LEX_MULTI_TOK(UNION, yytext); }
unsigned                                    { LEX_MULTI_TOK(UNSIGNED, yytext); }
void                                        { LEX_MULTI_TOK(VOID, yytext); }
volatile                                    { LEX_MULTI_TOK(VOLATILE, yytext); }
while                                       { LEX_MULTI_TOK(WHILE, yytext); }
_Bool                                       { LEX_MULTI_TOK(_BOOL, yytext); }
_Complex                                    { LEX_MULTI_TOK(_COMPLEX, yytext); }
_Imaginary                                  { LEX_MULTI_TOK(_IMAGINARY, yytext); }

(0[0-7]*){NUM_EXTENSIONS}                   { 
                                                return lex_handle_integers(yytext, &(yylval.tok));
                                            }
([1-9][0-9]*){NUM_EXTENSIONS}               { 
                                                return lex_handle_integers(yytext, &(yylval.tok));
                                            }
(0[xX][0-9a-fA-F]+){NUM_EXTENSIONS}         { 
                                                return lex_handle_integers(yytext, &(yylval.tok));
                                            }
(L\')|(\')                                  { 
                                                BEGIN(CHAR_TOK); 
                                            }
<CHAR_TOK>\\[0abfnrtv\'\"\\]'               { 
                                                yylval.tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.tok.data.char_d = lex_handle_esc_char(yytext + 1);
                                                yylval.tok.size = 1;

                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHAR_TOK>[^\\]'                            { 
                                                yylval.tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.tok.data.char_d = yytext[0];
                                                yylval.tok.size = 1;

                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHAR_TOK>[^\\][^']+                        { 
                                                yylval.tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.tok.data.char_d = yytext[0];
                                                yylval.tok.size = 1;

                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(MULTIBYTE);
                                                return CHARLIT;
                                            }
<CHAR_TOK><<EOF>>                           { 
                                                fprintf(stderr, "ERROR:EOF detected mid parse of a character sequence.\n");
                                                return 0;
                                            }
<CHAR_TOK>\\[0abfnrtv\'\"\\][^']+           { 
                                                char *temp = (char *)malloc(sizeof(char *));
                                                temp[0] = yytext[1];

                                                yylval.tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.tok.data.char_d = lex_handle_esc_char(temp);
                                                yylval.tok.size = 1;

                                                free(temp);
                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(MULTIBYTE);
                                                return CHARLIT;
                                            }
<MULTIBYTE>'                                { BEGIN(INITIAL); } 

(L\")|(\")                                  {  
                                                BEGIN(STR_TOK);
                                                yylval.tok.data_type = JACC_TYPE_STRING;
                                                yylval.tok.data.string_d = (char *)malloc(sizeof(char *));
                                                yylval.tok.size = 0;

                                            }
<STR_TOK>\\[0abfnrtv\'\"\\\?]               {
                                                char char_to_app = lex_handle_esc_str(yytext);
                                                lex_append_char(&(yylval.tok.size), &(yylval.tok.data.string_d), char_to_app);                                                   
                                            }
<STR_TOK>\\[xX][0-9a-fA-F]+                 { 
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 2, "%x", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                lex_append_char(&(yylval.tok.size), &(yylval.tok.data.string_d), (char)hex_val);                                                   
                                            }
<STR_TOK>\\[0-7]{1,3}                       { 
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 1, "%o", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                lex_append_char(&(yylval.tok.size), &(yylval.tok.data.string_d), (char)hex_val);                                                   
                                            }
<STR_TOK>\"                                 { 
                                                lex_append_char(&(yylval.tok.size), &(yylval.tok.data.string_d), '\0');                                                   
                                                BEGIN(INITIAL);
                                                return STRING;
                                            }
<STR_TOK><<EOF>>                            { 
                                                fprintf(stderr, "ERROR:EOF detected mid parse within a string.\n");
                                                return 0;
                                            }
<STR_TOK>.                                  { 
                                                lex_append_str(&(yylval.tok.size), &(yylval.tok.data.string_d), &yytext);                                                   
                                            }


[_A-Za-z]+[_A-Za-z0-9]*                     { 
                                                yylval.tok.data_type = JACC_TYPE_STRING;
                                                yylval.tok.data.string_d = strdup(yytext);
                                                yylval.tok.size = strlen(yytext) + 1;

                                                return IDENT;
                                            }
.                                           {  }
%%