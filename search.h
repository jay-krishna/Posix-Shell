#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <dirent.h>
#include <unordered_map>
#include <fstream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

#define lx 95
#define sx 32

using namespace std;

#ifndef __search_H_INCLUDED__
#define __search_H_INCLUDED__

struct Node* GetLoc(string key,struct Node* root);
string print_v(std::vector<string> v);
void AddNode(struct Node *root, string key);
void FindWords(struct Node* root, char str[],int level,vector<string>&optionlist);
vector<string> FindWordsDriver(struct Node* root,string key);
string TabDisplay(char buffer[],unordered_map <string,string> environment_var,vector <string>executable_var2);
string HistoryDisplay(char buffer[],unordered_map <string,string> environment_var);

#endif