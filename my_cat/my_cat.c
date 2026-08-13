#include "my_cat_helper.h"

/* Copying the features of cat cli program */

int main(int argc, char *argv[])
{
  int exit_status = 0;
  Options flags = {0}; // init all flags to false
  // init files array to NULL, capacity and file_count to 0
  File_List file_list = {NULL, 0, 0};

  // pass argv[1] because argv[0] is name of program, argc-1 because skip first
  // program
  parse_arguments(argc - 1, &argv[1], &file_list, &flags);
  if (flags.exit_early)
  {
    free(file_list.files);
    if (flags.show_help)
    {
      print_help(argv[0]);
      return exit_status;
    }
    else if (flags.show_version)
    {
      print_version(argv[0]);
      return exit_status;
    }
    else if (flags.invalid_flag)
    {
      exit_status = 1;
      return exit_status;
    }
  }

  // if no arguments or if only flags as arguments
  if (!(file_list.files))
  {
    print_file(stdin, &flags);
    cleanup(&(file_list.files));
    return exit_status;
  }

  // opens each file path and displays them one after another
  FILE *fp;
  for (size_t i = 0; i < file_list.file_count; i++)
  {
    // if argument is "-" takes input from stdin until EOF (ctrl+D) then
    // continues  to next argument
    if (strcmp("-", *(file_list.files + i)) == 0)
    {
      if (feof(stdin))
      {
        clearerr(stdin); // removes EOF on stdin if there was a prev stdin
      }
      print_file(stdin, &flags);
    }
    else
    {
      fp = open_file(*(file_list.files + i));
      if (fp == NULL)
      {
        exit_status = 1; // set to 1 if a file is missing
        continue;
      }
      print_file(fp, &flags);
      fclose(fp);
    }
  }

  cleanup(&(file_list.files));
  return exit_status;
}
