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
char** toke(char* command_str, char* splt, char* anc)
{
  char* command = malloc(strlen(command_str)+1);
  if(!command) return NULL;
  strcpy(command,command_str); char* base=command; command[strlen(command_str)]='\0';
  int tindex = 0; char **token = NULL, **temp=NULL; char* tstr = NULL;
  int issplit = 0;
  int index = 0, len = 0;
  int isanc = 0;
  char curanc[] = {'\0','\0'};
  int endanc;
  while (*command)
  {
    switch (isanc)
    {
    case 0:
      issplit = strspn(command,splt);
      switch (issplit)
      {
      case 0:
        if(!token || !token[tindex])
        {
          temp = realloc(token,(tindex+1)*sizeof(*token));
          if(!temp&&token) freetok(token);
          token = temp;
          if(!token) return NULL; token[tindex] = NULL;
        }
        isanc = strspn(command,anc);
        if(isanc)
        {
          curanc[0] = *command;
          command++;
          break;
        }
        if (!(token[tindex])||index>=len)
        {
          tstr = realloc(token[tindex],len+64);
          if(!tstr&&token[tindex]) free(token[tindex]);
          token[tindex]=tstr;
          if(!(token[tindex])) {freetok(token); return NULL;}
          tstr[len] = '\0';
          len+=64;
        }
        token[tindex][index]=*command;
        command++;
        index++;
        break;
      
      default:
        if(token&&token[tindex])
        {
          token[tindex][index]='\0';
          tindex++;
        }
        len = 0;
        index = 0;
        command+=issplit;
        break;
      }
      break;
    default:
      endanc = strcspn(command,curanc);
      if(!(token[tindex]) || len<=(index+endanc))
      {
        tstr = realloc(token[tindex],index+endanc+1);
        if(!tstr&&token[tindex]) free(token[tindex]);
        token[tindex]=tstr;
        if(!(token[tindex])) {freetok(token); return NULL;}
      }
      memcpy((token[tindex]+index),command,endanc);
      command+=endanc;
      index+=endanc; len = index + 1;
      if(*command==curanc[0]) {command++; isanc = 0;}
      else
      {
        free(base);
        printf("> ");
        base = read_line(stdin);
        command = base;
      }
      break;
    }
  }
  temp = realloc(token,(tindex+1)*sizeof(*token));
  if(!temp&&token) freetok(token);
  token = temp;
  if(!token) return NULL; token[tindex] = NULL;
  free(base);
  return token;
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


void freetok(char** token)
{
  if(!token) return;
  for (int i = 0; token[i]; i++)
  {
    free(token[i]);
  }
  free(token);
}