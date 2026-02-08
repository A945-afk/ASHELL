#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 512
int main(int argc, char *argv[]) 
{
  while(true)
  {
    // Flush after every printf
    setbuf(stdout, NULL);
    printf("$ ");
    char ibuf[MAX_INPUT];
    fgets(ibuf, MAX_INPUT, stdin);
    ibuf[strcspn(ibuf, "\n")] = '\0';
    if (!strcasecmp(ibuf, "exit"))
    {
      break;
    }
    
    printf("%s: command not found\n", ibuf);
  }
  return 0;
}
