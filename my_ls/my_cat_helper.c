/* Contains the funciton definitions to support my_ls */

#include "my_ls_helper.h"

int compare_filenames(const void *a, const void *b)
{
  const char *a_str = a;
  const char *b_str = b;

  return strcmp(a, b);
}
