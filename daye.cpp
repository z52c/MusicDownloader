#include "daye.h"

int getStringBetweenAandB(const char* a,const char* A,const char* B,char *find)
{

  //空字符串返回-1
  if(!a || !A || !B)
    return -1;

  char source[strlen(a)+1];
  strcpy(source,a);

  char * startIndex=NULL;
  if(!(startIndex=strstr(source,A)))
    return -2;

  startIndex=startIndex+strlen(A);
  char *endIndex=NULL;
  if(!(endIndex=strstr(startIndex,B)))
    return -3;

  strncpy(find,startIndex,endIndex-startIndex);
  find[endIndex-startIndex]='\0';
  return 0;
}
