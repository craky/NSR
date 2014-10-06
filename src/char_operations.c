#include "char_operations.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/* increment_character is used to change x th character.                      */
/* Returns char * which have hamming_distance(input, output) = 1              */

/******************************************************************************/
void increment_character(char input[], const int char_size, const int char_order) {

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
char * generate_char(const int size){
    char * new_char = (char *) malloc(size);
    int i = 0;
    
    for(i = 0; i < size; i++){
        new_char[i] = 'a';
    }
    
    return new_char;
}
/******************************************************************************/
/* print char of size = size to an output                                     */
/******************************************************************************/
void print_char(char input[], const int size){
    int i = 0;
    
    
    for(i = 0; i < size; i++){
        printf("%c", input[i]);
    }
    
    printf("\n");
}
/******************************************************************************/
/* all_words_rec - write to an output all words with size = size;             */
/******************************************************************************/
void all_words_rec(char input[], int num_char, const int size){
    int i = 0;
    
    if(num_char == size){
        return;
    }
    input[num_char] = 'a';
    
    for(i = 'a'; i <= 'z';i++){
        all_words_rec(input,num_char+1,size);
        input[num_char]++;
        printf("I = %d ",i);
        print_char(input,size);
    }
}