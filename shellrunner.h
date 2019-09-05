#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#ifndef __shellrunner_H_INCLUDED__
#define __shellrunner_H_INCLUDED__

void ExecuteScript(char buffer[],unordered_map <string,string> &environment_var,char* commands[]);
void execute(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);

#endif