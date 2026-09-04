#include "my_ls_helper.h"
#include "my_ls_printer.h"

int main(int argc, char *argv[])
{
  // initialize variables
  int return_code = 0;
  PathNames path_names; // init struct and then allocate memory for 1 pathname
  path_names.file_names = NULL;
  path_names.file_count = 0;
  path_names.file_capacity = 1; // set to 1 as tool most often has a single pathname
  path_names.dir_names = NULL;
  path_names.dir_count = 0;
  path_names.dir_capacity = 1; // set to 1 as tool most often has a single pathname
  if (!(path_names.file_names = alloc_array(path_names.file_names, path_names.file_capacity,
                                            sizeof(*(path_names.file_names)))))
  {
    // couldn't allocate memory for pathnames, so program exits
    fprintf(stderr, "Error allocating memory in main, exiting\n");
    return 1;
  }
  if (!(path_names.dir_names = alloc_array(path_names.dir_names, path_names.dir_capacity,
                                           sizeof(*(path_names.dir_names)))))
  {
    // couldn't allocate memory for pathnames, so program exits
    fprintf(stderr, "Error allocating memory in main, exiting\n");
    return 1;
  }
  unsigned int options = 0;  // initialize all options to false
  setlocale(LC_COLLATE, ""); // set locale to shell's env for sorting

  if (argc == 1)
  {
    path_names.dir_names[0] = ".";
    path_names.dir_count = 1;
  }
  else
  {
    int arg_code;
    if ((arg_code = parse_arguments(argc, argv, &path_names, &options)))
    {
      // code 0, success, proram ignores this conditional

      if (arg_code == 1)
      { // code 1, program with option flags, program will exit
        cleanup(&path_names);
        return 1;
      }
      else if (arg_code == 2)
      {
        // invalid path name, set the error return code, but program to continue
        return_code = 1;
      }
      else
      {
        fprintf(stderr, "Invalid return code, program will exit\n");
        cleanup(&path_names);
        return return_code;
      }
    }
  }

  // sorts all files and directories by LOCALE
  sort_pathnames(&path_names);

  // calculate width
  FileList *operand_width_source = NULL;
  if ((options & FLAG_LIST) && path_names.file_count > 0 && path_names.dir_count > 0)
  {
    size_t operand_count = path_names.file_count + path_names.dir_count;
    char *operands[operand_count];

    size_t index = 0;

    for (size_t i = 0; i < path_names.file_count; i++)
    {
      operands[index++] = path_names.file_names[i];
    }

    for (size_t i = 0; i < path_names.dir_count; i++)
    {
      operands[index++] = path_names.dir_names[i];
    }
    operand_width_source = create_file_list_files(operands, operand_count, true);
  }

  // first print all files, then print directories
  if (path_names.file_count > 0)
  {
    int print_code = print_items(path_names.file_names, path_names.file_count, options, false, 0,
                                 operand_width_source);
    if (!(return_code))
    {
      return_code = print_code;
    }
    if (path_names.dir_count)
    {
      printf("\n");
    }
  }
  if (path_names.dir_count > 0)
  {
    int print_code = print_items(path_names.dir_names, path_names.dir_count, options, true,
                                 path_names.file_count, NULL);
    if (!(return_code))
    {
      return_code = print_code;
    }
  }

  free_file_list(operand_width_source);
  cleanup(&path_names);
  return return_code;
}
