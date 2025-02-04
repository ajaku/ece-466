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
<PREPROC>\"$                            { BEGIN(0); }

[\n]                                    { lc++; }

!|%|^|&|\*|-|\+|=|~|\||\.|<|>|\/|\?|;   { OP(yytext); }
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

^\'                                     { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'            { printf("%s %d\tCHARLIT %.2s\n", filename, lc, yytext); BEGIN(0); }
<CHARLIT>[^\\n]'                        { printf("CHARLIT %.1s\n", yytext); BEGIN(0); }
<CHARLIT>[^\\n][^']+                    { printf("Accepted %.1s, error: Invalid character format: %s", yytext, yytext); BEGIN(MULTIBYTE); }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+        { printf("Accepted %.1s, error: Invalid character format: %s", yytext, yytext); BEGIN(MULTIBYTE); }
<MULTIBYTE>'$                           { printf("%s\n", yytext); BEGIN(0); } 

^\"                                     { BEGIN(STRING);
                                            str_size = 0;
                                            str = (char *)malloc(sizeof(char));
                                            if (!str) {
                                                fprintf(stderr, "Failed to allocate memory for string\n");
                                                exit(1);
                                            }
                                        }
<STRING>.                               { 
                                            str_size++;
                                            str = realloc(str, str_size + 1);
                                            strcat(str, yytext);
                                            printf("%s", yytext);
                                        }
<STRING>\\x[0-9a-fA-F]+                 { 
                                            str_size += strlen(yytext);
                                            str = realloc(str, str_size + 1);
                                            strcat(str, yytext);

                                            // Convert hex to decimal
                                            // Convert decmial to hex
                                            ssize_t dec_val = 0;
                                            ssize_t num_len = strlen(yytext);
                                            for (int i = num_len - 1 /* skip \x term */ ; i >= 2; i--) {
                                                if (yytext[i] >= '0' && yytext[i] <= '9') {
                                                    dec_val += (yytext[i] - '0') * pow(16, num_len - i - 1); 
                                                }
                                                if (yytext[i] >= 'A' && yytext[i] <= 'F') {
                                                    dec_val += (yytext[i] - 'A' + 10) * pow(16, num_len - i - 1);
                                                }
                                                if (yytext[i] >= 'a' && yytext[i] <= 'f') {
                                                    dec_val += (yytext[i] - 'a' + 10) * pow(16, num_len - i - 1);
                                                }
                                            }
                                            //printf("dec_val: %d\n", dec_val);
                                            ssize_t octal_rep[3]; // will be at most 3 octal bits wide (377 max)

                                            int i = 0;
                                            while(dec_val != 0) {
                                                octal_rep[i] = dec_val % 8;
                                                dec_val = dec_val / 8;
                                                i++;
                                            }
                                            printf("\\%ld", octal_rep[2]);
                                            printf("%ld", octal_rep[1]);
                                            printf("%ld", octal_rep[0]);
                                        }
<STRING>\\[0-7]{1,3}                    { 
                                            str_size += strlen(yytext);
                                            str = realloc(str, str_size + 1);
                                            strcat(str, yytext);

                                            if (str_size == 1) {
                                                printf("\00%s", yytext);
                                            }
                                            if (str_size == 2) {
                                                printf("\0%s", yytext);
                                            }
                                            printf("%s", yytext);
                                        }
<STRING>\"$                             { printf("leaving string: %s\n", str); BEGIN(0); }

.                                       printf("Unrecognized character: %s\n", yytext);

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
