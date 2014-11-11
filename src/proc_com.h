/* Functions for processor communications */
#ifndef __PROC_COMM_H__
#define __PROC_COMM_H__

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
int acz_ahd(const int loc_counter, const int proc_sum){    
    return (loc_counter+1)%proc_sum;
}

#endif