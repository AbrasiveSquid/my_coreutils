#include "my_cat_helper.h"
#include <string.h>

/* Copying the features of cat cli program */

int main(int argc, char *argv[])
{
  Options flags = {0}; // init all flags to false
  File_List file_list = {NULL,
                         0}; // init files array to NULL and file_count to 0

  // pass argv[1] because argv[0] is name of program, argc-1 because skip first
  // program
  parse_arguments(argc - 1, &argv[1], &file_list, &flags);
  if (flags.exit_early)
  {
    free(file_list.files);
    if (flags.show_help)
    {
      print_help(argv[0]);
      return 0;
    }
    else if (flags.show_version)
    {
      print_version(argv[0]);
      return 0;
    }
    else if (flags.invalid_flag)
    {
      return 1;
    }
  }

  // if no arguments or if only flags as arguments
  if (!(file_list.files))
  {
    print_file(stdin, &flags);
    cleanup(&(file_list.files));
    return 0;
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
        fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                *(file_list.files));
        continue;
      }
      print_file(fp, &flags);
      fclose(fp);
    }
  }

  cleanup(&(file_list.files));
  return 0;
}
