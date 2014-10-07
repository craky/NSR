#include "char_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/******************************************************************************/
/* increment_character is used to change x th character.                      */
/* Returns char * which have hamming_distance(input, output) = 1              */

/******************************************************************************/
void increment_character(char input[], int char_size, int char_order) {

    if (char_size <= char_order) {
        printf("Increment_character error.\r\n");
        return;
    }

    input[char_order]++;

    if (input[char_order] == 'z') {
        input[char_order] = 'a';
    }
}
/******************************************************************************/
/* generate_char = is consisted of 'a'                                        */
/******************************************************************************/
char * generate_string(int size){
    char * new_string = (char *) malloc(size);
    int i = 0;

    for(i = 0; i < size; i++){
        new_string[i] = 'a';
    }

    return new_string;
}
/******************************************************************************/
/* print char of size = size to an output                                     */
/******************************************************************************/
void print_string(const char input[], int size){
    int i = 0;

    for(i = 0; i < size; i++){
        printf("%c", input[i]);
    }

    printf("\n");
}
/******************************************************************************/
/* all_words_rec - write to an output all words with size = size;             */
/******************************************************************************/
void all_words_rec(char input[], int input_length, int idx){
   int nchars = 'z' - 'a' + 1;

   if (idx == input_length)
      return;

   input[idx] = 'a';

   while (nchars--) {
      if (idx == input_length - 1)
         print_string(input, input_length);
      all_words_rec(input, input_length, idx + 1);
      input[idx]++;
   }
}


