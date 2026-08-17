#include "my_ls_helper.h"

int main(int argc, char *argv[])
{
  DIR *dp;
  struct dirent *dirp;

  dp = opendir(argv[1]);
  dirp = readdir(dp);

  if (!(dirp))
  {
    perror(argv[1]);
    return 1;
  }

  return 0;
}
