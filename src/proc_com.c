#include "proc_com.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int acz_ahd(const int loc_counter, const int proc_sum)
{    
    return (loc_counter+1)%proc_sum;
}

void proc_com_finish_processes(void)
{
    int i = 0, proc_num = 0;
    
     /* find out number of processes */
     MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
     
     for( i = 1; i < proc_num; i++)
      MPI_Send(NULL,0,MPI_CHAR,i,MSG_FINISH,
              MPI_COMM_WORLD);
}

void proc_com_send_work(nsr_stack_t *stack, const int dest_proc, const int str_size)
{
    char *buffer = (char*) malloc((str_size+1)*sizeof(char));
    
    nsr_stack_elem_t elem = nsr_stack_pop_bottom(stack);
    memcpy(buffer,elem._string,str_size);
    buffer[str_size] = elem._idx;
    
    /* create message */
    MPI_Send(buffer,str_size+1,MPI_CHAR,dest_proc,MSG_WORK_SENT,MPI_COMM_WORLD);
    
    free(buffer);
}