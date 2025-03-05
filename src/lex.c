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

int lex_append_char(size_t *size, char **a, char b) {
    *size += 1;
    *a = realloc(*a, *size + 1);
    (*a)[*size - 1] = b;
    return 0;
}

char lex_handle_esc_char(char *s) {
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

char lex_handle_esc_str(char *s) {
    if (!strcmp(s, "\\0")) return '\0';
    if (!strcmp(s, "\\a")) return '\a';
    if (!strcmp(s, "\\b")) return '\b';
    if (!strcmp(s, "\\f")) return '\f';
    if (!strcmp(s, "\\n")) return '\n';
    if (!strcmp(s, "\\r")) return '\r';
    if (!strcmp(s, "\\t")) return '\t';
    if (!strcmp(s, "\\v")) return '\v';
    if (!strcmp(s, "\\\'")) return '\'';
    if (!strcmp(s, "\\\"")) return '\"';
    if (!strcmp(s, "\\\\")) return '\\';
    if (!strcmp(s, "\\\?")) return '\?';

    return 1; //what would be the correct error to return here?
}

char *lex_return_str_esc(char esc) {
    if (esc == '\0') return "\\0"; 
    if (esc == '\a') return "\\a";
    if (esc == '\b') return "\\b";
    if (esc == '\f') return "\\f";
    if (esc == '\n') return "\\n";
    if (esc == '\r') return "\\r";
    if (esc == '\t') return "\\t";
    if (esc == '\v') return "\\v";
    if (esc == '\'') return "\\\'";
    if (esc == '\"') return "\\\"";
    if (esc == '\\') return "\\\\";

    return NULL;
}

int lex_handle_integers(char *yytext, jacc_lex_tok_t *token) {
    char *endptr;
    unsigned long long val = strtoull(yytext, &endptr, 0);

    if (*endptr == '\0') {
        token->data_type = JACC_TYPE_INT;
        token->data.int_d = (int)val;
        return NUMBER;
    }

    if (strcmp(endptr, "U") == 0) {
        token->data_type = JACC_TYPE_UINT;
        token->data.uint_d = (unsigned int)val;
        return NUMBER;
    }

    if (strcmp(endptr, "L") == 0) {
        token->data_type = JACC_TYPE_LONG;
        token->data.long_d = (long)val;
        return NUMBER;
    }

    if (strcmp(endptr, "UL") == 0 || strcmp(endptr, "LU") == 0) {
        token->data_type = JACC_TYPE_ULONG;
        token->data.ulong_d = (unsigned long)val;
        return NUMBER;
    }

    if (strcmp(endptr, "ULL") == 0 ||
        strcmp(endptr, "LUL") == 0 ||
        strcmp(endptr, "LLU") == 0   ) {

        token->data_type = JACC_TYPE_ULONGLONG;
        token->data.ulonglong_d = val;
        return NUMBER;
    }
    return NUMBER;
}
