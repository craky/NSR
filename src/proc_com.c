#include "proc_com.h"

#include <mpi.h>

int acz_ahd(const int loc_counter, const int proc_sum)
{    
    return (loc_counter+1)%proc_sum;
}

void finish_processes(void)
{
    int i = 0, proc_num = 0;
    
     /* find out number of processes */
     MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
     
     for( i = 1; i < proc_num; i++)
      MPI_Send(NULL,0,MPI_CHAR,i,MSG_FINISH,
              MPI_COMM_WORLD);
}