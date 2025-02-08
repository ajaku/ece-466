%option noyywrap
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

int lc;
char *filename = NULL;
char *str = NULL;
ssize_t str_size = 0;
YYSTYPE yylval;

%}

%x PREPROC
%x CHARLIT
%x MULTIBYTE
%x STRING

%%
#[" "]+[0]+[" "]\"                      { BEGIN(PREPROC); }
<PREPROC>([^\"\n]+)                     {
                                            if (filename == NULL) {
                                                filename = strdup(yytext);
                                            }
                                        }
<PREPROC>\"$                            { BEGIN(INITIAL); }

[\n]                                    { lc++; }

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
                                            printf("%s %d\tNUMBER\t%s\tINTEGER\n", filename, lc, yytext); 
                                        }
[0-9]+L$                                { 
                                            printf("%s %d\tNUMBER\t%s\n\tLONG", filename, lc, yytext); 
                                        }

^(L\')|(\')                             { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'            { 
                                            yylval.char_literal = lex_handle_esc(yytext + 1);
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, lc, yytext); 
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n]'                        { 
                                            yylval.char_literal = yytext[0];
                                            printf("%s %d\tCHARLIT\t%c\n", filename, lc, yylval.char_literal); 
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n][^']+                    { 
                                            printf("%s %d\tCHARLIT\t%.1s\n", filename, lc, yytext);
                                            char *character = strdup(yytext);
                                            yylval.string_literal = (char *)malloc(sizeof(char *));
                                            for (int i = 0; i < 2; i++) {
                                                yylval.string_literal[i] = character[i];
                                            }
                                            fprintf(stdout, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, lc);
                                            BEGIN(MULTIBYTE);
                                        }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+        { 
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, lc, yytext);
                                            char *character = strdup(yytext);
                                            yylval.string_literal = (char *)malloc(sizeof(char *));
                                            for (int i = 0; i < 2; i++) {
                                                yylval.string_literal[i] = character[i];
                                            }
                                            fprintf(stdout, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, lc);
                                            BEGIN(MULTIBYTE);
                                        }
<MULTIBYTE>'$                           { BEGIN(INITIAL); return CHARLIT; } 

^(L\")|(\")                             { BEGIN(STRING);
                                            str_size = 0;
                                            str = (char *)malloc(sizeof(char *));
                                            printf("%s\t%d\tSTRING\t", filename, lc);
                                        }
<STRING>.                               { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            printf("%s", yytext);
                                        }
<STRING>\\[xX][0-9a-fA-F]+              { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            unsigned int dec_rep;
                                            sscanf(yytext + 2, "%x", &dec_rep);


                                            if (dec_rep < 127 && dec_rep  > 32) {
                                                printf("%c", dec_rep);
                                            } else {
                                                // Hex is getting stored in a char, which is max 1 byte (8 bits => 377 octal => 255 dec)
                                                if (dec_rep > 255)
                                                {
                                                    dec_rep = 255;
                                                    fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, lc, yytext);
                                                }
                                                printf("\\%03o", dec_rep);
                                            }


                                        }
<STRING>\\[0-7]{1,3}                    { 
                                            lex_append_str(&str_size, &str, &yytext);
                                            int yytext_size = strlen(yytext);
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
