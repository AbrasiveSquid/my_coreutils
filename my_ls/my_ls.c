#include "my_ls_helper.h"
#include "my_ls_printer.h"

int main(int argc, char *argv[])
{
  // initialize variables
  int return_code = 0;
  PathNames path_names; // init struct and then allocate memory for 1 pathname
  path_names.pathnames = NULL;
  path_names.count = 0;
  path_names.capacity = 1; // set to 1 as tool most often has a single pathname
  FileList *file_list = NULL;
  if (!(path_names.pathnames = alloc_array(path_names.pathnames, path_names.capacity,
                                           sizeof(*(path_names.pathnames)))))
  {
    // couldn't allocate memory for pathnames, so program exits
    return_code = 2; // program will continue and try to process and valid paths
  }
  unsigned int options = 0;  // initialize all options to false
  setlocale(LC_COLLATE, ""); // set locale to shell's env for sorting

  if (argc == 1)
  {
    path_names.pathnames[0] = ".";
    path_names.count = 1;
  }
  else
  {
    if ((parse_arguments(argc, argv, &path_names, &options)))
    {
      // error parsing the arguments, exit with failure
      cleanup(file_list, &path_names);
      return 1;
    }
  }

  if (path_names.count > 1) // sort pathnames
  {
    qsort(path_names.pathnames, path_names.count, sizeof(*(path_names.pathnames)), compare_paths);
  }

  for (size_t i = 0; i < path_names.count; i++)
  {
    if (path_names.count > 1)
    {
      if (i > 0)
      {
        printf("\n");
      }
      // if multiple paths, print pathname at top
      printf("%s:\n", path_names.pathnames[i]);
    }

    free_file_list(file_list);
    file_list = NULL;

    file_list = create_file_list(path_names.pathnames[i], options & FLAG_ALL);
    if (!(file_list))
    {
      cleanup(file_list, &path_names);
      return_code = 1;
      return return_code; // file_list could not be created so program exits
    }
    // sort files by LOCALE
    qsort(file_list->files, file_list->file_count, sizeof(file_list->files[0]), compare_filenames);

    // check if any flag set
    if (!(options) || (options == FLAG_ALL))
    {
      // basic print is no option set or only all is set
      if (basic_print(file_list)) // non-zero return means error
      {
        return_code = 1;
      }
    }
    else
    {
      if (!(print_path(file_list, &options)))
        return_code = 1;
    }
  }
  cleanup(file_list, &path_names);
  return return_code;
}
