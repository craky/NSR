#include "proc_com.h"

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int acz_ahd(const int my_rank, const int loc_counter, const int proc_sum)
{    
    /* processor should not ask itself to be a work donor */
    if(my_rank == ((loc_counter+1)%proc_sum))
        return (loc_counter+2)%proc_sum;
        
    return (loc_counter+1)%proc_sum;
}

void proc_com_finish_processes(const int str_len, nsr_result_t *result,const nsr_strings_t *strings)
{
    int i = 0, proc_num = 0, answers_rec = 0, delay_counter = 0, flag = 0,
            rec_dist;
    MPI_Status status;
    static char buffer[BUFFER_LENGTH];
    char *rec_str = (char *) malloc((str_len+1)*sizeof(char));
    
     /* find out number of processes */
     MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
     
     answers_rec = proc_num -1;
     
     for( i = 1; i < proc_num; i++)
      MPI_Send(NULL,0,MPI_CHAR,i,MSG_FINISH,
              MPI_COMM_WORLD);
     
     /* get results from other processes */
     while(answers_rec)
     {
         delay_counter++;
         if((delay_counter % CHECK_MSG_AMOUNT) == 0)
         {
             MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, 
                                &status);
             if(flag)
             {
                 if(status.MPI_TAG == MSG_WORK_SENT)
                 {
                     MPI_Recv(&buffer,str_len+2,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                     if(buffer[str_len+1] == CHAR_MAX)
                     {
                         answers_rec--;
                         continue;
                     }
                     memcpy(rec_str,buffer,str_len+1);
                     rec_dist = buffer[str_len+1];
                     if(rec_dist < result->_max_distance)
                     {
                         memcpy(result->_string, rec_str, strings->_min_string_length + 1);
                         result->_max_distance = rec_dist;
                     }
                     answers_rec--;
                 }
                 else
                 {
                     /* Ignore other tags */
                     MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                 }
             }
         }
     }
     
     free(rec_str);
}

