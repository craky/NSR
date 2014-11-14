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
        char *tmp_str, int *token)
{
    int wait_for_work = 0, position = 0, flag = 0, rec_idx = -1,prob_counter =0;
    MPI_Status status;
    char buffer[BUFFER_LENGTH];
    char *rec_string = (char *) malloc((strings->_min_string_length+1)*sizeof(char));
    token = BLACK;
    
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
                                                memcpy(tmp_str,buffer,strings->_min_string_length+1); /* TODO: parametr tmp_str and this line there are redundant*/
                                                rec_idx = buffer[strings->_min_string_length+1];
                                                printf("I received idx %d and string \'%s\'. \n",rec_idx,rec_string);
                                                nsr_stack_push(stack,rec_idx,rec_string,strings->_min_string_length);
                                                return;
                   case MSG_WORK_NOWORK:        return;
                   
                   case MSG_TOKEN:              MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                                                if(buffer[0] == WHITE)
                                                        printf("Ou a white Token appeared.\n");
                                                break;
                                                
                   case MSG_FINISH:             printf("MSG_FINISH appeared for proc\n");
                                                MPI_Finalize();
                                                exit(0);
                                                break;
                   
                   default:printf("Error: unknowm MPI_TAG.\n");
                   
               }
           }
       }
}

void proc_com_check_idle_state(const int my_rank, const int proc_num)
{
    int iprobe_counter = 0, flag = 0, token = WHITE;
    MPI_Status status;
    char buffer[BUFFER_LENGTH];
    
    
    /* send token to the next processor */
    buffer[0] = token;
    MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
    
    while(1)
    {
        iprobe_counter++;
        if((iprobe_counter%CHECK_MSG_AMOUNT) == 0)
        {
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, 
                   &status);
            
            if(flag)
            {
                switch(status.MPI_TAG)
                {
                    case MSG_WORK_REQUEST:      MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_WORK_NOWORK,MPI_COMM_WORLD);
                                                break;
                    case MSG_TOKEN:             
                               MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                               if(buffer[0] == BLACK)
                                   return;
                               token = WHITE;
                               /* send token to the next processor */
                               buffer[0] = token;
                               MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
                               break;
                               
                    default: printf("Unexpected tag when I want to exit.\n");
                }
            }
        }
    }
}

void proc_com_check_flag(nsr_stack_t *stack, int counter, const int str_len)
{
    int flag = 0;
    MPI_Status status;
    
    /* Delay because of large mode */
    if((counter%CHECK_MSG_AMOUNT) != 0)
        return;
    
    /* Check the received messages */
    MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &flag,&status);
    
    if(flag)
    {
        switch(status.MPI_TAG)
        {
            case MSG_WORK_REQUEST:
                /* Recieve the message */
                MPI_Recv(NULL,0,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,
                        MPI_COMM_WORLD,&status);
                /* Send work to status.MPI_SOURCE */
                proc_com_send_work(stack,status.MPI_SOURCE,str_len);
                /* For better debuging I Will send FINIS msg */
                MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_FINISH,
                        MPI_COMM_WORLD);
                break;
                
            case MSG_FINISH: 
                MPI_Finalize();
                exit(0);
                break;
                
            default: printf("Unknown status.MPI_TAG for me.\n");
        }
    }
}