#ifndef AMIO_H
#define AMIO_H

#define MAX_INPUT 512


struct state_machine
{
    int curs;
    int nxts;
};

struct redirect {
    int fd;
    char *file;
};

struct command {
    char **argv;
    struct redirect *redirects;
};

struct pipeline {
    int background;
    int cmd_count;
    struct command *cmds;
};

//read input
char* read_line(FILE* file, char* prev);

//tokenize string
char** tokenize(char* cmd, char* splt, char* inde, char* opflag, char* flagless);
#define simple_token(cmd,splt) tokenize(cmd,splt,"","","")
void freetok(char** token);


#endif