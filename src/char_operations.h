#ifndef __CHAR_OPERAIONS_H__
#define __CHAR_OPERAIONS_H__
#include "nsr_string.h"

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

#define CHARS_IN_ALPHABET 25

/**
 * generates a string of length `length' filled with `fill_char' char
 * @param length length of the output string
 * @param fill_char char to fill the string
 * @return the generated string
 */
char *generate_string(int length, char fill_char);

/**
 * recursive nsr solver
 * @param input temporary string used to generate all strings from
          {[a-z]^input_length}
 * @param strings the set of input strings to which we want to find the
          nearest string
 * @param input_length length of the temporary `input' string
 * @param idx current index
 * @param result the best result found so far
 */
void all_words_rec(char input[], nsr_strings_t *strings, int input_length,
        int idx, nsr_result_t *result);

/**
 * computes hamming distance between two given strings
 * @param str1 the first string
 * @param str2 the second string
 * @return the hamming distance between `str1' and `str2'
 */
int hamming_dist (const char *str1, const char *str2);

/**
 * returns the maximum hamming distance between `input' and all strings from
 * `strings'
 * @param strings the set of input strings
 * @param input string to find the maximum distance to `strings'
 * @return the maximum distance
 */
int get_maximum_dist(const nsr_strings_t *strings, const char *input);

/**
 * for each string in `strings' this functions sets the distance to the
 * string `input' to the nsr_result
 * @param strings the set of input strings
 * @param input string to set the distances from
 * @param result nsr_result to set the distances in
 */
void set_distances(const nsr_strings_t *strings, const char *input,
        nsr_result_t *result);

/**
 * iterative nsr solver
 * @param strings the set of input strings
 * @return the best result found
 */
nsr_result_t *nsr_solve(const nsr_strings_t *strings);
/**
 * iterative parallel nsr solver, powered by Open MPI
 * @param strings the set of input strings
 * @return the best result found
 */
nsr_result_t *mpi_nsr_solve(const nsr_strings_t *strings);

#endif /* __CHAR_OPERAIONS_H__ */
