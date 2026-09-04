// contains functions related to output for my_ls program
#ifndef MY_LS_PRINTER_H
#define MY_LS_PRINTER_H

#include "my_ls_helper.h"
// structs
// attributes of number of columns and rows to be printed
typedef struct
{
  size_t total_cols;
  size_t total_rows;
  size_t *col_widths;
} PrintDetails;

/*
  Controller function that calls other functions to print files

  Parameters:
    arr: char **
      an array of char * that represent filenames or direcotires

    size: size_t
      the number of elements in arr to be printed

    options: unsigned int
      bitmask that represents options for formatted output

    dir_file: bool
      flag to determine if arr contains files or directories

    files_printed: bool
      flag to determine if other files were printed so directory path is included

    width_source: const FileList *
      struct of the column widths

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
    Calls other method that will print the items in the arr to the stdout
*/
int print_items(char **arr, size_t size, unsigned int options, bool dir_file, bool files_printed,
                const FileList *width_source);

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
int print_single_column_basic(FileDetails **files, size_t file_count, FILE *fp);

/*
  Prints the list of files in the file_list in column-major order

  Parameters:
    file_list: FileList *
      a FileList structure that contains FileDetails of files to be printed, and the number
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
      a FileList structure that contains FileDetails of files to be printed, and the number of
      files in the array

    options: int *
      pointer to a bitmask that contains the options for formatted output

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
    prints the file strings in file_list to the output depending on what options are set
*/
int print_path(FileList *file_list, unsigned int options);

/*
  Calculates the number of columns and rows that can be printed to the screen depending of the
  length of the file names and the terminal width

  Parameters:
    files: struct dirent
      pointer to base of an array of FileDetails of files to be printed

    file_count: size_t
      the number of elements in files

    options: unsigned int
      bitmask that represents options for formatted output


  Returns:
    PrintDetails *:
      contains the total number of colums and rows to be printed
*/
PrintDetails *calc_rows_cols(FileDetails **files, size_t file_count);

/*
  Handles the printing for long listing format (when the -l flag is set)

  Parameters:
    file_list: FileList *
      a FileList structure that contains FileDetails of files to be printed, and the number of
      files in the array

    options: int *
      pointer to a bitmask that contains the options for formatted output

    width_source: const FileList *
      struct of the column widths

  Returns:
    int: 0 for success, 1 for error

  Postcondition:
    prints the strings of filenames in file_list to stdout in the long listing format
*/
int print_long_listing(FileList *file_list, unsigned int options, const FileList *width_source);

/*
  Prints the pathname to the fp. Used when multiple paths are listed as arguments

  Parameters:
    path_name: char *

  Returns:
    int:
      0 for success, 1 for error

  Postcondition:
    prints path_name to the fp or stdout
*/
// int print_pathname(char *pathname); currently not using, but might refactor

#endif
