#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/stat.h>
#include<unistd.h>
#include"ambuilt.h"






int main(int argc, char *argv[]) 
{
  int exe;
  while(ext_con)
  {
    exe=0;
    setbuf(stdout, NULL);
    printf("$ ");
    char* command_line = read_line(stdin);
    char** tokens = tokenize(command_line, " \n\t\r");
    if (!tokens)
    {
      if(command_line) free(command_line);
      continue;
    }
    for (int i = 0; builtins[i].key; i++)
    {
      if (!(strcmp(tokens[0], builtins[i].key)))
      {
        builtins[i].func(tokens);
        exe=1;
        goto end_op;
      }
    }
    char* full_path = exemap(tokens[0]);
    if (full_path)
    {
      execute(full_path, tokens);
      exe = 1;
    }
    free(full_path);
    end_op:
    if(!exe) printf("%s: command not found\n", tokens[0]);
    free(command_line); free(tokens);
  }
  return 0;
}