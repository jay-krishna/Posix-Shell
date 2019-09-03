#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <limits.h>
#include <regex>
#include <dirent.h>
#include <pwd.h>
#include <fstream>

#include "helper.h"

using namespace std;

#ifndef __environmenthandler_H_INCLUDED__
#define __environmenthandler_H_INCLUDED__

void PutPS1(unordered_map <string,string> environment_var);
void traverse(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var);
void FetchEnvironmentVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var);

#endif