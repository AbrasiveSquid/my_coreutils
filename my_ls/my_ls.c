#include "my_ls_helper.h"
#include <locale.h>

int main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;
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

  dp = opendir(pathname);

  if (!(dp))
  {
    perror(pathname);
    return 1;
  }

  // initialize FileList struct that holds details of directories files
  FileList file_list;
  file_list.files = NULL;
  file_list.file_capacity = 0;
  file_list.file_count = 0;
  file_list.field_width = 0;
  file_list.files = alloc_mem_for_files(&file_list);
  size_t curr_len;
  if (!(file_list.files))
  {
    fprintf(stderr, "Error allocating memory for struct dirent, exiting.\n");
    return 1;
  }

  while ((dirp = readdir(dp)) != NULL)
  {
    if (file_list.file_capacity <= file_list.file_count)
    {
      file_list.files = alloc_mem_for_files(&file_list);
    }
    // add file to file_list
    file_list.files[file_list.file_count++] = *dirp;

    // check if current entry has longest pathname, and update if it does. Used
    // for formatting
    curr_len = strlen(dirp->d_name);
    if (curr_len > file_list.field_width)
    {
      file_list.field_width = curr_len;
    }
  }

  qsort(file_list.files, file_list.file_count, sizeof(file_list.files[0]),
        compare_filenames);

  /*
    calculate max number of columns, and number of rows based on terminal
    window. Then print out files in column major order, if printing out all
    files in row-major on one line is too long.
  */
  int max_num_columns =
      (get_window_columns(STDOUT_FILENO) / ((int)file_list.field_width + 2));
  // printf("columns: %d\nfield_width: %zu\nwind_width: %d\n", max_num_columns,
  //        file_list.field_width + 2, get_window_columns(STDOUT_FILENO));

  size_t num_rows = 1;
  while ((file_list.file_count - 2) / num_rows > (size_t)max_num_columns)
  {
    num_rows++;
  }
  size_t total_col = (file_list.file_count - 2) / num_rows;
  // printf("number of rows: %zu\ntotal number of cols: %zu\n", num_rows,
  //        total_col);
  struct dirent curr_file; // use to have make function more readable
  size_t row = 0, col = 0, i = 2;
  // print out files in column-major order IF it is too many files to fit in one
  // line of terminal window
  while (i < file_list.file_count)
  {
    curr_file = file_list.files[row + col + 2];
    printf("%-*s", (int)file_list.field_width + 2, curr_file.d_name);
    col += num_rows; // increment by num of rows because column-major order
    if (col - num_rows > total_col) // reset to go to next row
    {
      if (i <
          file_list.file_count - 1) // to avoid double newline after last item
      {
        printf("\n");
      }
      row++;
      col = 0;
    }
    i++;
  }
  printf("\n");

  return 0;
}
