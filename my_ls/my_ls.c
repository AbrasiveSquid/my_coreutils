#include "my_ls_helper.h"

int main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;
  char *pathname = NULL;

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

  struct dirent curr_file; // use to have make function more readable
  for (size_t i = 0; i < file_list.file_count; i++)
  {
    curr_file = file_list.files[i];
    if (strcmp(curr_file.d_name, "..") == 0 ||
        strcmp(curr_file.d_name, ".") == 0)
    {
      continue;
    }
    printf("%-*s", (int)file_list.field_width + 2, curr_file.d_name);
    if (i % 5 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");

  return 0;
}
