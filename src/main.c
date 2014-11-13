/******************************************************************************/
/* MI-PPR.2 - NSR                                                             */
/****************                                                             */
/* @author krakovoj                                                           */
/* @author tatarsan                                                           */
/******************************************************************************/
#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>
#include <string.h>

#include "char_operations.h"
#include "nsr_string.h"
#include "nsr_io.h"

/* MPI message tags */
#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

int main(int argc, char **argv)
{
   FILE *input;
   nsr_strings_t *strings;
   nsr_result_t *result;
   int proc_num, my_rank, i = 0;
   
   /* starts MPI */
   MPI_Init(&argc,&argv);
   
    /* find out process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   
   /* find out number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
   
   
     strings = (nsr_strings_t *) malloc(sizeof(nsr_strings_t));
     if (argc != 2)
     {
        fprintf(stderr, "usage: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
     }

     input = fopen(argv[1], "r");
 
     nsr_read_strings(input, strings);

     result = nsr_solve(strings); 

     printf("Result string is \'%s\' with total distance %d.\n", result->_string,
           result->_max_distance);
     for(i = 0; i < strings->_count; i++)
         printf(" hamming_dist(%s,%s) = %d\n", strings->_strings[i],
                 result->_string, result->_distances[i]);
   
     nsr_strings_destroy(strings);
     free(strings);
     nsr_result_destroy(result);
     free(result);
  
     fclose(input);

   MPI_Finalize(); /* ends MPI*/
   return EXIT_SUCCESS;
}
