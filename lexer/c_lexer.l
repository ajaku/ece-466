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
                                            BEGIN(0);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n]'                        { 
                                            printf("%s %d\tCHARLIT\t%.1s\n", filename, lc, yytext); 
                                            BEGIN(0);
                                            return CHARLIT;
                                        }
<CHARLIT>[^\\n][^']+                    { 
                                            printf("%s %d\tCHARLIT\t%.1s\n", filename, lc, yytext);
                                            fprintf(stdout, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, lc);
                                            BEGIN(MULTIBYTE);
                                        }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+        { 
                                            printf("%s %d\tCHARLIT\t%.2s\n", filename, lc, yytext);
                                            fprintf(stdout, "%s:%d:Warning:Unsupported multibyte character literal truncated to first byte\n", filename, lc);
                                            BEGIN(MULTIBYTE);
                                        }
<MULTIBYTE>'$                           { BEGIN(0); return CHARLIT; } 

^(L\")|(\")                             { BEGIN(STRING);
                                            str_size = 0;
                                            str = (char *)malloc(sizeof(char));
                                            printf("%s\t%d\tSTRING\t", filename, lc);
                                        }
<STRING>.                               { 
                                            append_str(&str_size, &str, &yytext);
                                            printf("%s", yytext);
                                        }
<STRING>\\[xX][0-9a-fA-F]+              { 
                                            append_str(&str_size, &str, &yytext);
                                            /*
                                            int hex_value;
                                            sscanf(yytext + 2, "%2x", &hex_value);  // Skip "\x" and read hex
                                            printf("hex_value: %d\n", hex_value);
                                            printf("Octal: %03o\n",(unsigned char) hex_value);
                                            printf("YYTEXT: %s\n", yytext);
                                            printf("%03o\n", (unsigned char) *yytext);
                                            */

                                            // I think there's definitely a bug with the string I'm saving here, but out of time to fix it
                                            ssize_t dec_val = 0;
                                            ssize_t num_len = strlen(yytext);
                                            for (int i = num_len - 1; i >= 2; i--) {
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
                                            ssize_t octal_rep[3]; // will be at most 3 octal bits wide (377 max)

                                            if (dec_val > 255) {
                                                dec_val = 255;

                                                fprintf(stderr, "%s:%d:Warning:Hex escape sequence %s out of range\n", filename, lc, yytext);
                                            }

                                            if (dec_val < 127 && dec_val > 32) {
                                                printf("%c", (char)(dec_val));
                                            } else {
                                                // Convert decmial to hex
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
                                        }
<STRING>\\[0-7]{1,3}                    { 
                                            append_str(&str_size, &str, &yytext);
                                            if (str_size == 2 && yytext[1] == '0') {
                                                printf("\\0"); 
                                            }
                                            else if (str_size == 2) {
                                                printf("\\00%d", 
                                                        yytext[1] - '0');
                                            }
                                            else if (str_size == 3) {
                                                printf("\\0%d%d\n",
                                                        yytext[1] - '0',
                                                        yytext[2] - '0');
                                            }
                                            else if (str_size == 4) {
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
                                            BEGIN(0);
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
