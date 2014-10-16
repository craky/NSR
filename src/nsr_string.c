#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "nsr_string.h"

void nsr_strings_destroy(nsr_strings_t *strings)
{
   int i;
   for (i = 0; i < strings->_count; i++)
      free(strings->_strings[i]);
   free(strings->_strings);
}

int nsr_strings_min_length(const nsr_strings_t *strings)
{
   int min_length = INT_MAX, len, i;
   for (i = 0; i < strings->_count; i++)
   {
      len = strlen(strings->_strings[i]);
      if (len < min_length)
          min_length = len;
   }
   return min_length;
}

void nsr_result_init(nsr_result_t *result, const nsr_strings_t *strings)
{
   result->_distances_count = strings->_count;
   result->_total_distance = INT_MAX;
   result->_distances = (int *) malloc(result->_distances_count * sizeof(int));
   result->_string = (char *) malloc(strings->_min_string_length + 1);
}
