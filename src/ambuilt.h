#ifndef ambuilt_h
#define ambuilt_h



#define MAX_INPUT 512
typedef int (*built_func)(char** argv);

struct  builtin
{
  char* key;
  built_func func;
};


extern int ext_con;

//read input
char* read_line(FILE* file);

//tokenize string
char** tokenize(char* command, char* splt);

//exe permission checker
char* exemap(char* fln);

//echo
int builtin_echo(char** tokens);

//type
int builtin_type(char** tokens);

//exit
int builtin_exit(char** tokens);

//execution
int execute(char* path, char** args);

//pwd
int builtin_pwd(char** tokens);


//builtin list
static struct builtin builtins[] = 
{ 
  {"echo", builtin_echo},
  {"type", builtin_type},
  {"exit", builtin_exit},
  {"pwd", builtin_pwd},
  {NULL,NULL}
};


#endif