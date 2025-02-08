#include "lex.h"

const char *string_tokens[] = {
	"TOKEOF",
	"IDENT",	      
	"CHARLIT",        // `byte` (we don't support multibyte)
	"STRING",         // "<here>"
	"NUMBER",         // Numerical value - will be specified as INT, DOUBLE, ETC.
    /* Operators */
	"INDSEL",         // ->
	"PLUSPLUS",       // ++
	"MINUSMINUS",     // --
	"SHL",            // <<
	"SHR",            // >>
	"LTEQ",           // <=
	"GTEQ",           // >=
	"EQEQ",           // ==
	"NOTEQ",          // !=
	"LOGAND",         // &&
	"LOGOR",          // ||
	"ELLIPSIS",       // ...
	"TIMESEQ",        // *=
	"DIVEQ",          // /=
	"MODEQ",          // %=
	"PLUSEQ",         // +=
	"MINUSEQ",        // -=
	"SHLEQ",          // <<=
	"SHREQ",          // >>=
	"ANDEQ",          // &=
	"OREQ",           // |=
	"XOREQ",          // ^=
    /* Keywords */
	"AUTO",           // auto
	"BREAK",          // break
	"CASE",           // case
	"CHAR",           // char
	"CONST",          // const
	"CONTINUE",       // continue
	"DEFAULT",        // default
	"DO",             // do
	"DOUBLE",         // double
	"ELSE",           // else
	"ENUM",           // enum
	"EXTERN",         // extern
	"FLOAT",          // float
	"FOR",            // for
	"GOTO",           // goto
	"IF",             // if
	"INLINE",         // inline
	"INT",            // int
	"LONG",           // long
	"REGISTER",       // register
	"RESTRICT",       // restrict
	"RETURN",         // return
	"SHORT",          // short
	"SIGNED",         // signed
	"SIZEOF",         // sizeof
	"STATIC",         // static
	"STRUCT",         // struct
	"SWITCH",         // switch
	"TYPEDEF",        // typedef
	"UNION",          // union
	"UNSIGNED",       // unsigned
	"VOID",           // void
	"VOLATILE",       // volatile
	"WHILE",          // while
	"_BOOL",          // _bool
	"_COMPLEX",       // _complex
	"_IMAGINARY"      // _imaginary
};

int lex_append_str(size_t *size, char **a, char **b) {
    *size += strlen(*b);
    *a = realloc(*a, *size + 1);
    if (a == NULL) {
        fprintf(stderr, "Failed to allocate memory within realloc\n");
        exit(1);
    }
    strcat(*a, *b);
    return 0;
}

char lex_handle_esc(const char *s) {
    switch(*s) {
        case '0': return '\0';
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'v': return '\v';
        case '\'': return '\'';
        case '\"': return '\"';
        case '\\': return '\\';
        default:
            fprintf(stdout, "Some strange error!\n");
            return *s;
    }
}

void lex_handle_nums(const char *yytext, YYSTYPE *yylval, const char *filename, int line_ct) {
    char *endptr;
    unsigned long long num = strtoull(yytext, &endptr, 0);

    if (*endptr == '\0') {
        yylval->int_type = (int)val;
        printf("%s\t%d\tNUMBER\tINTEGER\t%d\tINT\n", filename, line_ct, yylval->int_type); 
        return;
    }

    if (strcmp(endptr, "U")) {
        yylval->uint_type = (unsigned int)val;
        printf("%s\t%d\tNUMBER\tINTEGER\t%u\tUNSIGNED\n", filename, line_ct, yylval->uint_type); 
        return;
    }

    if (strcmp(endptr, "L")) {
        yylval->long_type = (long))val;
        printf("%s\t%d\tNUMBER\tINTEGER\t%ld\tLONG\n", filename, line_ct, yylval->uint_type); 
        return;
    }

    if (strcmp(endptr, "UL") || strcmp(endptr, "LU")) {
        yylval->ulong_type = (unsigned long))val;
        printf("%s\t%d\tNUMBER\tINTEGER\t%lu\tUNSIGNED, LONG\n", filename, line_ct, yylval->ulong_type); 
        return;
    }

    if (strcmp(endptr, "ULL") || strcmp(endptr, "LUL") || strcmp(endptr, "LLU")) {
        yylval->ulonglong_type = val;
        printf("%s\t%d\tNUMBER\tINTEGER\t%ld\tUNSIGNED, LONGLONG\n", filename, line_ct, yylval->ulonglong_type); 
        return;
    }
}