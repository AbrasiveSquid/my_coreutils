#include "my_ls_helper.h"

int main(int argc, char *argv[])
{
  char *pathname = NULL;
  Options *flags = {0};      // initialize all options to false
  setlocale(LC_COLLATE, ""); // set locale to shell's env for sorting

  if (argc == 1)
  {
    pathname = ".";
  }
  else
  {
    pathname = argv[1];
  }

  FileList *file_list = create_file_list(pathname);
  if (!(file_list))
  {
    cleanup(file_list);
    return 1; // file_list could not be created so program exits
  }

  qsort(file_list->files, file_list->file_count, sizeof(file_list->files[0]), compare_filenames);

  if (isatty(STDOUT_FILENO) != 1)
  {
    // printing to a file, print single column and exit program
    print_single_column(file_list->files, file_list->file_count, stdout);
    cleanup(file_list);
    return 0;
  }

  basic_print(file_list);

  cleanup(file_list);
  return 0;
}
