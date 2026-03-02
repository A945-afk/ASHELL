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
char** toke(char* command, char* splt, char* anc);
#define tokenize(command,splt) toke(command,splt,"\"\'")
void freetok(char** token);

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