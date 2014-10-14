#include <stdlib.h>
#include <stdio.h>

#include "nsr_string.h"

void nsr_strings_destroy(nsr_strings_t *strings)
{
   int i;
   for (i = 0; i < strings->_count; i++)
      free(strings->_strings[i]);
   free(strings->_strings);
}
