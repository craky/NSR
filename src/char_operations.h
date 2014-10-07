#ifndef __CHAR_OPERAIONS_H__
#define __CHAR_OPERAIONS_H__


char * generate_string(int size);
void all_words_rec(char input[], int input_length, int idx);
void print_string(const char input[], int size);
int hamming_dist (const char *str1, const char *str2);

#endif /* __CHAR_OPERAIONS_H__ */
