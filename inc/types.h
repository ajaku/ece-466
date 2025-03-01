#pragma once

#include <stddef.h>
#include "ast.h"

typedef enum {
  JACC_TYPE_CHARLIT,
  JACC_TYPE_CHAR_ESC,
  JACC_TYPE_STRING,
  JACC_TYPE_INT,
  JACC_TYPE_UINT,
  JACC_TYPE_LONG,
  JACC_TYPE_ULONG,
  JACC_TYPE_ULONGLONG,
  // Not supported ATM
  JACC_TYPE_FLOAT,
  JACC_TYPE_DOUBLE,
  JACC_TYPE_LDOUBLE
} jacc_data_type_t;

typedef union {
  char char_literal;
  char *string_literal;

  /* Integer Numbers */
  int int_type;
  unsigned int uint_type;
  long long_type;
  unsigned long ulong_type;
  unsigned long long ulonglong_type;

  // Not supported ATM
  /* Real Numbers */
  float float_type;
  double double_type;
  long double ldouble_type;

} jacc_stored_data_t;

typedef struct {
  jacc_data_type_t type;
  jacc_stored_data_t data;
  size_t size;
} jacc_yystruct_t;

typedef union {
  jacc_yystruct_t token;
  struct astnode astnode;
} jacc_yystype_t;