#include <stdio.h>
#include <stdlib.h>

#include "nsr_io.h"


#define NSR_IO_MAX_LINE_LENGTH 64

int nsr_read_strings(FILE *input, nsr_strings_t *strings)
{
   char line[NSR_IO_MAX_LINE_LENGTH];
   int i;

   if (!fgets(line, NSR_IO_MAX_LINE_LENGTH, input))
   {
      fprintf(stderr, "invalid input format");
      return 0;
   }

   if (sscanf(line, "%u %u ", &strings->_count, &strings->_max_string_length) != 2)
   {
      fprintf(stderr, "invalid input format");
      return 0;
   }

   strings->_strings = (char **) malloc(sizeof(char *) * strings->_count);
   for (i = 0; i < strings->_count; i++)
      strings->_strings[i] = (char *) malloc(strings->_max_string_length + 1);

   for (i = 0; i < strings->_count; i++)
   {
      if (!fgets(line, NSR_IO_MAX_LINE_LENGTH, input))
      {
         fprintf(stderr, "invalid input format");
         return 0;
      }
      if (sscanf(line, "%s ", strings->_strings[i]) != 1)
      {
         fprintf(stderr, "invalid input format");
         return 0;
      }
   }

   return 1;
}

void nsr_strings_print(const nsr_strings_t *strings)
{
   int i;
   printf("%d %d\n", strings->_count, strings->_max_string_length);
   for (i = 0; i < strings->_count; i++)
      printf("%d %s\n", i, strings->_strings[i]);
}
