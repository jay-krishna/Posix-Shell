#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <sys/wait.h>
#include <utility>
#include <fstream>
#include <ctime>
#include <fcntl.h>
#include <errno.h>

#include "helper.h"
#include "environmenthandler.h"

using namespace std;

#ifndef __shellrunnerscript_H_INCLUDED__
#define __shellrunnerscript_H_INCLUDED__

void executeS(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
void ExecuteKernelS(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]);
void EchoExecuteS(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]);
string ResolveDS(char * buffer,unordered_map <string,string> &environment_var,unordered_map <string,string> &local_var);
void WriteOutput(string one, string two,unordered_map<string,string>environment_var);

void executeP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count);
void executeRedAP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count);
void executeRedP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count);
void ExecuteKernelP(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[],int count);
#endif