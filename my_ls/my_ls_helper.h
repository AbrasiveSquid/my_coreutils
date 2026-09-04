/* contains the function prototypes that support my_ls */
#ifndef MY_LS_HELPER_H
#define MY_LS_HELPER_H

#include <ctype.h>
#include <dirent.h>
#include <grp.h>
#include <locale.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// structs

// pathname - contains details of the pathnames to directories whose contents will be listed
typedef struct
{
  char **dir_names;
  size_t dir_count;
  size_t dir_capacity;

  char **file_names;
  size_t file_count;
  size_t file_capacity;
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
  char *dirpath;
  FileDetails **files;  // array of FileDetails
  size_t file_count;    // number of struct dirent  in files
  size_t file_capacity; // amount of struct dirent  allocated
  size_t blocksize_sum;
  bool direcory_listing; // is the FileList from a directory(1) or single files(0)
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

/*
  Examines if PathNames ptr contains elements in either the dir_names or file_names array, if so
  sorts them

  Parameters:
    pathnames: PathNames *
      pointer to a structure that contains arrays of dir_names and file_names to be sorted

  Returns:
    None

  Postcondition:
    if pathnames.dir_names and/or path_names.file_names contains any elements, those elements will
    be sorted according to LOCALE.
*/
void sort_pathnames(PathNames *pathnames);

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
void cleanup(PathNames *path_names);

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
    filenames: char **
      array of char * that represent filenames

    size: size_t
      number of elements in filenames

    hidden_files: bool
      flag if dotfiles should be included in the FileList

  Returns:
    FileList *: a pointer to a FileList structure that contains the attributes
        files *: a pointer to an array of struct dirent
        file_capacity: size_t of the amount of struct dirent files has memory
        allocate for file_count: size_t of the number of struct dirent in files
*/
FileList *create_file_list_files(char **filenames, size_t size, bool hidden_files);

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

  Returns:
    int:
      0 for success, 1 for failed option flags (program should terminate). 2 for invalid path name,
      program may not terminate
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

/*
  Returns a blocksize as an int depending on what environment variables are set, if none are set
  defaults to 1024

  Parameters:
    None

  Returns:
    int as a blocksize
*/
int get_blocksize(void);

/*
  Builds a string reprsenting the permissions a file has on unix

  Parameters:
    c_ptr: char *
      pointer to an array of characters

    size: int
      amount of bytes allocated for c_ptr

    file_stats: struct stat *
      a pointer that contains file details to extract permissions from

  Returns:
    a string of 10 characters plus the null char that represents permissions
    d for directory, r for read, w for write, x for execute
    - in first position for regular file
    examples:
      ----------
      -rwxr-xr-x
      drw-------

*/
char *build_file_perm_string(char *c_ptr, int size, struct stat *file_stats);

/*
  Finds the most number of digits used in file size and returns that as an int. Used for printf
  specifier formatting

  Parameters:
    file_list: FileList *
      pointer to a list of files that each have a file_stats->st_size field

  Returns:
    positive int
*/
int largest_num_digits_filesize(const FileList *file_list);

/*
  Finds the most number of digits used in number of links and returns that as an int. Used for
  printf specifier formatting

  Parameters:
    file_list: FileList *
      pointer to a list of files that each have a file_stats->st_nlink field

  Returns:
    positive int
*/
int largest_num_digits_links(const FileList *file_list);

/*
  Finds the most number of digits used for major and minor devices and returns the total number of
  characters needed for printf formatting

  Parameters:
    file_list: FileList *
      pointer to a list of files that each have a file_stats->st_rdev field

  Returns:
    positive int
*/
int char_width_devices(const FileList *file_list);

/*
  Returns the number of digits in num

  Parameters:
    num: int

  Returns:
    int:
      equal to number of digits, example: 300 returns 3
*/
int num_digits(int num);

/*
  Builds a string representation of a human readable data in local time from epoch time

  Parameters:
    epoch_time: time_t
      time to convert to human readable data, example 'Aug 24 12:56'

    str: char *
      pointer to write the string into

    str_len: int
      number of bytes allocated in str to write to

  Returns:
    string of human readable time: 'Aug 24 12:56'
*/
char *epoch_to_human_readable_localtime(time_t epoch_time, char *str, int str_len);

/*
  Converts a number to a string, if number is negative, returns NULL

  Parameters:
    num: uintmax_t
      number to convert, must be non-negative integer

    str: char *
      string pointer to put in num

    size: int
      number of bytes allocated for str

  Returns:
    char *, that is the integer number converted to a string
*/
char *num_to_str(uintmax_t num, char *str, int size);

/*
  Builds a string of filesize or device depending on the file type. Character device returns device
  details, otherwise returns filesize

  Parameters:
    str: char *
      char array to hold the string

    n: int
      number of bytes allocated for str

    file_stats: const struct stat *
      pointer to the file stats for a file

    options: unsigned int
      bit mask that has boolean options that affect formatting
*/
char *get_size_or_dev_str(char *str, int n, const struct stat *file_stats, unsigned int options);

/*
  Determines the max number of characters for the owners in a file list for print formatting

  Parameters:
    file_list: const FileList *

  Returns:
    int, positive value of number of characters in a owner name, -1 for error
*/
int find_max_uname_len(const FileList *file_list);

/*
  Determines the max number of characters for the group owners in a file list for print formatting

  Parameters:
    file_list: const FileList *

  Returns:
    int, positive value of number of characters in a group name, -1 for error
*/
int find_max_gname_len(const FileList *file_list);

#endif
