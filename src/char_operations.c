#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <mpi.h>

#include "char_operations.h"
#include "nsr_stack.h"
#include "proc_com.h"

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
   char *rec_string = (char *) malloc((strings->_min_string_length+1)*sizeof(char));
   nsr_stack_t stack;
   nsr_stack_elem_t elem;
   nsr_result_t *result;
   int tmp_dist, min_dist = INT_MAX; 
   int my_rank = 0,i = 0, token = BLACK, proc_num = 0;
   
    /* find out process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   
   /* find out number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
   
   /* Stack code */
   result = (nsr_result_t *) malloc(sizeof(nsr_result_t));
   nsr_result_init(result, strings);
   nsr_stack_init(&stack);

   tmp_str = generate_string(strings->_min_string_length, 'a');

   /* Only the first process will start calculation */
   if(my_rank == 0)
        nsr_stack_push(&stack, -1, tmp_str, strings->_min_string_length);
   
   tmp_str[0] = 'a';
   
   /* All processes (except 0) are waiting for work from proc. 0 */
   if(my_rank != 0)
       proc_com_ask_for_work(&stack,strings,tmp_str,&token);
  
   while(!nsr_stack_empty(&stack))
   {
       elem = nsr_stack_pop(&stack);
       
       /* Do not add to stack */
       if(elem._idx+1 == strings->_min_string_length)
       {
           /* check distances */
         tmp_dist = get_maximum_dist(strings, tmp_str);
         if (tmp_dist < min_dist)
         {
            min_dist = tmp_dist;
            memcpy(result->_string, tmp_str, strings->_min_string_length + 1);
            result->_max_distance = tmp_dist;
            set_distances(strings, tmp_str, result);
         }
         /* get next elem from stack */
         continue;
       }
       memcpy(tmp_str,elem._string,strings->_min_string_length+1);
       
       for(i = 0; i < (CHARS_IN_ALPHABET+1); i++)
       {
           tmp_str[elem._idx+1] = 'z' - i;
           nsr_stack_push(&stack,elem._idx+1,tmp_str,strings->_min_string_length);
       }
       nsr_stack_print(&stack);
   }
   
   if(my_rank != 0)
   {
       printf("Accidentaly at the end, stack empty is %d.\n",nsr_stack_empty(&stack));
   }
   /* Sorry, but i needed to delete these free(tmp_string) but dont know why */
   nsr_stack_destroy(&stack);
   
   if(my_rank == 0)
   {
      // proc_com_check_idle_state(my_rank,proc_num);
       proc_com_finish_processes();
   }
   free(rec_string);
    return result;
}


nsr_result_t *mpi_nsr_solve(const nsr_strings_t *strings)
{
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
