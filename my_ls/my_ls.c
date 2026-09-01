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

  // first print all files, then print directories
  if (path_names.file_count > 0)
  {
    int print_code = print_items(path_names.file_names, path_names.file_count, options, false);
    if (!(return_code))
    {
      return_code = print_code;
    }
  }
  if (path_names.dir_count > 0)
  {
    int print_code = print_items(path_names.dir_names, path_names.dir_count, options, true);
    if (!(return_code))
    {
      return_code = print_code;
    }
  }

  // for (size_t i = 0; i < path_names.count; i++)
  // {
  //   if (path_names.count > 1)
  //   {
  //     if (i > 0)
  //     {
  //       printf("\n");
  //     }
  //     // if multiple paths, print pathname at top
  //     printf("%s:\n", path_names.pathnames[i]);
  //   }
  //
  //   free_file_list(file_list);
  //   file_list = NULL;
  //
  //   file_list = create_file_list(path_names.pathnames[i], options & FLAG_ALL);
  //   if (!(file_list))
  //   {
  //     cleanup(file_list, &path_names);
  //     return_code = 1;
  //     return return_code; // file_list could not be created so program exits
  //   }
  //   // sort files by LOCALE
  //   qsort(file_list->files, file_list->file_count, sizeof(file_list->files[0]),
  //   compare_filenames);
  //
  //   // check if any flag set
  //   if (!(options) || (options == FLAG_ALL))
  //   {
  //     // basic print is no option set or only all is set
  //     if (basic_print(file_list)) // non-zero return means error
  //     {
  //       return_code = 1;
  //     }
  //   }
  //   else
  //   {
  //     if (print_path(file_list, &options))
  //       return_code = 1;
  //   }
  // }
  cleanup(&path_names);
  return return_code;
}
