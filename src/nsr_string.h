#ifndef __NSR_STRING_H__
#define __NSR_STRING_H__

typedef struct nsr_strings
{
   unsigned _count;
   unsigned _max_string_length;
   char **_strings;
} nsr_strings_t;

void nsr_strings_destroy(nsr_strings_t *strings);


#endif /* __NSR_STRING_H__ */
