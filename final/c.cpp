#include<iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <cstring>
using namespace std;

#define BUFF_SIZE 1000
using namespace std;

int main(int argc, char **argv, char* envp[])
{
  // Define buffer for user's command
  char *UsrCmd = (char *)malloc(sizeof(char) * BUFF_SIZE);
  char * token;
  char * ParaArray[100];
  int ParaIndex  = 0;  int i = 0;

  // printf("Here are your Environment Variables:\n");
  // while(envp[i]!=NULL)
  //   { printf("%s\n", envp[i]); i++; }
  // printf("================================\n\n%s\n\n",envp[4]);

  string sk="PATH=/bin";
  char* e[2]={(char*)sk.c_str(),NULL};

  // Get user's command
  while(true)
{  printf("ENTER A COMMAND:  ");
  fgets(UsrCmd, BUFF_SIZE+1, stdin);

  // We tokenize the user's command and load the tokens into ParaArray
  token = strtok(UsrCmd, " \t\n");
  ParaArray[ParaIndex] = token;  ParaIndex++;
  while(token != NULL)
    {
      token = strtok(NULL, " \t\n");
      ParaArray[ParaIndex] = token;  ParaIndex++;
    }
  ParaArray[ParaIndex]=NULL;

  // Now we run execve() with envp[] passed in as the environment variables
  cout<<ParaArray[0]<<endl;
  cout<<ParaArray[1]<<endl;
  execve(ParaArray[0], ParaArray, e);
  printf("Your command FAILED!\n");
}}