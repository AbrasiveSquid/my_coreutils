/* Contains the funciton definitions to support my_ls */

#include "my_ls_helper.h"
#include "debug.h"

int compare_filenames(const void *a, const void *b)
{
  const struct dirent *a_struct = (const struct dirent *)a;
  const char *a_str = a_struct->d_name;
  const struct dirent *b_struct = (const struct dirent *)b;
  const char *b_str = b_struct->d_name;

  return strcoll(a_str, b_str);
}

int get_window_columns(int fd)
{
  struct winsize size;
  if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
  {
    fprintf(stderr, "TIOCGWINSZ error");
  }
  return size.ws_col;
}

void cleanup(FileList *file_list)
{
  // only free file_list if allocated
  if (file_list)
  {
    // only free the struct dirent *, if mem was allocated
    if (file_list->files)
    {
      free(file_list->files);
    }
    free(file_list);
  }
  // // only free col_widths if mem was allocated
  // if (col_widths)
  // {
  //   free(col_widths);
  // }
}

void *alloc_array(void *arr, size_t size, size_t elem_size)
{
  if (!(arr))
  {
    arr = malloc(elem_size * size);
    if (!(arr))
    {
      fprintf(stderr, "error allocating memory, returning NULL\n");
      return NULL;
    }
    return arr;
  }

  size_t *temp_arr = realloc(arr, elem_size * size);
  if (!(temp_arr))
  {
    fprintf(stderr, "error allocating memory, returning NULL\n");
    return NULL;
  }
  return temp_arr;
}

void print_single_column(struct dirent *files, size_t file_count, FILE *fp)
{
  if (!(fp))
  {
    fprintf(stderr, "FILE * does not exist, cannot print files, exiting...\n");
    return;
  }
  // start at 2, because ".", and ".." are not printed
  for (size_t i = 2; i < file_count; i++)
  {
    fprintf(fp, "%s\n", (files + i)->d_name);
  }
}

FileList *create_file_list(char *pathname)
{
  DIR *dp;
  struct dirent *dirp;

  dp = opendir(pathname);
  if (!(dp))
  {
    perror(pathname);
    return NULL;
  }

  // initialize FileList struct that holds details of directories files
  FileList *file_list = malloc(sizeof(*file_list));
  if (!(file_list))
  {
    fprintf(stderr, "Error allocating memory in create_file_list, exiting.\n");
    return NULL;
  }

  // initialize file_list then allocate memory for files
  file_list->files = NULL;
  file_list->file_capacity = 4; // set inital capacity for 5
  file_list->file_count = 0;
  file_list->files =
      alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));

  // read all files in pathname and all to files array
  while ((dirp = readdir(dp)) != NULL)
  {
    if (file_list->file_capacity <= file_list->file_count)
    {
      file_list->file_capacity *= 2;
      file_list->files =
          alloc_array(file_list->files, file_list->file_capacity, sizeof(*(file_list->files)));
    }
    // add file to file_list
    file_list->files[file_list->file_count++] = *dirp;
  }
  closedir(dp);

  return file_list;
}

