#ifndef __CHAR_OPERAIONS_H__
#define __CHAR_OPERAIONS_H__
#include "nsr_string.h"

char *generate_string(int length, char fill_char);
void all_words_rec(char input[],nsr_strings_t *strings, int input_length,
        int idx, int min_dist,nsr_result_t *result);
void print_string(const char input[], int size);
int hamming_dist (const char *str1, const char *str2);
int get_worst_dist(const nsr_strings_t * strings, const char *input);
void set_distances(const nsr_strings_t *strings, const char *input,
        nsr_result_t *result);
nsr_result_t* nsr_solve(const nsr_strings_t *strings);

#endif /* __CHAR_OPERAIONS_H__ */
