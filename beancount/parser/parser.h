#ifndef BEANCOUNT_PARSER_H
#define BEANCOUNT_PARSER_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "beancount/parser/macros.h"

/* This typedef is included in the Flex generated lexer.h header, but
 * it is needed in the Bison generated header grammar.h that needs to
 * be included before lexer.h. Repeat it here and make sure to include
 * the headers in the right order. */
typedef void* yyscan_t;

/* A global constant pointing to the special 'MISSING' object. */
extern PyObject* missing_obj;

C_BEGIN_DECLS

/**
 * Convert an ASCII string to a PyDate object.
 *
 * The @string is assumed to be a valid date represetation in the
 * format YYYY-MM-DD allowing for any character to divide the three
 * digits groups.
 */
PyObject* pydate_from_cstring(const char* string);

C_END_DECLS

#endif /* BEANCOUNT_PARSER_H */
