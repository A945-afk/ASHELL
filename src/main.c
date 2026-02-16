#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/stat.h>
#include<unistd.h>


#define MAX_INPUT 512
typedef int (*built_func)(char** argv);

struct  builtin
{
  char* key;
  built_func func;
};


int ext_con = 1;


//take stdin and return a char*
char* read_line(FILE* file);

//tokenize the command lines
char** tokenize(char* command, char* splt);

// echo
int builtin_echo(char** tokens);

//type
int builtin_type(char** tokens);

//exit
int builtin_exit(char** tokens);




static struct builtin builtins[] = 
{ 
  {"echo", builtin_echo},
  {"type", builtin_type},
  {"exit", builtin_exit}
};






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
        break;
      }
    }
    


    if(!exe) printf("%s: command not found", tokens[0]);
    if(ext_con) printf("\n");
    free(command_line); free(tokens);
  }
  return 0;
}


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


int builtin_echo(char** tokens)
{
  for (int i = 1; tokens[i]; i++) printf("%s ", tokens[i]);
  return 0;
}


int builtin_type(char** tokens)
{
  for (int i = 1; tokens[i]; i++)
  {
    int found = 0;
    for (int j = 0; builtins[j].key; j++)
    {
      if(strcmp(tokens[i],builtins[j].key)) continue;
      printf("%s is a shell builtin", tokens[i]);
      goto loop_end;
    }
    struct stat buf;
    char* enp = getenv("PATH");
    char* p = malloc(strlen(enp) + 1);if(!p) return 1;strcpy(p,enp); char** path = tokenize(p,":");
    for (int j = 0; path[j]; j++)
    {
      char full_path[1024];
      if(strchr(tokens[i],'/')) strcpy(full_path,tokens[i]);
      else sprintf(full_path, "%s/%s", path[j],tokens[i]);
      if(stat(full_path, &buf)==0 && S_ISREG(buf.st_mode) && access(full_path, X_OK)==0) 
      {printf("%s is %s",tokens[i],full_path); found = 1; break;}
    }
    free(p); free(path);
    if(!found) printf("%s: not found", tokens[i]);
    loop_end:
  }
  return 0;
}


int builtin_exit(char** tokens)
{
  ext_con = 0;
  return ext_con;
}