void proc_com_send_work(nsr_stack_t **stack, const int dest_proc, const int str_size)
{
    char *buffer = (char*) malloc((BUFFER_LENGTH)*sizeof(char));
    int i = 0,buff_offset = 0;
    nsr_stack_elem_t elem;
    
    /* how much elems will be send */
    buffer[0] = nsr_stack_get_size(*stack)/2;
    /* If there are some buffer size problems */
    if(buffer[0] >= BUFFER_LENGTH)
        printf("BUFFER_LENGTH is not big enough.\n "
                "BUFFER_LENGTH = %d,\n"
                "nsr_stack_get_size = %d,\n"
                "nsr_stack_get_size/2 = %d.\n",BUFFER_LENGTH,
                nsr_stack_get_size(*stack),(nsr_stack_get_size(*stack)/2));
    
    buff_offset = str_size+1;
   
    
    for( i = 0; i < buffer[0];i++)
    {
        elem = nsr_stack_pop_bottom(*stack);
        memcpy(buffer+(i*buff_offset+1),elem._string,str_size);
        buffer[(i+1)*buff_offset] = elem._idx;      
    }
     /* create message */
        MPI_Send(buffer,BUFFER_LENGTH,MPI_CHAR,dest_proc,MSG_WORK_SENT,MPI_COMM_WORLD);
        
    free(buffer);
}
int proc_com_zero_ask_for_work(nsr_stack_t *stack, 
        const nsr_strings_t *strings, const int donor)
{
    int iprobe_delay = 0, flag, elem_sum, rec_idx = -1, buff_offset, i = 0,
            position = 0;
    char *rec_string = (char *) malloc((strings->_min_string_length+1)*sizeof(char));
    static char buffer[BUFFER_LENGTH];
    MPI_Status status;
    
    /* send request for work to processor 'donor' */
    MPI_Send(buffer,position,MPI_CHAR,donor,MSG_WORK_REQUEST,MPI_COMM_WORLD);
    /* active waiting until message not arrive */
    
    while(1)
    {        
        if((iprobe_delay%CHECK_MSG_AMOUNT) == 0)
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, 
                                &status);
        iprobe_delay++;
        
        if(!flag)
            continue;
        
      switch(status.MPI_TAG)
      {
          case MSG_WORK_SENT:
              MPI_Recv(&buffer,BUFFER_LENGTH,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
              elem_sum = buffer[0];
              buff_offset = strings->_min_string_length + 2;
                               
              for(i = 0; i < elem_sum; i++)
              {
                  memcpy(rec_string,buffer+(i*buff_offset+1),strings->_min_string_length+1);
                  rec_idx = buffer[(i+1)*buff_offset];
                  nsr_stack_push(stack,rec_idx,rec_string,strings->_min_string_length);
              }
              return 1;
            
          case MSG_WORK_NOWORK:
              MPI_Recv(NULL,0,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
              return 0;
            
          case MSG_WORK_REQUEST:
              MPI_Recv(NULL,0,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
              MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_WORK_NOWORK,MPI_COMM_WORLD);
              flag = 0;
              break;
            
          default: printf("Proc [0], unknown request for %d.\n",status.MPI_TAG);
      }
    }
    
    printf("ERROR: Function proc_com_zero_ask_for_work. Unexpected end.\n");
    return -1;
}
void proc_com_ask_for_work(nsr_stack_t *stack,const int donor, 
        const nsr_strings_t *strings, int *token, nsr_result_t *result, 
        int debug_counter, const int token_rec)
{
    int wait_for_work = 0, position = 0, flag = 0, rec_idx = -1,prob_counter =0;
    int my_rank, proc_num, elem_sum = 0, buff_offset = 1, tmp_donor = donor, i = 0;
    MPI_Status status;
    static char buffer[BUFFER_LENGTH];
    char *rec_string = (char *) malloc((strings->_min_string_length+1)*sizeof(char));
   
    
   /* find out process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   
   /* find out number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    
    while(1)
       {
           /* processor received token while it was working */
           if(token_rec != TOKEN_NOT_REC)
           {
               if(token == BLACK)
                   buffer[0] = BLACK;
               else
                   buffer[0] = token_rec;
                MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
           }
           
           if(!wait_for_work)
           {    /* there can be troubles with donor */
                MPI_Send(NULL,position,MPI_CHAR,tmp_donor,MSG_WORK_REQUEST,MPI_COMM_WORLD);
                wait_for_work = 1;
           }
           
           prob_counter++;
           
           if((prob_counter%CHECK_MSG_AMOUNT) == 0)
           {
                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, 
                                &status);
                prob_counter = 0;
           }
           
             if(flag)
             {
                 switch(status.MPI_TAG)
                 {
                     case MSG_WORK_REQUEST:
                         MPI_Recv(NULL,0,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                         MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_WORK_NOWORK,MPI_COMM_WORLD);
                         break;
                       
                     case MSG_WORK_SENT:
                         MPI_Recv(&buffer,BUFFER_LENGTH,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                         elem_sum = buffer[0];
                         buff_offset = strings->_min_string_length + 2;
                               
                         for(i = 0; i < elem_sum; i++)
                         {
                          memcpy(rec_string,buffer+(i*buff_offset+1),strings->_min_string_length+1);
                          rec_idx = buffer[(i+1)*buff_offset];
                          nsr_stack_push(stack,rec_idx,rec_string,strings->_min_string_length);
                         }
                         return;
                       
                     case MSG_WORK_NOWORK: /* What to do if there is no work */
                         MPI_Recv(NULL,0,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                         /* lets send request again */
                         wait_for_work = 0;
                         tmp_donor = acz_ahd(my_rank,tmp_donor,proc_num);
                         break;
                   
                     case MSG_TOKEN:          
                         MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                       
                         if(buffer[0] == BLACK)
                           buffer[0] = BLACK; /* TODO buffer is already black */
                         else
                           buffer[0] = *token; 
                       
                         MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
                         *token = WHITE;
                         break;
                                                
                      case MSG_FINISH:
                         printf("[%d] I have done %d things.\n",my_rank,debug_counter);
                         memcpy(buffer,result->_string,strings->_min_string_length+1);
                         /* Did not received any data from start*/
                         if(result->_max_distance == INT_MAX)
                             buffer[strings->_min_string_length+1] = CHAR_MAX;
                         else
                             buffer[strings->_min_string_length+1] = result->_max_distance;
                         MPI_Send(buffer,strings->_min_string_length+2,MPI_CHAR,0,MSG_WORK_SENT,MPI_COMM_WORLD);
                         MPI_Finalize();
                         exit(0);
                         break;
                   
                     default:printf("Error: unknowm MPI_TAG [%d].\n",status.MPI_TAG);
                   
                 }
             }
           flag = 0;
           
       }
}

void proc_com_check_idle_state(const int my_rank, const int proc_num)
{
    int iprobe_counter = 0, flag = 0, token = WHITE;
    MPI_Status status;
    static char buffer[BUFFER_LENGTH];
    
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
                    case MSG_WORK_REQUEST:      
                        MPI_Recv(NULL,0,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                        MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_WORK_NOWORK,MPI_COMM_WORLD);
                        break; 
                        
                    case MSG_TOKEN:             
                               MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                               if(buffer[0] == WHITE)
                                   return;
                               /* send token to the next processor */
                               buffer[0] = WHITE;
                               MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
                               break;
                               
                    default: printf("Unexpected tag when I want to exit.\n");
                }
            }
        }
    }
}

void proc_com_check_flag(nsr_stack_t *stack, int *token, int counter, const int str_len,
        int my_rank, int proc_num, int *token_rec)
{
    int flag = 0;
    static char buffer[BUFFER_LENGTH];
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
                /* Check if exist some work */
                if(nsr_stack_empty(stack) || nsr_stack_get_size(stack) < 3)
                {
                    MPI_Send(NULL,0,MPI_CHAR,status.MPI_SOURCE,MSG_WORK_NOWORK,MPI_COMM_WORLD);
                    break;
                }
                /* Send work to status.MPI_SOURCE */
                if(status.MPI_SOURCE < my_rank)
                    *token = BLACK;
                proc_com_send_work(&stack,status.MPI_SOURCE,str_len);
                break;
                
            case MSG_TOKEN:
                MPI_Recv(&buffer,1,MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                
                /* token_rec is color of received token while proc. was working */
                *token_rec = buffer[0];
                /* In this state I'm bussy */
                //buffer[0] = BLACK;
                //MPI_Send(buffer,1,MPI_INT,(my_rank+1)%proc_num,MSG_TOKEN,MPI_COMM_WORLD);
                break;
                
            default: printf("[%d]: Unknown status.MPI_TAG for me. Tag is %d from source %d\n",my_rank, status.MPI_TAG, status.MPI_SOURCE);
            //default: break;
        }
    }
}