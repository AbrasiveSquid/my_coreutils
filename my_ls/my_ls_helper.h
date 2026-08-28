/* contains the function prototypes that support my_ls */

#include <ctype.h>
#include <dirent.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

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

/*
   Get the number of columns in characters of the current terminal window

   Parameters:
    int: fd
      file descriptor of the terminal window to get the num of columns from
   Returns:
      int: of the number of columns in characters
*/
int get_window_columns(int fd);

/*
   Free up any allocated memory

   Parameters:
      files: struct dirent *
          array of structs that contains a strut of file details

      col_widths: size_t *
          array of size_t that represent the width of each column for printing
  Returns:
    NULL

  Postcondition:
      if files was allocated on the heap it will free it's memory and return
  NULL
*/
void cleanup(struct dirent *files, size_t *col_widths);

/*
   allocates or reallocs memory for an array of length size

   Parameters:
      arr: size_t *
          array of size_t
      size: size_t
          number of indices to allocate memory for

  Returns:
    size_t *:
      pointer to memory allocated
*/
size_t *alloc_sizet_array(size_t *arr, size_t size);

/*
   Prints the list of file names in a single column,
   if a filename is greater than the width,
   will wrap filename to next column.

   Parameters:
      files: struct dirent *
        an array of struct of the files in the directory to be printed

      file_count: size_t
        the number of files to be printed

      fp: FILE *
        where the files will be printed.

    Postcondition:
        Filenames of all the files in the given directory will
        be printed to the FILE * fp in a single column
*/
void print_single_column(struct dirent *files, size_t file_count, FILE *fp);
