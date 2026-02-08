#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 512

char* builtin[] = {"exit","echo","type"};
int built_count = 3;



int main(int argc, char *argv[]) 
{
  while(1)
  {
    // Flush after every printf
    setbuf(stdout, NULL);
    printf("$ ");
    char ibuf[MAX_INPUT];
    fgets(ibuf, MAX_INPUT, stdin);
    ibuf[strcspn(ibuf, "\n")] = '\0';
    if (!strcasecmp(ibuf, "exit")) break;
    if(!strncasecmp(ibuf, builtin[1], 5))
    {
      char* echo_out = ibuf + 5;
      printf("%s\n",echo_out);
      continue;
    }
    else if(!strncasecmp(ibuf, builtin[2], 5))
    {
      char* type_out = ibuf + 5;
      int type_match = 0;
      for (int i = 0; i < built_count; i++)
      {
        if (!strcasecmp(builtin[i], type_out))
        {
          printf("%s is a shell builtin",type_out);
          type_match = 1;
          break;
        }
        
      }
      if (!type_match)
      {
        printf("%s: not found", type_match);
      }
      continue;
    }
    printf("%s: command not found\n", ibuf);
  }
  return 0;
}
