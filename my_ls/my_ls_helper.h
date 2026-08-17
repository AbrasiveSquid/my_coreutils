/* contains the function prototypes that support my_ls */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
   Compares the file names of a and b by characters.
   Returns positive int if a > b, negative value if a < b, otherwise 0.
*/
int compare_filenames(const void *a, const void *b);
