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

char lex_handle_esc(char *s) {
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
            fprintf(stderr, "Some strange error!\n");
            return *s;
    }
}

int lex_handle_integers(char *yytext, YYSTYPE *yylval) {
    char *endptr;
    unsigned long long val = strtoull(yytext, &endptr, 0);

    if (*endptr == '\0') {
        yylval->token.type = JACC_TYPE_INT;
        yylval->token.data.int_type = (int)val;
        return NUMBER;
    }

    if (strcmp(endptr, "U") == 0) {
        yylval->token.type = JACC_TYPE_UINT;
        yylval->token.data.uint_type = (unsigned int)val;
        return NUMBER;
    }

    if (strcmp(endptr, "L") == 0) {
        yylval->token.type = JACC_TYPE_LONG;
        yylval->token.data.long_type = (long)val;
        return NUMBER;
    }

    if (strcmp(endptr, "UL") == 0 || strcmp(endptr, "LU") == 0) {
        yylval->token.type = JACC_TYPE_ULONG;
        yylval->token.data.ulong_type = (unsigned long)val;
        return NUMBER;
    }

    if (strcmp(endptr, "ULL") == 0 ||
        strcmp(endptr, "LUL") == 0 ||
        strcmp(endptr, "LLU") == 0   ) {

        yylval->token.type = JACC_TYPE_ULONGLONG;
        yylval->token.data.ulonglong_type = val;
        return NUMBER;
    }
    return NUMBER;
}