#ifndef AMIO_H
#define AMIO_H

#define MAX_INPUT 512

//read input
char* read_line(FILE* file, char* prev);

//tokenize string
char** tokenize(char* command, char* splt);
void freetok(char** token);


#endif