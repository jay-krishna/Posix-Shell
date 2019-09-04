#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define DEL (127)
#define CTRLR (18)
#define ASCII_ESC 27

struct termios initial;

char buffer[1024];
int top=0;

void error_func(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &initial) == -1)
    error_func("ERROR");
}
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &initial) == -1) error_func("ERROR");
  // atexit(disableRawMode);
  struct termios raw = initial;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) error_func("ERROR");
}

void display(){

	write(STDOUT_FILENO,"\n\nKey was pressed will get back soon\n\n",strlen("\n\nKey was pressed will get back soon\n\n"));
	write(STDOUT_FILENO,"1)One\n2)Two\n3)Three\n\n",strlen("1)One\n2)Two\n3)Three\n\n"));

	write(STDOUT_FILENO,buffer,top);

}

int editorReadKey() {
  int nread;
  char c;
  int flag=1;
  int enter_flag=1;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) error_func("read");
  }
  switch(c){
  	case DEL:{
  		if(top>0){
  			--top;
	  		write(STDOUT_FILENO, "\033[1D",4);
	  		write(STDOUT_FILENO, "\033[0K",4);
	  	}
  		flag=0;
  		break;
  	}
  	case CTRLR:{
  		display();
  		flag=0;
  		break;
  	}
  	case '\t':{
  		display();
  		flag=0;
  		break;
  	}
  	case '\n':{
  		write(STDOUT_FILENO, "\n", 1);
  		flag=0;
  		enter_flag=0;
  		break;
  	}
  }
  if (c == '\x1b' && flag) {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1);// return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1);// return '\x1b';
    if (seq[0] == '[') {
      switch (seq[1]) {
        case 'A':flag=0;break;// write(STDOUT_FILENO, "UP", 3);break;
		case 'B':flag=0;break;// write(STDOUT_FILENO, "DOWN", 5);break;
		case 'C':flag=0;break;// write(STDOUT_FILENO, "RIGHT", 6);break;
		case 'D':flag=0;break;// write(STDOUT_FILENO, "LEFT", 5);break;
      }
    }
    // return '\x1b';
  } else {
  	if(!iscntrl(c) && flag){
  	    	write(STDOUT_FILENO,&c, 1);
  	    	buffer[top++]=c;}
  }

  if(!enter_flag)
  		buffer[top]='\0';

  return enter_flag;
}

// void editorProcessKeypress() {
//   char c = editorReadKey();

//   if(!iscntrl(c))
//     	write(STDOUT_FILENO,&c, 1);
// }

/*** init ***/
int main() {
  enableRawMode();
  int it=1;
  while (it) {
  	// write(STDOUT_FILENO, "INSIDE", 6);
    it=editorReadKey();
  }
  disableRawMode();

  printf("Buffer is %s\n",buffer);
  return 0;
}