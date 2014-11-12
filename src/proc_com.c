#include "proc_com.h"

int acz_ahd(const int loc_counter, const int proc_sum){    
    return (loc_counter+1)%proc_sum;
}