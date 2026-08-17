#include "my_ls_helper.h"

int main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;
  char *pathname = NULL;
  int curr_len, field_width = 0;

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

  // find the max length of files in directory for formatting
  int file_count = 0;
  while ((dirp = readdir(dp)) != NULL)
  {
    curr_len = strlen(dirp->d_name);
    if (curr_len > field_width)
    {
      field_width = curr_len;
    }
    file_count++;
  }
  rewinddir(dp); // go back to start after getting max len
  char *file_list[file_count];

  // TODO need to add each entry to file_list strcpy? or can just add pointer?
  while ((dirp = readdir(dp)) != NULL)
  {
  }
  qsort(file_list, file_count, sizeof(file_list[0]), compare_filenames);

  int i = 0;
  while ((dirp = readdir(dp)) != NULL)
  {
    if (strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".") == 0)
    {
      continue;
    }
    printf("%-*s", field_width + 2, dirp->d_name);
    i++;
    if (i % 5 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");

  return 0;
}
