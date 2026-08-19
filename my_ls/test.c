#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int rows = 1;
  int file_count = atoi(argv[1]);
  int max_col = atoi(argv[2]);

  while ((file_count / rows) + (file_count % rows != 0) > max_col)

    rows++;

  printf("num of rows: %d\n", rows);

  return 0;
}
