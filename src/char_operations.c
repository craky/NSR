#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <mpi.h>

#include "char_operations.h"
#include "nsr_stack.h"

char *generate_string(int length, char fill_char)
{
    char *new_string = (char *) malloc(length + 1);
    memset(new_string, fill_char, length);
    new_string[length] = 0;
    return new_string;
}

void all_words_rec(char input[], nsr_strings_t *strings,
        int input_length, int idx, nsr_result_t *result)
{
    int nchars = 'z' - 'a' + 1;
    int tmp_dist = 0;

    if (idx == input_length)
        return;

    input[idx] = 'a';

    while (nchars--)
    {
        tmp_dist = get_maximum_dist(strings, input);
        if (tmp_dist < result->_max_distance)
        {
            result->_max_distance = tmp_dist;
            memcpy(result->_string, input, input_length);
            set_distances(strings, input, result);
        }

        if (idx == input_length - 1)
            printf("%s [%d]\n", input,tmp_dist);
        all_words_rec(input, strings, input_length, idx + 1, result);
        input[idx]++;
    }
}


nsr_result_t *nsr_solve(const nsr_strings_t *strings)
{
   char *tmp_str;
   nsr_stack_t stack;
   nsr_stack_elem_t elem;
   nsr_result_t *result;
   int nchars, tmp_dist, prev_idx = 0, min_dist = INT_MAX;

   result = (nsr_result_t *) malloc(sizeof(nsr_result_t));
   nsr_result_init(result, strings);
   nsr_stack_init(&stack);

   tmp_str = generate_string(strings->_min_string_length, 'a');

   nsr_stack_push(&stack, -1);
   tmp_str[0] = 'a';
   while (!nsr_stack_empty(&stack))
   {
      elem = nsr_stack_pop(&stack);
      if (elem._idx == strings->_min_string_length)
      {
         prev_idx = elem._idx;
         continue;
      }

      if (prev_idx < elem._idx)
      {
         if (elem._idx == strings->_min_string_length - 1)
            tmp_str[elem._idx] = 'a';
         else
            tmp_str[elem._idx] = 'a' - 1;
      }

      for (nchars = 'z' - 'a' + 1; nchars--; )
        nsr_stack_push(&stack, elem._idx + 1);

      if (elem._idx == strings->_min_string_length - 1)
      {
         tmp_dist = get_maximum_dist(strings, tmp_str);
         if (tmp_dist < min_dist)
         {
            min_dist = tmp_dist;
            memcpy(result->_string, tmp_str, strings->_min_string_length + 1);
            result->_max_distance = tmp_dist;
            set_distances(strings, tmp_str, result);
         }
      }
      if (elem._idx >= 0)
         tmp_str[elem._idx]++;
      prev_idx = elem._idx;
   }

   free(tmp_str);
   nsr_stack_destroy(&stack);
   return result;
}
nsr_result_t *mpi_nsr_solve(const nsr_strings_t *strings){
    MPI_Init(NULL,NULL); /* MPI_Init arguments are MPI_Init (&argc, &argv);*/
    
    MPI_Finalize(); /* ends MPI*/
    return NULL;
}
int hamming_dist(const char *str1, const char *str2)
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


int get_maximum_dist(const nsr_strings_t *strings, const char *input)
{
    int i = 0;
    int maximum_dist = 0;
    int tmp_dist = 0;

    for(i = 0; i < strings->_count; i++)
    {
        tmp_dist = hamming_dist(strings->_strings[i],input);
        if(maximum_dist < tmp_dist)
                maximum_dist = tmp_dist;
    }

    return maximum_dist;
}

void set_distances(const nsr_strings_t *strings, const char *input,
        nsr_result_t *result)
{
    int i = 0;

    for(i = 0; i < strings->_count; i++)
    {
        result->_distances[i] = hamming_dist(strings->_strings[i],
                input);
    }
}
