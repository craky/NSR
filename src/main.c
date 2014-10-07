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

#define TESTING_INPUT_SIZE 10                   /* n >= 10 */
#define TESTING_MINIMAL_STRING_SIZE             /* minimal is log(n) */

int main(int argc, char *argv[]) {
    int result;
    char * input2 = generate_string(7);

    printf("Generated char is %s \n", input2);
    result = log(TESTING_INPUT_SIZE);
    printf("log(%d) = %d\n", TESTING_INPUT_SIZE, result);

    all_words_rec(input2,5,0);

    free(input2);
    return 0;
}
