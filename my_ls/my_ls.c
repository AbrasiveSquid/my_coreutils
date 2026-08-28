#include "debug.h"
#include "my_ls_helper.h"

int main(int argc, char *argv[])
{
  char *pathname = NULL;
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
  /*
    calculate max number of columns, and number of rows based on terminal
    window. Then print out files in column major order, if printing out all
    files in row-major on one line is too long.
  */
  basic_print(file_list);

  // #ifdef DEBUG
  //   for (size_t i = 0; i < total_col; i++)
  //   {
  //     DEBUG_PRINT("col %zu: %zu\n", i, col_widths[i]);
  //   }
  // #endif
  //   // determine number of columns to use based on num_rows, if not even add extra
  //   // col
  //   DEBUG_PRINT("\nnumber of rows: %zu\ntotal number of cols: %zu\nnum printable_files: "
  //               "%zu\n",
  //               num_rows, total_col, printable_files);
  //

  cleanup(file_list);
  return 0;
}
