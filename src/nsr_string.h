#ifndef __NSR_STRING_H__
#define __NSR_STRING_H__

typedef struct nsr_strings
{
   unsigned _count;
   unsigned _max_string_length;
   unsigned _min_string_length;
   char **_strings;
} nsr_strings_t;

typedef struct nsr_result
{
   int _max_distance;
   char *_string;
   int *_distances;
   int _distances_count;
} nsr_result_t;

/**
 * deletes the strings
 * @param strings nsr_strings to delete
 */
void nsr_strings_destroy(nsr_strings_t *strings);

/**
 * returns the length of the shortest string in `strings'
 * @param strings the set of input strings
 * @return the length of the shortest string
 */
int nsr_strings_min_length(const nsr_strings_t *strings);

/**
 * initializes a nsr_result
 * @param result result to initialize
 * @param strings the set of input strings
 */
void nsr_result_init(nsr_result_t *result, const nsr_strings_t *strings);

/**
 * deletes the nsr_result
 * @param result result to delete
 */
void nsr_result_destroy(nsr_result_t *result);

#endif /* __NSR_STRING_H__ */
