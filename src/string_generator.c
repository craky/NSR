#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <tgmath.h>

int main (int argc, char **argv)
{
   int n, z, i, logz, len, j;
   if (argc != 3)
   {
      fprintf(stderr, "usage: %s n z\n", *argv);
      return EXIT_FAILURE;
   }

   srand(time(NULL));

   n = atoi(argv[1]);
   z = atoi(argv[2]);

   logz = log(z);

   printf("%d %d\n", n, z);
   for (i = 0; i < n; i++)
   {
      len = logz + rand() % (z - logz + 1);
      for (j = 0; j < len; j++)
         printf("%c", 'a' + rand() % ('z' - 'a' + 1));
      printf("\n");
   }

   return EXIT_SUCCESS;
}