int basic_print(FileList *file_list)
{

  PrintDetails *print_details = calc_rows_cols(file_list->files + 2, file_list->file_count - 2);
  if (!(print_details))
  {
    fprintf(stderr, "Error calcualting number of rows and columns, exiting\n");
    return 1;
  }

  // means only row for single col, or some error calcuating, print 1 col
  if (print_details->total_cols < 2)
  {
    print_single_column(file_list->files, file_list->file_count, stdout);
    free(print_details->col_widths);
    free(print_details);
    return 0;
  }

  size_t index;
  size_t row = 0, col = 0;
  size_t i = 0;
  size_t printable_file_count = file_list->file_count - 2;
  /* print out files in column-major order IF it is too many files to fit in
  one line of terminal window */
  struct dirent curr_file; // use to have make function more readable
  while (i < printable_file_count)
  {
    DEBUG_PRINT("i=%zu row=%zu col=%zu index=%zu\n", i, row, col, row + (num_rows * col) + 2);
    DEBUG_PRINT("\nrow: %zu\ncol: %zu\nnum_rows: %zu\ni: %zu\nindex: %zu\n "
                "col_widths: %zu\n",
                row, col, print_details->total_rows, i, row + (col * print_details->total_rows) + 2,
                print_details->col_widths[col]);
    index = row + (col * print_details->total_rows) + 2; // get index for file
    if (index < file_list->file_count)
    {
      curr_file = file_list->files[index];
      printf("%-*s", (int)print_details->col_widths[col], curr_file.d_name);
      i++;
    }
    col++;
    if (col > print_details->total_cols - 1) // reset to go to next row
    {
      if (i < printable_file_count) // to avoid double newline after last item
      {
        printf("\n");
      }
      row++;
      col = 0;
    }
  }
  printf("\n");

  free(print_details->col_widths);
  free(print_details);
  return 0;
}

PrintDetails *calc_rows_cols(struct dirent *files, size_t file_count)
{

  size_t term_width = get_window_columns(STDOUT_FILENO);

  DEBUG_PRINT("term_width: %zu\n", term_width);
  // determine number of columns and rows dependent on term width and length
  // of filenames in dir
  size_t width;
  size_t total_col;
  size_t prev_total_col = 0;
  size_t num_rows = 0;
  size_t max_col_width;
  size_t row;
  size_t col;
  size_t file_name_len;
  size_t index;

  // init print_details and allocate memory
  PrintDetails *print_details = malloc(sizeof(*print_details));
  if (!(print_details))
  {
    fprintf(stderr, "Error allocating memory in calc_row_cols, exiting\n");
    return NULL;
  }
  print_details->col_widths = NULL;

  do
  {
    num_rows++; // increment row count each loop
    // calculate total col dependent on num of rows and files, if not even adds
    // 1 col
    total_col = (file_count / num_rows) + (file_count % num_rows != 0);
    if (total_col == prev_total_col)
    {
      continue; // prevent calcuating same total_col twice and instead inc
                // num_rows
    }

    else
    {
      prev_total_col = total_col;
    }

    print_details->col_widths =
        alloc_array(print_details->col_widths, total_col, sizeof(*(print_details->col_widths)));
    if (!(print_details->col_widths))
    {
      free(print_details);
      return NULL;
    }
    // reset width variable
    width = 0;
    for (col = 0; col < total_col; col++)
    {
      max_col_width = 0;
      row = 0;
      while (row < num_rows)
      {
        index = row + (col * num_rows);
        if (index > file_count - 1)
        {
          // index is outside bounds of files array
          break;
        }
        file_name_len = strlen(files[index].d_name);
        // DEBUG_PRINT("length of %s: %zu\n", file_list->files[index].d_name,
        //             file_name_len);
        if (file_name_len > max_col_width)
        {
          max_col_width = file_name_len;
          if (max_col_width + 2 > term_width)
          {
            // too wide for more than 1 col, print single column
            print_details->total_cols = 1;
            print_details->total_rows = file_count;
            return print_details;
          }
        }
        // DEBUG_PRINT("max_col_width: %zu\n\n", max_col_width);
        row++;
      }
      // add 2 to each one for minumum spacing between columns of 2
      if (col == total_col - 1)
      {
        width += max_col_width;
        print_details->col_widths[col] = max_col_width;
      }
      else
      {
        width += max_col_width + 2;
        print_details->col_widths[col] = max_col_width + 2;
      }
    }
    // DEBUG_PRINT("term width: %zu\nwidth: %zu\nnum_rows: %zu\ntotal_col:
    // %zu\n",
    //             term_width, width, num_rows, total_col);
    DEBUG_PRINT("rows=%zu cols=%zu width=%zu\n", num_rows, total_col, width);
  } while (width >= term_width);

  // set fields of print_details and return
  print_details->total_cols = total_col;
  print_details->total_rows = num_rows;
  return print_details;
}
