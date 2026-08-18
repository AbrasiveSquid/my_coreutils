/* Contains the funciton definitions to support my_ls */

#include "my_ls_helper.h"

int compare_filenames(const void *a, const void *b)
{
  const struct dirent *a_struct = (struct dirent *)a;
  const char *a_str = a_struct->d_name;
  const struct dirent *b_struct = (struct dirent *)b;
  const char *b_str = b_struct->d_name;

  int a_chr, b_chr;
  int i = 0;
  while (a_str[i] != '\0' && b_str[i] != '\0')
  {
    a_chr = tolower((unsigned char)a_str[i]);
    b_chr = tolower((unsigned char)b_str[i]);

    if (a_chr == b_chr)
    {
      i++;
      continue;
    }
    return a_chr - b_chr;
  }
  if (a_str[i] == '\0' && b_str[i] == '\0') // identical strings
  {
    return 0;
  }
  else if (a_str[i] == '\0')
  {
    return 1; // a_str should come before b_str
  }
  else
  {
    return -1; // b_str should come before a_str
  }
}

struct dirent *alloc_mem_for_files(FileList *file_list)
{
  if (!(file_list->files))
  {
    // allocate memory for 5 struct direct

    struct dirent *new_files = malloc(sizeof(*new_files) * 5);
    if (!(new_files))
    {
      return NULL; // can't allocate memory return NULL as error state
    }
    else
    {
      // set capacity and return
      file_list->file_capacity = 5;
      return new_files;
    }
  }
  else // double the capacity and realloc
  {
    int new_cap = file_list->file_capacity * 2;
    struct dirent *temp_files =
        realloc(file_list->files, sizeof(*file_list->files) * new_cap);
    if (!(temp_files))
    {
      return NULL; // can't allocate mem for files, return NULL as error state
    }
    else
    {
      file_list->file_capacity = new_cap;
      return temp_files;
    }
  }
}
