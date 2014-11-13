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

void proc_com_ask_for_work(nsr_stack_t *stack,const nsr_strings_t *strings,
        char *tmp_str)
{
    int wait_for_work = 0, position = 0, flag = 0, rec_idx = -1,prob_counter =0;
    MPI_Status status;
    char buffer[BUFFER_LENGTH];
    char *rec_string = (char *) malloc((strings->_min_string_length+1)*sizeof(char));
    
    while(1)
       {
           if(!wait_for_work)
           {
                MPI_Send(buffer,position,MPI_CHAR,0,MSG_WORK_REQUEST,MPI_COMM_WORLD);
                wait_for_work = 1;
           }
           
           prob_counter++;
           
           if((prob_counter%CHECK_MSG_AMOUNT) == 0)
                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, 
                                &status);
           
           if(flag)
           {
               switch(status.MPI_TAG)
               {
                   case MSG_WORK_REQUEST:       break;
                   case MSG_WORK_SENT:          MPI_Recv(&buffer,strings->_min_string_length+1+1,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                                                memcpy(rec_string,buffer,strings->_min_string_length+1);
                                                memcpy(tmp_str,buffer,strings->_min_string_length+1);
                                                rec_idx = buffer[strings->_min_string_length+1];
                                                printf("I received idx %d and string \'%s\'. \n",rec_idx,rec_string);
                                                rec_idx++; /* Need next idx char */
                                                nsr_stack_push(stack,rec_idx,rec_string,strings->_min_string_length);
                                                return;
                   case MSG_WORK_NOWORK:        break;
                   case MSG_TOKEN:              break;
                   case MSG_FINISH:             printf("MSG_FINISH appeared for proc\n");
                                                MPI_Finalize();
                                                exit(0);
                                                break;
                   
                   default:printf("Error: unknowm MPI_TAG.\n");
                   
               }
           }
       }
}