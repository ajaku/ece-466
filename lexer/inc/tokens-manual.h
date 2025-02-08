#define MAX_READABLE_ASCII 127
#define MIN_READABLE_ASCII 321

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

extern const char *string_tokens[];
