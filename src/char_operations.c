#include "char_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/******************************************************************************/
/* generate_char = is consisted of 'a'                                        */
/******************************************************************************/
char * generate_string(int size){
    char * new_string = (char *) malloc(size + 1);
    memset(new_string, 'a', size);
    new_string[size] = 0;
    return new_string;
}
/******************************************************************************/
/* all_words_rec - write to an output all words with size = size;             */
/******************************************************************************/
void all_words_rec(char input[], int input_length, int idx){
   int nchars = 'z' - 'a' + 1;

   if (idx == input_length)
      return;

   input[idx] = 'a';

   while (nchars--) {
      if (idx == input_length - 1)
         printf("%s\n", input);
      all_words_rec(input, input_length, idx + 1);
      input[idx]++;
   }
}

int hamming_dist (const char *str1, const char *str2)
{
   const char *shorter, *longer;
   int i, min_dist = INT_MAX, shifts, shift_dist;

   int str1_len = strlen(str1);
   int str2_len = strlen(str2);

   longer = str1;
   shorter = str2;

   if (str2_len > str1_len)
   {
      longer = str2;
      shorter = str1;
   }

   shifts = abs(str1_len - str2_len) + 1;
   for (i = 0; i < shifts; i++)
   {
      shift_dist = 0;
      str2 = shorter;
      for (str1 = longer + i; *str2 && shift_dist < min_dist; str1++, str2++)
         shift_dist += *str1 != *str2;
      if (shift_dist < min_dist)
         min_dist = shift_dist;
   }
   return min_dist;
}

