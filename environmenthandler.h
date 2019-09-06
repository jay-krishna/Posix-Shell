#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <limits.h>
#include <regex>
#include <dirent.h>
#include <pwd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

#include "helper.h"

using namespace std;

#ifndef __environmenthandler_H_INCLUDED__
#define __environmenthandler_H_INCLUDED__

vector<string> CheckProfileFile(unordered_map <string,string> environment_var);
void AddPath(char* commands[],unordered_map <string,string> &new_environment_var);
void AddAlias(char* commands[],unordered_map <string,string> &new_alias_var);
void CheckBashrcFile(unordered_map <string,string> &environment_var,const char *env_var[],unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var);
string PutPS1(unordered_map <string,string> &environment_var);
void AddPath(char* commands[],unordered_map <string,string> &new_environment_var);
void traverse(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var);
void FetchHome(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[],unordered_map <string,string> &new_environment_var,unordered_map <string,string> &local_var);
void FetchUserHostname(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[],vector<string>locations);
void FetchPath(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[],vector<string>locations);
void FetchEnvironmentVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var);
// void FetchEnvironmentVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var);
void FetchBashrcVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var);

#endif