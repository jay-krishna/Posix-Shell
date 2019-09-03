#include <string>
#include <cstring>
#include <unordered_map>

using namespace std;

#ifndef __helper_H_INCLUDED__
#define __helper_H_INCLUDED__

void make_path(const char* var, char* val,unordered_map <string,string> &environment_var);
bool break_command(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);

#endif