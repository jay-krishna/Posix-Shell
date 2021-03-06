/*
	2019201019
	Helper Module
*/
#include <string>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>

using namespace std;

#ifndef __helper_H_INCLUDED__
#define __helper_H_INCLUDED__

void make_path(const char* var, char* val,unordered_map <string,string> &environment_var);
bool break_command(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
string CheckForAlias(string command,unordered_map <string,string> alias_var);
void MakeCharArray(string command_check,char* buff);
bool break_command(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]);
bool CheckContains(char* buff,string pattern);
bool CheckContainsS(string buff,string pattern);
void RecordHistory(char buffer[],unordered_map<string,string> environment_var);

#endif