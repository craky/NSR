/* Functions for processor communications */
#ifndef __PROC_COMM_H__
#define __PROC_COMM_H__

#include "nsr_stack.h"
#include "nsr_string.h"

#define CHECK_MSG_AMOUNT  100
#define BUFFER_LENGTH 100

/* MPI message tags */
#define MSG_WORK_REQUEST 1000
#define MSG_WORK_SENT    1001
#define MSG_WORK_NOWORK  1002
#define MSG_TOKEN        1003
#define MSG_FINISH       1004

/* Token colors */
#define BLACK  0
#define WHITE 1

/**
 * 
 * Algorithms for donor search
 * Name of function is based on Czech language for consistency with source:
 * https://edux.fit.cvut.cz/courses/MI-PPR.2/labs/prohledavani_do_hloubky
 * 
 * @param loc_counter
 * @param proc_sum
 * @return Next donor id
 */
int acz_ahd(const int loc_counter, const int proc_sum);

/**
 * Will finish all processes except the process 0
 * Needs to be after MPI_Init() and before MPI_Finalize()
 */
void proc_com_finish_processes(void);

/**
 * Sends work to the destination processor.
 * @param stack
 * @param dest_proc destination processor
 * @param str_size  minimal string size
 */
void proc_com_send_work(nsr_stack_t *stack, const int dest_proc, const int str_size);

/**
 * Ask 0 processor for work
 * @param stack 
 * @param strings
 * @param tmp_str from char_operatioons.h
 */
void proc_com_ask_for_work(nsr_stack_t *stack,const nsr_strings_t *strings,
        char *tmp_str, int *token);

/**
 * Receive a token a send yours, to the next processor
 * @param my_rank 
 * @param proc_num
 */
void proc_com_check_idle_state(const int my_rank, const int proc_num);

#endif