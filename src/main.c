/******************************************************************************/
/* MI-PPR.2 - NSR                                                             */
/****************                                                             */
/* @author krakovoj                                                           */
/* @author tatarsan                                                           */
/******************************************************************************/
#include <stdio.h>
#include <tgmath.h>
#include <stdlib.h>
#include "char_operations.h"
#include "nsr_string.h"
#include "nsr_io.h"

#define TESTING_INPUT_SIZE 10                   /* n >= 10 */
#define TESTING_MINIMAL_STRING_SIZE             /* minimal is log(n) */

/*
int main(int argc, char *argv[]) {
    int result;
    char * input2 = generate_string(3);

    printf("Generated char is %s \n", input2);
    result = log(TESTING_INPUT_SIZE);
    printf("log(%d) = %d\n", TESTING_INPUT_SIZE, result);

    all_words_rec(input2,3,0);

    printf("%d\n", hamming_dist("abcd", "aec"));

    free(input2);
    return 0;
}
*/

int main(int argc, char **argv)
{
   FILE *input;
   nsr_strings_t *strings;
   strings = (nsr_strings_t *) malloc(sizeof(nsr_strings_t));
   if (argc != 2)
   {
      fprintf(stderr, "usage: %s input_file\n", argv[0]);
      return EXIT_FAILURE;
   }

   input = fopen(argv[1], "r");

   nsr_read_strings(input, strings);
   nsr_strings_print(strings);
   nsr_strings_destroy(strings);
   free(strings);

   fclose(input);

   return EXIT_SUCCESS;
}
