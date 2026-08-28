/* Contains the funciton definitions to support my_ls */

#include "my_ls_helper.h"

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

void cleanup(FileList *file_list, size_t *col_widths)
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
  // only free col_widths if mem was allocated
  if (col_widths)
  {
    free(col_widths);
  }
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
  // if (!(file_list->files))
  // {
  //   fprintf(stderr, "Error allocating memory for struct dirent, exiting.\n");
  //   return NULL;
  // }

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
