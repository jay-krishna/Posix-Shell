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

#ifndef __getinput_H_INCLUDED__
#define __getinput_H_INCLUDED__

void disableRawMode(struct termios initial_state);
struct termios enableRawMode();
void display_options(char* buffer,int top_buffer,string display);
int logkey(char* buffer,int &top_buffer,string display);
void sendinput(char* buffer,string display);
void error_func(const char *s);
void PS1Display(string display);

#define DEL (127)
#define CTRLR (18)
#define ASCII_ESC 27

// extern struct termios initial_state;

// char buffer[1024];
// extern int top_buffer=0;

#endif