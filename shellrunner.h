#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <sys/wait.h>
#include <utility>
#include <fstream>
#include <ctime>
#include <fcntl.h>

#include "helper.h"
#include "environmenthandler.h"

using namespace std;

#ifndef __shellrunner_H_INCLUDED__
#define __shellrunner_H_INCLUDED__

void ExecuteScript(char buffer[],unordered_map <string,string> &environment_var,char* commands[],unordered_map <string,string> &local_var);
void execute(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
void ExecuteKernel(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]);
void EchoExecute(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]);
string ResolveD(char * buffer,unordered_map <string,string> &environment_var,unordered_map <string,string> &local_var);
void GetAlarmChild(string buffer_string,string display,unordered_map<time_t,pid_t> &alarmchilds);
void executeRed(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
void executeRedA(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
#endif