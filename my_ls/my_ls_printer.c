#define _POSIX_C_SOURCE 200809L
#include "my_ls_printer.h"

int print_items(char **arr, size_t size, unsigned int options, bool dir_file)
{
  FileList *file_list = NULL;
  int return_code = 0;
  int file_count = size;

  for (size_t i = 0; i < size; i++)
  {
    // if multiple directories, prints directory above filelist
    if (dir_file && size > 1)
    {
      if (i > 0)
      {
        printf("\n");
      }
      printf("%s:\n", arr[i]);
    }

    // frees file_list (if not NULL) and sets pointer to NULL to prevent dangling ptr
    free_file_list(file_list);
    file_list = NULL;

    if (dir_file)
    {
      file_list = create_file_list_dir(arr[i], options & FLAG_ALL);
    }
    else
    {
      file_list = create_file_list_files(arr, file_count, options & FLAG_ALL);
      // reset count to 1 as files are just printed one time
      size = 1;
    }
    if (!(file_list))
    {
      return_code = 1;
      return return_code;
    }

    // sort file_list by LOCALE
    qsort(file_list->files, file_list->file_count, sizeof(*(file_list->files)), compare_filenames);

    // check if a no flag set
    if (!(options & ~FLAG_ALL)) // FLAG_ALL already handled during path creation
    {
      if (basic_print(file_list))
      {
        return_code = 1;
      }
    }
    else if (options & FLAG_LIST)
    {
      if (print_long_listing(file_list, options))
      {
        return_code = 1;
      }
    }
  }

  free_file_list(file_list);
  return return_code;
}

int basic_print(FileList *file_list)
{
  int return_code = 0;

  if (isatty(STDOUT_FILENO) != 1)
  {
    // printing to a file, print single column and exit program
    // use offset when calculating file count and base address
    return print_single_column_basic(file_list->files, file_list->file_count, stdout);
  }
  PrintDetails *print_details = calc_rows_cols(file_list->files, file_list->file_count);
  if (!(print_details))
  {
    fprintf(stderr, "Error calcualting number of rows and columns, exiting\n");
    return 1;
  }

  // means only row for single col, or some error calcuating, print 1 col
  if (print_details->total_cols < 2)
  {
    return_code = print_single_column_basic(file_list->files, file_list->file_count, stdout);
    free(print_details->col_widths);
    free(print_details);
    return return_code;
  }

  // start at index 2  then sub 2 from count because 0, and 1 are '.' and ".."
  char *file_names[file_list->file_count];
  for (size_t i = 0; i < file_list->file_count; i++)
  {
    file_names[i] = file_list->files[i]->filename;
  }
  return_code = print_column_layout(file_names, file_list->file_count, print_details);

  free(print_details->col_widths);
  free(print_details);
  return return_code;
}

int print_column_layout(char **file_names, size_t file_count, PrintDetails *print_details)
{
  size_t index;
  size_t row = 0, col = 0;
  size_t i = 0;
  /* print out files in column-major order IF it is too many files to fit in
  one line of terminal window */

  char *curr_file;
  while (i < file_count)
  {
    index = row + (col * print_details->total_rows); // get index for file
    if (index < file_count)
    {
      curr_file = file_names[index];
      printf("%-*s", (int)print_details->col_widths[col], curr_file);
      i++;
    }
    col++;
    if (col > print_details->total_cols - 1) // reset to go to next row
    {
      if (i < file_count) // to avoid double newline after last item
      {
        printf("\n");
      }
      row++;
      col = 0;
    }
  }
  printf("\n");
  return 0;
}

PrintDetails *calc_rows_cols(FileDetails **files, size_t file_count)
{

  size_t term_width = get_window_columns(STDOUT_FILENO);

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
        file_name_len = strlen(files[index]->filename);
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
  } while (width >= term_width);

  // set fields of print_details and return
  print_details->total_cols = total_col;
  print_details->total_rows = num_rows;
  return print_details;
}

int print_single_column_basic(FileDetails **files, size_t file_count, FILE *fp)
{
  if (!(fp))
  {
    fprintf(stderr, "FILE * does not exist, cannot print files, exiting...\n");
    return 1;
  }
  for (size_t i = 0; i < file_count; i++)
  {
    fprintf(fp, "%s\n", files[i]->filename);
  }

  return 0;
}

