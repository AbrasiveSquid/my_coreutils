/* contains the function prototypes that support my_ls */

#include <ctype.h>
#include <dirent.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// structs

// pathname - contains details of the pathnames to directories whose contents will be listed
typedef struct
{
  char **pathnames;
  size_t count;
  size_t capacity;
} PathNames;

// contains details of files to be printed
typedef struct
{
  struct dirent *files; // array of struct dirent
  size_t file_count;    // number of struct dirent  in files
  size_t file_capacity; // amount of struct dirent  allocated
} FileList;

// attributes of number of columns and rows to be printed
typedef struct
{
  size_t total_cols;
  size_t total_rows;
  size_t *col_widths;
} PrintDetails;

#define FLAG_HELP (1u << 0)
#define FLAG_VER (1u << 1)
#define FLAG_ALL (1u << 2)
// functions

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
          a structure that contains an array of struct dirent and other attributes related
          to the directory

      path_names: PathNames *
          a pointer to a structure that contains an array of char * to be freed

  Returns:
    none

  Postcondition:
      Will free the file_list->files array, the FileList *, and the path_names.pathnames array
  NULL
*/
void cleanup(FileList *file_list, PathNames *path_names);

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
  Prints the list of file names in a single column

  Parameters:
      files: struct dirent
        an array of struct of the files in the directory to be printed

      file_count: size_t
        the number of files to be printed

      fp: FILE *
        where the files will be printed.

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
        Filenames of all the files in the given directory will
        be printed to the FILE * fp in a single column
*/
int print_single_column_basic(struct dirent *files, size_t file_count, FILE *fp);

/*
  Reads a diretory pathname and creates a structure FileList pointer that
  contains details of those that directory, including file details and number of
  files

  Parameters:
    None


  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent
        file_capacity: size_t of the amount of struct dirent files has memory
  allocate for file_count: size_t of the number of struct dirent in files
*/
FileList *create_file_list(char *pathname);

/*
  Prints the list of files in the file_list in column-major order

  Parameters:
    file_list: FileList *
      a FileList structure that contains struct dirent of files to be printed, and the number
      of files in the array

  Returns:
    int:
      0 for success, 1 for error
  Postcondition:
      Will print all files to the screen in column-major order depending on terminal width
*/
int basic_print(FileList *file_list);

/*
  Prints the list of files in file_list in column major order

  Parameters:
    file_names: char**
      an array of char* that are the file names to be printed

    file_count: size_t
      the number of elements in the file_names array

    print_details: PrintDetails *
      a pointer PrintDetails structer that contains details of number of columns and number of rows
      and the width of each column

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
    prints the strings of file names from the file_list in column major order
*/
int print_column_layout(char **file_names, size_t file_count, PrintDetails *print_details);

/*
  Prints the list of files in the file_list to the output, depending on what option are set

  Parameters:
    file_list: FileList *
      a FileList structure that contains struct dirent of files to be printed, and the number of
  files in the array

  options: int *
    pointer to a bitmask that contains the options for formatted output

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
    prints the file strings in file_list to the output depending on what options are set
*/
int print_path(FileList *file_list, unsigned int *options);

/*
  Calculates the number of columns and rows that can be printed to the screen depending of the
  length of the file names and the terminal width

  Parameters:
    files: struct dirent
      pointer to base of an array of struct dirent of files to be printed

    file_count: size_t
      the number of elements in files

  Returns:
    PrintDetails *:
      contains the total number of colums and rows to be printed
*/
PrintDetails *calc_rows_cols(struct dirent *files, size_t file_count);

/*
  Parses a string for command line arguments and sets the appropriate flags that are preceded by a
  hyphen.

  Paramters:
    argv: char **
      an array of char * that represent command line arguments to parse

    size: int
      number of command line arguments in the argv array

    path_names: PathNames *
      a pointer to a struct that contains the path_names that will be outputted by the program

    options: unsigned int *
      a pointer to an unsigned bit that is a bitmask to store boolean flags that affect program
      output

  Returns:
    0 if success, otherwise returns non-zero representing an error
*/
int parse_arguments(int size, char **argv, PathNames *path_names, unsigned int *options);

/*
  Set the bitmask options to match the char *flag

  Parameters:
    flag: char *
      an option that begins with a hyphen and affects formatted output of the program

    options: unsigned int *
      a pointer to an unsigned bit that is a bitmask to store boolean flags that affect program
      output

  Postcondition:
    The appropriate bit that matches the option flag will be set to a 1 bit.
*/
int parse_option(char *flag, unsigned int *options);

/*
   Determines if a path is a legitimate directory and adds it to the path_names struct.

   Parameters:
      path: char *
        a string that represents a directory or filename

    path_names: PathNames *
      a pointer to a struct that contains the path_names that will be outputted by the program

  Returns:
    0 if success, otherwise returns non-zero error code

  Postcondition:
    if path is a legimate directory, adds it to the path_names.pathnames array and increases
    path_names.count
*/
int parse_paths(char *path, PathNames *path_names);
