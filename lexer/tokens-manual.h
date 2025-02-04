#include <string.h>

#define STRING_TOK_IDX(IDX) (IDX - 256)

enum tokens {
	TOKEOF=0,
	IDENT=257,
	CHARLIT,        // `byte` (we don't support multibyte)
	STRING,         // "<here>"
	NUMBER,         // Numerical value - will be specified as INT, DOUBLE, ETC.
    /* Operators */
	INDSEL,         // ->
	PLUSPLUS,       // ++
	MINUSMINUS,     // --
	SHL,            // <<
	SHR,            // >>
	LTEQ,           // <=
	GTEQ,           // >=
	EQEQ,           // ==
	NOTEQ,          // !=
	LOGAND,         // &&
	LOGOR,          // ||
	ELLIPSIS,       // ...
	TIMESEQ,        // *=
	DIVEQ,          // /=
	MODEQ,          // %=
	PLUSEQ,         // +=
	MINUSEQ,        // -=
	SHLEQ,          // <<=
	SHREQ,          // >>=
	ANDEQ,          // &=
	OREQ,           // |=
	XOREQ,          // ^=
    /* Keywords */
	AUTO,           // auto
	BREAK,          // break
	CASE,           // case
	CHAR,           // char
	CONST,          // const
	CONTINUE,       // continue
	DEFAULT,        // default
	DO,             // do
	DOUBLE,         // double
	ELSE,           // else
	ENUM,           // enum
	EXTERN,         // extern
	FLOAT,          // float
	FOR,            // for
	GOTO,           // goto
	IF,             // if
	INLINE,         // inline
	INT,            // int
	LONG,           // long
	REGISTER,       // register
	RESTRICT,       // restrict
	RETURN,         // return
	SHORT,          // short
	SIGNED,         // signed
	SIZEOF,         // sizeof
	STATIC,         // static
	STRUCT,         // struct
	SWITCH,         // switch
	TYPEDEF,        // typedef
	UNION,          // union
	UNSIGNED,       // unsigned
	VOID,           // void
	VOLATILE,       // volatile
	WHILE,          // while
	_BOOL,          // _bool
	_COMPLEX,       // _complex
	_IMAGINARY      // _imaginary
};

const char* token_strings[] = {
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
typedef union {
    char *string_literal;
    int integer;
} YYSTYPE;

extern YYSTYPE yylval;

#define OP(yytext) \
                printf("%s\t%d\t%s\n", filename, lc, yytext); \
                yylval.string_literal = strdup(yytext); \
                return (int)(*yytext);

#define MULTI_OP(OP) \
                printf("%s\t%d\t%s\n", filename, lc, token_strings[STRING_TOK_IDX(OP)]); \
                yylval.string_literal = strdup(yytext); \
                return OP;
