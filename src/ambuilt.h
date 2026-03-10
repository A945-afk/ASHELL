#ifndef ambuilt_h
#define ambuilt_h



typedef int (*built_func)(char** argv);
struct  builtin
{
  char* key;
  built_func func;
};
extern int ext_con;


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

//path directory
int builtin_pwd(char** tokens);

//change directory
int builtin_cd(char** tokens);


//builtin list
static struct builtin builtins[] = 
{ 
  {"echo", builtin_echo},
  {"type", builtin_type},
  {"exit", builtin_exit},
  {"pwd", builtin_pwd},
  {"cd", builtin_cd},
  {NULL,NULL}
};


#endif