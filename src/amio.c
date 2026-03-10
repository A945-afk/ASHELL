#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"amio.h"

//reads from input, 512B chunks
char* read_line(FILE* file, char* prev)
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
  int sinq=0,douq=0,esc=0;char *quote="\'\"";
  int plen = (prev)?strlen(prev):0;
  int tlen = plen + len;
  temp = realloc(prev,tlen+1); if(!temp) {if(prev)free(prev);free(buffer);return NULL;}
  prev=temp;
  strcpy(prev+plen,buffer);
  free(buffer); buffer = prev;
  for (int i = 0; i < tlen; i++)
  {
    if(buffer[i]=='\\' && !(sinq%2)) {esc++;continue;}
    if(buffer[i]=='\"' && (!i||((buffer[i-1]!='\\'||!(esc%2))&&!(sinq%2))))  {douq++;continue;}
    if(buffer[i]=='\''&&!(douq%2))  {sinq++;continue;}
    esc=0;
  }
  if (sinq%2||douq%2||(tlen>2&&buffer[tlen-2]=='\\'))
  {
    printf("> ");
    temp = read_line(stdin,buffer);
    if(!temp){free(buffer); return NULL;}
    buffer=temp;

  }
  
  return buffer;
}



//makes string tokens
char** tokenize(char* command_str, char* splt)
{
  if(!command_str) return NULL;
  char curq='\0', **temp = NULL, *tstr=NULL, **tokens=NULL;
  int count = strlen(command_str), scount, tcount=0, qcon=0, bs=0;

  for (int i=0,strind=0; i<count; i++)
  {
    if(command_str[i]=='\''||command_str[i]=='\"')
    {
      if(!qcon){curq=command_str[i];qcon=1;continue;}
      if(curq==command_str[i]){qcon=0;continue;}
    }
    if(!tokens||!*(tokens+tcount))
    {
      temp = realloc(tokens,sizeof(*tokens)*(tcount+2));
      if(!temp){freetok(tokens);return NULL;}
      *(temp+tcount)=NULL;
      *(temp+tcount+1)=NULL;
      tokens = temp;
    }
    if(!tokens[tcount]||strind>=bs)
    {
      tstr = realloc(*(tokens+tcount),strind+64);
      if(!tstr) {freetok(tokens);return NULL;}
      tstr[strind]='\0';
      bs+=64;
      *(tokens+tcount)=tstr;
    }
    scount = strspn((command_str+i),splt);
    if(!qcon && scount)
    {
      if(strind) {*(*(tokens+tcount)+strind)='\0';tcount++;}
      i+=scount-1;bs=strind=0;
      continue;
    }
    if(command_str[i]=='\\' && !(qcon&&curq=='\''))i++;
    *(*(tokens+tcount)+strind)=command_str[i];
    strind++;
  }
  
  return tokens;
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