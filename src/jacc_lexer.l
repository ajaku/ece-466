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
                                                lex_alloc_yylval(&yylval);
                                                return lex_handle_integers(yytext, &(yylval.ast_p->lex_tok));
                                            }
([1-9][0-9]*){NUM_EXTENSIONS}               { 
                                                lex_alloc_yylval(&yylval);
                                                return lex_handle_integers(yytext, &(yylval.ast_p->lex_tok));
                                            }
(0[xX][0-9a-fA-F]+){NUM_EXTENSIONS}         { 
                                                lex_alloc_yylval(&yylval);
                                                return lex_handle_integers(yytext, &(yylval.ast_p->lex_tok));
                                            }
(L\')|(\')                                  { 
                                                BEGIN(CHAR_TOK); 
                                                lex_alloc_yylval(&yylval);
                                            }
<CHAR_TOK>\\[0abfnrtv\'\"\\]'               { 
                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.ast_p->lex_tok.data.char_d = lex_handle_esc_char(yytext + 1);
                                                yylval.ast_p->lex_tok.size = 1;

                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHAR_TOK>[^\\]'                            { 
                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.ast_p->lex_tok.data.char_d = yytext[0];
                                                yylval.ast_p->lex_tok.size = 1;

                                                BEGIN(INITIAL);
                                                return CHARLIT;
                                            }
<CHAR_TOK>[^\\][^']+                        { 
                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.ast_p->lex_tok.data.char_d = yytext[0];
                                                yylval.ast_p->lex_tok.size = 1;

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

                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_CHAR_ESC;
                                                yylval.ast_p->lex_tok.data.char_d = lex_handle_esc_char(temp);
                                                yylval.ast_p->lex_tok.size = 1;

                                                free(temp);
                                                fprintf(stderr, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, line_ct);
                                                BEGIN(MULTIBYTE);
                                                return CHARLIT;
                                            }
<MULTIBYTE>'                                { BEGIN(INITIAL); } 

(L\")|(\")                                  {  
                                                BEGIN(STR_TOK);
                                                lex_alloc_yylval(&yylval);

                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_STRING;
                                                yylval.ast_p->lex_tok.data.string_d = (char *)malloc(sizeof(char *));
                                                yylval.ast_p->lex_tok.size = 0;

                                            }
<STR_TOK>\\[0abfnrtv\'\"\\\?]               {
                                                char char_to_app = lex_handle_esc_str(yytext);
                                                lex_append_char(&(yylval.ast_p->lex_tok.size), &(yylval.ast_p->lex_tok.data.string_d), char_to_app);                                                   
                                            }
<STR_TOK>\\[xX][0-9a-fA-F]+                 { 
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 2, "%x", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                lex_append_char(&(yylval.ast_p->lex_tok.size), &(yylval.ast_p->lex_tok.data.string_d), (char)hex_val);                                                   
                                            }
<STR_TOK>\\[0-7]{1,3}                       { 
                                                unsigned int hex_val = 0;
                                                sscanf(yytext + 1, "%o", &hex_val);

                                                if(hex_val > UCHAR_MAX) {
                                                    hex_val = UCHAR_MAX;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, line_ct, yytext);
                                                }

                                                lex_append_char(&(yylval.ast_p->lex_tok.size), &(yylval.ast_p->lex_tok.data.string_d), (char)hex_val);                                                   
                                            }
<STR_TOK>\"                                 { 
                                                lex_append_char(&(yylval.ast_p->lex_tok.size), &(yylval.ast_p->lex_tok.data.string_d), '\0');                                                   
                                                BEGIN(INITIAL);
                                                return STRING;
                                            }
<STR_TOK><<EOF>>                            { 
                                                fprintf(stderr, "ERROR:EOF detected mid parse within a string.\n");
                                                return 0;
                                            }
<STR_TOK>.                                  { 
                                                lex_append_str(&(yylval.ast_p->lex_tok.size), &(yylval.ast_p->lex_tok.data.string_d), &yytext);                                                   
                                            }


[_A-Za-z]+[_A-Za-z0-9]*                     { 
                                                lex_alloc_yylval(&yylval);
                                                yylval.ast_p->lex_tok.data_type = JACC_TYPE_STRING;
                                                yylval.ast_p->lex_tok.data.string_d = strdup(yytext);
                                                yylval.ast_p->lex_tok.size = strlen(yytext) + 1;

                                                return IDENT;
                                            }
.                                           {  }
%%