#ifndef MY_CAT_HELPER_H
#define MY_CAT_HELPER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct that contains all the possible flags and members for arg parsing
typedef struct
{
  bool number_lines;          // flag -n
  bool number_nonblank_lines; // flag -b or --number-nonblank
  bool show_ends; // flag -E, also included with -A or --show-all or --show-ends
  bool squeeze_blank;     // -s or --squeeze-blank
  bool show_tabs;         // flag -T, -t, or -A or --show-all or --show-tabs
  bool show_nonprinting;  // flag -t, -A, -v, or show-all or show-nonprinting
  bool show_help;         // flag --help
  bool show_version;      // flag --version
  bool active_flags;      // used to do fast print
  bool option_terminator; // used to stop processing flags when -- given
  bool invalid_flag;      // flag that isn't one of above
  bool exit_early; // set when either show_help, show_version, invalid_set
} Options;

// struct that contains an array of files to print, and holds number in array
typedef struct
{
  char **files;
  size_t capacity;
  size_t file_count;
} File_List;

/*
   Prints out the file name fp to stdout. Takes and prints input char by char

   Parameters:
    fp: FILE *
      name of the file or stdin to print
    flag: Options *
      struct that contains the options to modify the print to stdout

    Postcondition:
      displays the char from fp to stdout
*/
void print_file(FILE *fp, Options *flag);

/*
   Prints out the file name fp to stdout. Takes and prints input char by char,
   Has no control flow for checking flags, called when no flags set

   Parameters:
    fp: FILE *
      name of the file or stdin to print

    Postcondition:
      displays the char from fp to stdout
*/
void print_file_basic(FILE *fp);

/*
   frees up allocated memory for file

   Parameters:
    files: char ***
      pointer to an array of char * pointers

    Postcondition:
      frees up memory used by files unless, files is NULL.
*/
void cleanup(char ***files);

/*
  Opens the file with the name filepath

  Parameters:
    filepath: char *
      pointer to a null terminated char string that is the name of a file

  `Returns:
    FILE pointer to the file, if can't be opened returns NULL pointer`
*/
FILE *open_file(char *filepath);

/*
  Identifies what options should be set dependent on flag

  Parameters:
    flag: char *
      pointer to null terminated string that begins with -
      char in string are used to set Option flags

    flags: Options *
      a struct wth booleans that represent options for printing

    Postcondtion:
      a number of booleans in the flags struct may be set depending on flag
*/
void parse_flags(char *flag, Options *flags);

/*
  Allocates memory in the struct file_list, files for a char pointer,
  then appends the pointer.

  Parameters:
    file_name: char *
      pointer to a null-terminated string that is the name of a file

    file_list:  File_List *
      pointer to a struct that contains a char ** and size_t that tracks
      the pointers to names of files, and number of files appendded

  Postcondition:
    memory will be allocated for one char * pointer, then flag will be appended
    to flags->files.
*/
void parse_files(char *file_name, File_List *file_list);

/*
   Iterates through argv and calls helper functions to parse either file names
   or flags

   Parameters:
    size: int
      the number of objects in the char ** argv

    argv: char **
      pointer to char *, an array of pointers to null-terminated strings that
      are file names or program flags

    file_list:  File_List *
      pointer to a struct that contains a char ** and size_t that tracks
      the pointers to names of files, and number of files appendded

    flags: Options *
      a struct wth booleans that represent options for printing
*/
void parse_arguments(int size, char **argv, File_List *files, Options *flags);

/*
  Prints to stdout the usage of the program and the various flags that can be
  given when the program is called

  Parameters:
    program_name: char *
      null-terminated string that is the name of the program called in the
      command line. Used in the examples printed out
*/
void print_help(char *program_name);

/*
   Prints to stdout version information and some prgoram details
    void print_version(char *program_name);

  Parameters:
    program_name: char *
      null-terminated string that is the name of the program called in the
      command line. Used in the examples printed out
*/
void print_version(char *program_name);

#endif
