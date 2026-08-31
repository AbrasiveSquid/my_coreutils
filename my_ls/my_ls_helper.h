/* contains the function prototypes that support my_ls */
#ifndef MY_LS_HELPER_H
#define MY_LS_HELPER_H

#include <ctype.h>
#include <dirent.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
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

// Details about a single file
typedef struct
{
  char *filename;
  struct stat *file_stats; // stat details
} FileDetails;

// contains details of files to be printed
typedef struct
{
  FileDetails **files;  // array of FileDetails
  size_t file_count;    // number of struct dirent  in files
  size_t file_capacity; // amount of struct dirent  allocated
} FileList;

#define FLAG_HELP (1u << 0)
#define FLAG_VER (1u << 1)
#define FLAG_ALL (1u << 2)
#define FLAG_LIST (1u << 3)
// functions

/*
  Compares the filenames of files of a and b use locale aware sorting
   Returns positive int if a > b, negative value if a < b, otherwise 0.
*/
int compare_filenames(const void *a, const void *b);

/*
  Compares the pathnames of a and b use locale aware sorting
  Returns positive int if a > b, negative value if a < b, otherwise 0.
*/
int compare_paths(const void *a, const void *b);

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
  Free memory allocated in a file_list

  Parameters:
    file_list: FileList *
      pointer to a FileList structure

  Returns:
    None

  Postcondition:
    if memory was allocated for a FileList, will free up each .files FileDetails as well
*/
void free_file_list(FileList *file_list);

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
  Reads a diretory pathname and creates a structure FileList pointer that
  contains details of those that directory, including file details and number of
  files

  Parameters:
    pathname: char *
      string that represents a pathname to a file or directory

    hidden_files: bool
      if true, includes . files, otherwise excludes them

  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent
        file_capacity: size_t of the amount of struct dirent files has memory
          allocate for file_count: size_t of the number of struct dirent in files
*/
FileList *create_file_list(char *pathname, bool hidden_files);

/*
  Creates a FileList structure for a directory that contains details of that directory

  Parameters:
    pathname: char *
      string that represents a pathname to a directory

    hidden_files: bool
      if true, includes . files, otherwise excludes them

  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent
        file_capacity: size_t of the amount of struct dirent files has memory
        allocate for file_count: size_t of the number of struct dirent in files
*/
FileList *create_file_list_dir(char *pathname, bool hidden_files);

/*
  Creates a FileList structure for a regular file

  Parameters:
    pathname: char *
      string that represents a pathname to a file

  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent
        file_capacity: size_t of the amount of struct dirent files has memory
        allocate for file_count: size_t of the number of struct dirent in files
*/
FileList *create_file_list_file(char *pathname);

/*
  Creates a structure of FileDetails and allocates memory, returns a pointer. Contains the filename
  and stat strucutre of information

  Parameters:
    pathname: char *
      string that represents a pathname to a file

    filename: char *
      string that represents a filename

  Returns:
    FileDetails *:
      a pointer to a FileDetails structure
*/
FileDetails *create_file_details(char *pathname, char *filename);

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
int parse_options(char *flag, unsigned int *options);

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

#endif
