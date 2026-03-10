#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"ambuilt.h"
#include"amio.h"



//checks exe permissions and returns path
char* exemap(char* fln)
{
  int len = (fln)?strlen(fln):0; if(!len)return NULL;
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
      {free(p); freetok(path);return full_path;}
      free(full_path);
    }
    free(p); freetok(path);
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
    if(full_path) {printf("%s is %s\n",tokens[i],full_path); free(full_path); found = 1; continue;}
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



//change shell directory
int builtin_cd(char** tokens)
{
  if(!(tokens[1]) || !strcmp(tokens[1], "~"))
  {
    char* home = getenv("HOME");
    if(!home || chdir(home)<0) perror("cd");
    return 1;
  }
  struct stat buf;
  if(stat(tokens[1], &buf) || !S_ISDIR(buf.st_mode))
  {printf("cd: /non-existing-directory: No such file or directory\n"); return -1;}
  chdir(tokens[1]);
  return 0;
}


