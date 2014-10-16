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
    int _total_distance;
    char *_string;
    int *_distances;
    int _distances_count;
} nsr_result_t;

void nsr_strings_destroy(nsr_strings_t *strings);
int nsr_strings_min_length(const nsr_strings_t *strings);
void nsr_result_init(nsr_result_t *result, const nsr_strings_t *strings);

#endif /* __NSR_STRING_H__ */
