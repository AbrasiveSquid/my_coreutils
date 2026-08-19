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
  closedir(dp);

  qsort(file_list.files, file_list.file_count, sizeof(file_list.files[0]),
        compare_filenames);

  /*
    calculate max number of columns, and number of rows based on terminal
    window. Then print out files in column major order, if printing out all
    files in row-major on one line is too long.
  */
  size_t term_width = get_window_columns(STDOUT_FILENO);
  // printf("columns: %d\nfield_width: %zu\nwind_width: %d\n",
  // max_num_columns, file_list.field_width + 2,
  // get_window_columns(STDOUT_FILENO));

  printf("term_width: %zu\n", term_width);
  // determine number of columns and rows dependent on term width and length
  // of filenames in dir
  size_t printable_files =
      file_list.file_count - 2; // "." and ".." aren't printed
  size_t width;
  size_t total_col;
  size_t num_rows = 0;
  size_t max_col_width;
  size_t row;
  size_t col;
  size_t file_name_len;
  size_t *col_widths = NULL; // save width of each column for printing

  do
  {
    num_rows++; // increment row count each loop
    // calculate total col dependent on num of rows and files, if not even adds
    // 1 col
    total_col =
        (printable_files / num_rows) + (printable_files % num_rows != 0);
    col_widths = alloc_sizet_array(col_widths, total_col);
    if (!(col_widths))
    {
      cleanup(file_list.files, col_widths);
      return 1;
    }
    // reset width variable
    width = 0;
    for (col = 0; col < total_col; col++)
    {
      max_col_width = 0;
      row = 0;
      while (row < num_rows)
      {
        file_name_len =
            strlen(file_list.files[row + (col * num_rows) + 2].d_name);
        printf("length of %s: %zu\n",
               file_list.files[row + (col * num_rows) + 2].d_name,
               file_name_len);
        if (file_name_len > max_col_width)
        {
          max_col_width = file_name_len;
        }
        printf("max_col_width: %zu\n\n", max_col_width);
        row++;
      }
      width += max_col_width + 2;
      col_widths[col] = max_col_width + 2;
    }
    // printf("term width: %zu\nwidth: %zu\nnum_rows: %zu\ntotal_col: %zu\n",
    //        term_width, width, num_rows, total_col);
  } while (width > term_width);

  for (size_t i = 0; i < total_col; i++)
  {
    printf("col %zu: %zu\n", i, col_widths[i]);
  }
  // determine number of columns to use based on num_rows, if not even add extra
  // col
  printf(
      "\nnumber of rows: %zu\ntotal number of cols: %zu\nnum printable_files: "
      "%zu\n",
      num_rows, total_col, printable_files);
  // size_t row = 0, col = 0, i = 0;
  // // print out files in column-major order IF it is too many files to fit in
  // one
  // // line of terminal window
  // struct dirent curr_file; // use to have make function more readable
  // while (i < printable_files)
  // {
  //   // printf("i=%zu row=%zu col=%zu index=%zu\n", i, row, col,
  //   //        row + (num_rows * col) + 2);
  //   curr_file = file_list.files[row + (col * num_rows) + 2];
  //   printf("%-*s", (int)file_list.field_width + 2, curr_file.d_name);
  //   col++;
  //   if (col > total_col) // reset to go to next row
  //   {
  //     if (i < printable_files - 1) // to avoid double newline after last item
  //     {
  //       printf("\n");
  //     }
  //     row++;
  //     col = 0;
  //   }
  //   i++;
  // }
  // printf("\n");

  cleanup(file_list.files, col_widths);
  return 0;
}
