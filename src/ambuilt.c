#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"ambuilt.h"





//reads from input, 512B chunks
char* read_line(FILE* file)
{ if(!file) return NULL;
  char* buffer = malloc(MAX_INPUT);
  char* temp = NULL;
  int len = 0;
  while (fgets(buffer + len,MAX_INPUT,file))
  {
    len = strlen(buffer);
    if(buffer[len-1] == '\n') break;
    temp = realloc(buffer, len + MAX_INPUT);
    if(!temp) {free(buffer); perror("malloc failure"); return NULL;}
    buffer = temp;
  }
  return buffer;
}



//makes string tokens
char** tokenize(char* command, char* splt)
{
  char** tokens = NULL;
  char** temp = NULL;
  int i = 0;
  while(*command != '\0')
  {
    command += strspn(command, splt);
    if(*command == '\0') break;
    temp = realloc(tokens, (sizeof(*tokens) * (i + 1)));
    if(!temp) {free(tokens); return NULL;}
    temp[i] = command;
    tokens = temp;
    command += strcspn(command, splt);
    i++;
    if(*command == '\0') break;
    *command = '\0';
    command += 1;
  }
  if(tokens) temp = realloc(tokens, (sizeof(*tokens) * (i + 1)));
  if (!temp)
  {
    free(tokens);
    return NULL;
  }
  
  temp[i] = NULL;
  tokens = temp;
  return tokens;
}



//checks exe permissions and returns path
char* exemap(char* fln)
{
  int len = strlen(fln);
  struct stat buf;
    char* enp = getenv("PATH");
    char* p = malloc(strlen(enp) + 1);if(!p) return NULL;strcpy(p,enp); char** path = tokenize(p,":");
    char* full_path;
    for (int j = 0; path[j]; j++)
    {
      int x = strlen(path[j]);
      full_path = malloc(x+len+2);
      full_path[x+len+1] = '\0';
      if(strchr(fln,'/')) strcpy(full_path,fln);
      else sprintf(full_path, "%s/%s", path[j],fln);
      if(stat(full_path, &buf)==0 && S_ISREG(buf.st_mode) && access(full_path, X_OK)==0) 
      {free(p); free(path);return full_path;}
      free(full_path);
    }
    free(p); free(path);
    return NULL;
}



//echo line
int builtin_echo(char** tokens)
{
  for (int i = 1; tokens[i]; i++) printf("%s ", tokens[i]); printf("\n");
  return 0;
}



//type determiner
int builtin_type(char** tokens)
{
  for (int i = 1; tokens[i]; i++)
  {
    int found = 0;
    for (int j = 0; builtins[j].key; j++)
    {
      if(strcmp(tokens[i],builtins[j].key)) continue;
      printf("%s is a shell builtin\n", tokens[i]);
      goto loop_end;
    }
    char* full_path = exemap(tokens[i]);
    if(exemap(tokens[i])) {printf("%s is %s\n",tokens[i],full_path); free(full_path); found = 1; continue;}
    if(!found) printf("%s: not found\n", tokens[i]);
    loop_end:
  }
  return 0;
}



//exit shell
int ext_con = 1;
int builtin_exit(char** tokens)
{
  ext_con = 0;
  return ext_con;
}



//execute program
int execute(char* path, char** args)
{
  pid_t pid = fork();
  if (pid<0)
  {
    perror("fork");
    return -1;
  }
  if(pid==0)
  {execve(path,args,__environ);perror("execve"); exit(1);}
  int status;
  waitpid(pid, &status, 0);
}



//find current directory path
int builtin_pwd(char** tokens)
{
  char* dn = getcwd(NULL,0);
  if (!dn) {perror("getcwd"); return 1;}
  printf("%s\n", dn);
  free(dn);
}