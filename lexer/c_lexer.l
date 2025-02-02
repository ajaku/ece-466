%option noyywrap
%{
#include <math.h>
#include "tokens-manual.h"

int yyval; // Want to make this a struct
int lc;
char *filename = "ltests/chars.c";
%}

%x STRING
%x CHARLIT
%x MULTIBYTE

%x PREPROC

%%
#[" "]+[0-9]+[" "]\"                    {printf("Preprocessor stuff: %s\n", yytext); BEGIN(PREPROC); }
<PREPROC>([^\"]+)                       {printf("Name of file: %s\n", yytext);}
<PREPROC>\"$                            {printf("Done with preproc: %s\n", yytext); BEGIN(0); }

[\n]                                    { lc++;}
!|%|^|&|\*|-|\+|=|~|\||\.|<|>|\/|\?     {printf("Simple operator: %s, %d\n", yytext, (*yytext)); return (int)(*yytext);}
\+=                                     {printf("Compount assignment operator: %s\n", yytext); return PLUSEQ;}
-=                                      {printf("Compount assignment operator: %s\n", yytext); return MINUSEQ;}
\*=                                     {printf("Compount assignment operator: %s\n", yytext); return TIMESEQ;}
\/=                                     {printf("Compount assignment operator: %s\n", yytext); return DIVEQ;}
%=                                      {printf("Compount assignment operator: %s\n", yytext); return MODEQ;}
->                                      {printf("Compount assignment operator: %s\n", yytext); return MODEQ;}

^\'                                     { BEGIN(CHARLIT); }
<CHARLIT>\\[0abfnrtv\'\"\\]'            { printf("%s %d\tCHARLIT %.2s\n", filename, lc, yytext); BEGIN(0); }
<CHARLIT>[^\\n]'                        { printf("CHARLIT %.1s\n", yytext); BEGIN(0); }
<CHARLIT>[^\\n][^']+                    { printf("Accepted %.1s, error: Invalid character format: %s", yytext, yytext); BEGIN(MULTIBYTE); }
<CHARLIT>\\[0abfnrtv\'\"\\][^']+        { printf("Accepted %.1s, error: Invalid character format: %s", yytext, yytext); BEGIN(MULTIBYTE); }
<MULTIBYTE>'$                           { printf("%s\n", yytext); BEGIN(0); } 


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
            default: printf("Not a digit!\n");
       } 
    }
//<CHARLIT>\\[0ntx"'\\]\'$                { printf("CHARLIT: %s\n", yytext[0]); }
//<CHARLIT>[^'\\]{1}\'$                      { printf("CHARLIT: %s\n", yytext); BEGIN(0);}
//<CHARLIT>'                              { BEGIN(0); }

// <CHARLIT>[^\\n][^']+                    { printf("Error: Invalid character format: %d\nAccepted %s\n", (int)(*yytext), yytext); BEGIN(0); }
// <CHARLIT>\\[0abfnrtv\'\"\\][^']+        { printf("Error: Invalid character format: %d\nAccepted %s\n", (int)(*yytext), yytext); BEGIN(0); } 
}