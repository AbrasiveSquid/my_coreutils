/* contains the function prototypes that support my_ls */

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct
typedef struct
{
  struct dirent *files;
  size_t file_count;
  size_t file_capacity;
  size_t field_width; // set by the max len of a file's pathname, used for
                      // foramtting
} FileList;

/*
   Compares the file names of a and b by characters.
   Returns positive int if a > b, negative value if a < b, otherwise 0.
*/
int compare_filenames(const void *a, const void *b);

struct dirent *alloc_mem_for_files(FileList *file_list);
