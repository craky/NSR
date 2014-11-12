/* Functions for processor communications */
#ifndef __PROC_COMM_H__
#define __PROC_COMM_H__

#define MSG_FINISH       1004

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
void finish_processes(void);

#endif