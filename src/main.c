#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 512
int main(int argc, char *argv[]) 
{
  // Flush after every printf
  setbuf(stdout, NULL);
  char ibuf[MAX_INPUT];
  fgets(ibuf, MAX_INPUT, stdin);
  ibuf[strcspn(ibuf, "\n")] = '\0';
  printf("%s: command not found\n", ibuf);

  return 0;
}
