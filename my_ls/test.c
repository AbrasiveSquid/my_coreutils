#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{

  printf("%d\n", isatty(STDOUT_FILENO));
  return 0;
}
