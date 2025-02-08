%option noyywrap
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens-manual.h"

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

!|%|^|&|\*|-|\+|=|~|\||\.|<|>|\/|\?|;   { OP(yytext); }
\[|\]                                   { OP(yytext); }
\{|\}                                   { OP(yytext); }
\(|\)                                   { OP(yytext); }
->                                      { MULTI_OP(INDSEL); }
\+\+                                    { MULTI_OP(PLUSPLUS); }
--                                      { MULTI_OP(MINUSMINUS); } 
\<\<                                    { MULTI_OP(SHL); }
>>                                      { MULTI_OP(SHR); }
\<=                                     { MULTI_OP(LTEQ); }
>=                                      { MULTI_OP(GTEQ); }
==                                      { MULTI_OP(EQEQ); }
!=                                      { MULTI_OP(NOTEQ); }
&&                                      { MULTI_OP(LOGAND); }
\|\|                                    { MULTI_OP(LOGOR); }
\.\.\.                                  { MULTI_OP(ELLIPSIS); }
\*=                                     { MULTI_OP(TIMESEQ); }
\/=                                     { MULTI_OP(DIVEQ); }
\%=                                     { MULTI_OP(MODEQ); }
\+=                                     { MULTI_OP(PLUSEQ); }
-=                                      { MULTI_OP(MINUSEQ); }
\<\<=                                   { MULTI_OP(SHLEQ); }
>>=                                     { MULTI_OP(SHREQ); }
&=                                      { MULTI_OP(ANDEQ); }
\|=                                     { MULTI_OP(OREQ); }
\^=                                     { MULTI_OP(XOREQ); }

AUTO                                    { MULTI_OP(AUTO); }
BREAK                                   { MULTI_OP(BREAK); }
CASE                                    { MULTI_OP(CASE); }
CHAR                                    { MULTI_OP(CHAR); }
CONST                                   { MULTI_OP(CONST); }
CONTINUE                                { MULTI_OP(CONTINUE); }
DEFAULT                                 { MULTI_OP(DEFAULT); }
DO                                      { MULTI_OP(DO); }
DOUBLE                                  { MULTI_OP(DOUBLE); }
ELSE                                    { MULTI_OP(ELSE); }
ENUM                                    { MULTI_OP(ENUM); }
EXTERN                                  { MULTI_OP(EXTERN); }
FLOAT                                   { MULTI_OP(FLOAT); }
FOR                                     { MULTI_OP(FOR); }
GOTO                                    { MULTI_OP(GOTO); }
IF                                      { MULTI_OP(IF); }
INLINE                                  { MULTI_OP(INLINE); }
INT                                     { MULTI_OP(INT); }
LONG                                    { MULTI_OP(LONG); }
REGISTER                                { MULTI_OP(REGISTER); }
RESTRICT                                { MULTI_OP(RESTRICT); }
RETURN                                  { MULTI_OP(RETURN); }
SHORT                                   { MULTI_OP(SHORT); }
SIGNED                                  { MULTI_OP(SIGNED); }
SIZEOF                                  { MULTI_OP(SIZEOF); }
STATIC                                  { MULTI_OP(STATIC); }
STRUCT                                  { MULTI_OP(STRUCT); }
SWITCH                                  { MULTI_OP(SWITCH); }
TYPEDEF                                 { MULTI_OP(TYPEDEF); }
UNION                                   { MULTI_OP(UNION); }
UNSIGNED                                { MULTI_OP(UNSIGNED); }
VOID                                    { MULTI_OP(VOID); }
VOLATILE                                { MULTI_OP(VOLATILE); }
WHILE                                   { MULTI_OP(WHILE); }
_BOOL                                   { MULTI_OP(_BOOL); }
_COMPLEX                                { MULTI_OP(_COMPLEX); }
_IMAGINARY                              { MULTI_OP(_IMAGINARY); }

[0-9]+                                  { 
                                            printf("%s %d\tNUMBER\t%s\tINTEGER\n", filename, lc, yytext); 
                                        }
[0-9]+L$                                { 
                                            printf("%s %d\tNUMBER\t%s\n\tLONG", filename, lc, yytext); 
                                        }

^(L\')|(\')                             { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'            { 
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, lc, yytext); 
                                            char *character = strdup(yytext);
                                            yylval.string_literal = (char *)malloc(sizeof(char *));
                                            for (int i = 0; i < 2; i++) {
                                                yylval.string_literal[i] = character[i];
                                            }
                                            printf("%s\n", yylval.string_literal);
                                            BEGIN(INITIAL);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n]'                        { 
                                            printf("%s %d\tCHARLIT\t%.1s\n", filename, lc, yytext); 
                                            char *character = strdup(yytext);
                                            yylval.string_literal = (char *)malloc(sizeof(char));
                                            yylval.string_literal[0] = character[0];
                                            printf("%s\n", yylval.string_literal);
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
                                            append_str(&str_size, &str, &yytext);
                                            printf("%s", yytext);
                                        }
<STRING>\\[xX][0-9a-fA-F]+              { 
                                            append_str(&str_size, &str, &yytext);
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
                                            append_str(&str_size, &str, &yytext);
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
                                            append_str(&str_size, &str, &yytext);
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
