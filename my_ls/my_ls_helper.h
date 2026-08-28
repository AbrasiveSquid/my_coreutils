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
  struct dirent *files; // array of struct dirent *
  size_t file_count;    // number of struct dirent * in files
  size_t file_capacity; // amount of struct dirent * allocated
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
      file_list: FileList *
          a structure that contains an array of struct dirent * and other attributes related
          to the directory

      col_widths: size_t *
          array of size_t that represent the width of each column for printing
  Returns:
    NULL

  Postcondition:
      Will free the file_list->files array, the FileList *, and the col_widths *
  NULL
*/
void cleanup(FileList *file_list, size_t *col_widths);

/*
   allocates or reallocs memory for an array of length size

   Parameters:
      arr: void *
          array of size_t
      size: size_t
          number of indices to allocate memory for
      elem_size:
          the size in bytes of each element in arr

  Returns:
    void *:
      pointer to memory allocated of whatever type passed as first param
*/
void *alloc_array(void *arr, size_t size, size_t elem_size);

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

/*
  Reads a diretory pathname and cretes a structure FileList pointer that
  contains details of those that directory, including file details and number of
  files

  Parameters:
    None


  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent *
        file_capacity: size_t of the amount of struct dirent * files has memory
  allocate for file_count: size_t of the number of struct dirent * in files
*/
FileList *create_file_list(char *pathname);
