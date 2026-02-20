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


//take stdin and return a char*
char* read_line(FILE* file);


//tokenize the command lines
char** tokenize(char* command, char* splt);


//exe permission checker
char* exemap(char* fln);


// echo
int builtin_echo(char** tokens);


//type
int builtin_type(char** tokens);


//exit
int builtin_exit(char** tokens);


//execution
int execute(char* path, char** args);



//builtin list
static struct builtin builtins[] = 
{ 
  {"echo", builtin_echo},
  {"type", builtin_type},
  {"exit", builtin_exit},
  {NULL,NULL}
};


#endif