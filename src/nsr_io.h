#ifndef __NSR_IO_H__
#define __NSR_IO_H__

#include "nsr_string.h"

/**
 * reads file of nsr strings in the following format:
 * N Z
 * string1
 * string2
 * ...
 * stringN
 * where Z is the maximal length of string
 */
int nsr_read_strings(FILE *input, nsr_strings_t *strings);


void nsr_strings_print(const nsr_strings_t *strings);

#endif /* __NSR_IO_H__ */
