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
 * @param input file to read the strings from
 * @param strings nsr_strings to store the strings to
 * @return 1 on success; false otherwise
 */
int nsr_read_strings(FILE *input, nsr_strings_t *strings);

/**
 * prints the strings
 * @strings strings to print
 */
void nsr_strings_print(const nsr_strings_t *strings);

#endif /* __NSR_IO_H__ */