int print_path(FileList *file_list, unsigned int options)
{
  int single_column_mask = 0; // if any of these flags set, always single column layout
  int return_code = 0;
  PrintDetails *print_details = NULL;

  if (!(single_column_mask)) // output can be multi-column
  {
    if (options & FLAG_ALL)
    {
      print_details = calc_rows_cols(file_list->files, file_list->file_count);
    }

    else
    {
      print_details = calc_rows_cols(file_list->files, file_list->file_count);
    }
    if (!(print_details))
    {
      fprintf(stderr, "Error calculating number of rows and columns, exiting\n");
      return 1;
    }
  }

  if (options == FLAG_ALL)
  {
    // -a only flag set, same as basic print with '.' and ".." included
    char *file_names[file_list->file_count];
    for (size_t i = 0; file_list->file_count; i++)
    {
      file_names[i] = file_list->files[i]->filename;
    }
    return_code = print_column_layout(file_names, file_list->file_count, print_details);
  }

  else
  {

    for (size_t i = 0; i < file_list->file_count; i++)
    {
    }
  }

  if (print_details)
  {
    if (print_details->col_widths)
    {
      free(print_details->col_widths);
    }
    free(print_details);
  }
  return return_code;
}

int print_long_listing(FileList *file_list, unsigned int options)
{
  // print to long_listing
  int basic_long_list_flag =
      FLAG_ALL | FLAG_LIST;        // this flag will test bits that affect long listing print only
  int blocksize = get_blocksize(); // gets blocksize depending on env var
  int file_size_digits = largest_num_digits_filesize(file_list);
  int link_num_digits = largest_num_digits_links(file_list);
  FileDetails *curr_file = NULL;
  // print sum of blocksize
  if (file_list->direcory_listing)
  {
    printf("total %zu\n", file_list->blocksize_sum * 512 / blocksize);
  }

  char *perm_str = malloc(sizeof(char) * 11); // allocate 11 bytes for permission string
  if (!(perm_str))
  {
    fprintf(stderr, "Error allocating memory in print_long_listing, exiting\n");
    return 1;
  }
  char *time_str = malloc(sizeof(char) * 13);
  if (!(time_str))
  {
    fprintf(stderr, "Error allocating memory in print_long_listing, exiting\n");
    return 1;
  }

  if (!(options & basic_long_list_flag))
  {
    // NEED TO DO deal with options
  }
  for (size_t i = 0; i < file_list->file_count; i++)
  {
    curr_file = file_list->files[i];
    printf("%s", build_file_perm_string(perm_str, 11, curr_file->file_stats));
    printf(" %*zu", link_num_digits, curr_file->file_stats->st_nlink);
    printf(" %s %s", getpwuid(curr_file->file_stats->st_uid)->pw_name,
           getgrgid(curr_file->file_stats->st_gid)->gr_name);

    printf(" %*zu", file_size_digits, curr_file->file_stats->st_size);

    printf(" %s", epoch_to_human_readable_localtime(curr_file->file_stats->st_mtime, time_str, 13));

    if (S_ISLNK(curr_file->file_stats->st_mode))
    {
      int buf_size = curr_file->file_stats->st_size + 1; // size of bytes for symlink name
      char buf[buf_size];
      ssize_t link_code;
      if (!file_list->dirpath) // empty string, means no path to append
      {
        link_code = readlink(curr_file->filename, buf, buf_size);
      }
      else
      {
        int str_len = strlen(file_list->dirpath) + strlen(curr_file->filename);
        char fullpath[str_len + 1];
        strncpy(fullpath, file_list->dirpath, strlen(file_list->dirpath) + 1);
        strncat(fullpath, curr_file->filename, strlen(curr_file->filename));
        link_code = readlink(fullpath, buf, buf_size);
      }

      if (link_code < 0)
      {
        perror(curr_file->filename);
        free(perm_str);
        free(time_str);
        return 1;
      }

      printf(" %s -> %s\n", curr_file->filename, buf);
    }
    else
    {
      printf(" %s\n", curr_file->filename);
    }
  }

  free(perm_str);
  free(time_str);
  return 0;
}
