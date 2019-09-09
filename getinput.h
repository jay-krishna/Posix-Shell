/*
  2019201019
  Termios Input
*/
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>

#include "search.h"

using namespace std;

#ifndef __getinput_H_INCLUDED__
#define __getinput_H_INCLUDED__

void disable(struct termios initial_state);
struct termios enable();
void display_options(char buffer[],int top_buffer,string display,unordered_map <string,string> environment_var,vector <string>executable_var2);
void display_optionsH(char buffer[],int top_buffer,string display,unordered_map <string,string> environment_var);
int logkey(char* buffer,int &top_buffer,string display,unordered_map <string,string> environment_var,vector <string>executable_var2);
void sendinput(char* buffer,string display,unordered_map <string,string> environment_var,vector <string>executable_var2);
void PS1Display(string display);

#define DEL (127)
#define CTRLR (18)
#define ASCII_ESC 27

// extern struct termios initial_state;

// char buffer[1024];
// extern int top_buffer=0;

#endif