#pragma once

#include <stddef.h>

typedef enum jacc_data_type {
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

typedef union jacc_stored_data {
    char char_d;
    char *string_d;

    /* Integer Numbers */
    int int_d;
    unsigned int uint_d;
    long long_d;
    unsigned long ulong_d;
    unsigned long long ulonglong_d;

    // Not supported ATM
    /* Real Numbers */
    float float_d;
    double double_d;
    long double ldouble_d;
} jacc_stored_data_t;

typedef struct jacc_lex_tok {
    jacc_data_type_t data_type;
    size_t size;
    jacc_stored_data_t data;
} jacc_lex_tok_t;