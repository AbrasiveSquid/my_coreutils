#include "debug.h"
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

  DEBUG_PRINT("term_width: %zu\n", term_width);
  // determine number of columns and rows dependent on term width and length
  // of filenames in dir
  size_t printable_files =
      file_list.file_count - 2; // "." and ".." aren't printed
  size_t width;
  size_t total_col;
  size_t prev_total_col = 0;
  size_t num_rows = 0;
  size_t max_col_width;
  size_t row;
  size_t col;
  size_t file_name_len;
  size_t *col_widths = NULL; // save width of each column for printing
  size_t index;

  DEBUG_PRINT("Num of printable_files: %zu\n", file_list.file_count - 2);
  do
  {
    num_rows++; // increment row count each loop
    // calculate total col dependent on num of rows and files, if not even adds
    // 1 col
    total_col =
        (printable_files / num_rows) + (printable_files % num_rows != 0);
    if (total_col == prev_total_col)
    {
      continue; // prevent calcuating same total_col twice and instead inc
                // num_rows
    }

    else
    {
      prev_total_col = total_col;
    }

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
        index = row + (col * num_rows) + 2;
        if (index > file_list.file_count - 1)
        {
          // index is outside bounds of files array
          break;
        }
        file_name_len = strlen(file_list.files[index].d_name);
        // DEBUG_PRINT("length of %s: %zu\n", file_list.files[index].d_name,
        //             file_name_len);
        if (file_name_len > max_col_width)
        {
          max_col_width = file_name_len;
          if (max_col_width + 2 > term_width)
          {
            // too wide for more than 1 col, print single column
            print_single_column(file_list.files, file_list.file_count, stdout);
            cleanup(file_list.files, col_widths);
            return 0;
          }
        }
        // DEBUG_PRINT("max_col_width: %zu\n\n", max_col_width);
        row++;
      }
      // add 2 to each one for minumum spacing between columns of 2
      if (col == total_col - 1)
      {
        width += max_col_width;
        col_widths[col] = max_col_width;
      }
      else
      {
        width += max_col_width + 2;
        col_widths[col] = max_col_width + 2;
      }
    }
    // DEBUG_PRINT("term width: %zu\nwidth: %zu\nnum_rows: %zu\ntotal_col:
    // %zu\n",
    //             term_width, width, num_rows, total_col);
    DEBUG_PRINT("rows=%zu cols=%zu width=%zu\n", num_rows, total_col, width);
  } while (width > term_width);

#ifdef DEBUG
  for (size_t i = 0; i < total_col; i++)
  {
    DEBUG_PRINT("col %zu: %zu\n", i, col_widths[i]);
  }
#endif
  // determine number of columns to use based on num_rows, if not even add extra
  // col
  DEBUG_PRINT(
      "\nnumber of rows: %zu\ntotal number of cols: %zu\nnum printable_files: "
      "%zu\n",
      num_rows, total_col, printable_files);

  row = 0, col = 0;
  size_t i = 0;
  /* print out files in column-major order IF it is too many files to fit in
  one line of terminal window */
  struct dirent curr_file; // use to have make function more readable
  while (i < printable_files)
  {
    DEBUG_PRINT("i=%zu row=%zu col=%zu index=%zu\n", i, row, col,
                row + (num_rows * col) + 2);
    DEBUG_PRINT("\nrow: %zu\ncol: %zu\nnum_rows: %zu\ni: %zu\nindex: %zu\n "
                "col_widths: %zu\n",
                row, col, num_rows, i, row + (col * num_rows) + 2,
                col_widths[col]);
    index = row + (col * num_rows) + 2; // get index for file
    if (index < file_list.file_count)
    {
      curr_file = file_list.files[index];
      printf("%-*s", (int)col_widths[col], curr_file.d_name);
      i++;
    }
    col++;
    if (col > total_col - 1) // reset to go to next row
    {
      if (i < printable_files) // to avoid double newline after last item
      {
        printf("\n");
      }
      row++;
      col = 0;
    }
  }
  printf("\n");

  cleanup(file_list.files, col_widths);
  return 0;
}
