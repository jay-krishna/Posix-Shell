#include "getinput.h"

void display_options(char buffer[],int top_buffer,string display,unordered_map <string,string> environment_var){

	// write(STDOUT_FILENO,"\n\nKey was pressed will get back soon\n\n",strlen("\n\nKey was pressed will get back soon\n\n"));
	// write(STDOUT_FILENO,"1)One\n2)Two\n3)Three\n\n",strlen("1)One\n2)Two\n3)Three\n\n"));

  auto data=TabDisplay(buffer,environment_var);
  write(STDOUT_FILENO,data.c_str(),(size_t)data.size());
  // write(STDOUT_FILENO,"Hello",strlen("Hello"));
	PS1Display(display);
	write(STDOUT_FILENO,buffer,top_buffer);

}

void display_optionsH(char buffer[],int top_buffer,string display,unordered_map <string,string> environment_var){

  // write(STDOUT_FILENO,"\n\nKey was pressed will get back soon\n\n",strlen("\n\nKey was pressed will get back soon\n\n"));
  // write(STDOUT_FILENO,"1)One\n2)Two\n3)Three\n\n",strlen("1)One\n2)Two\n3)Three\n\n"));

  auto data=HistoryDisplay(buffer,environment_var);
  write(STDOUT_FILENO,data.c_str(),(size_t)data.size());
  // write(STDOUT_FILENO,"Hello",strlen("Hello"));
  PS1Display(display);
  write(STDOUT_FILENO,buffer,top_buffer);

}

void error_func(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode(struct termios initial_state) {
  if (tcsetattr(STDIN_FILENO, TCSADRAIN, &initial_state) == -1)
    error_func("ERROR");
}

struct termios enableRawMode() {
	struct termios initial_state;
  if (tcgetattr(STDIN_FILENO, &initial_state) == -1) error_func("ERROR");
  struct termios raw = initial_state;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSADRAIN, &raw) == -1) error_func("ERROR");

  return initial_state;
}

int logkey(char* buffer,int &top_buffer, string display,unordered_map <string,string> environment_var) {
  int status;
  char c;
  int flag=1;
  int enter_flag=1;
  while ((status = read(STDIN_FILENO, &c, 1)) != 1) {
    if (status==-1&&errno!=EAGAIN) error_func("read");
  }
  switch(c){
  	case DEL:{
  		if(top_buffer>0){
  			--top_buffer;
	  		write(STDOUT_FILENO, "\033[1D",4);
	  		write(STDOUT_FILENO, "\033[0K",4);
	  	}
  		flag=0;
  		break;
  	}
  	case CTRLR:{
  		display_optionsH(buffer,top_buffer,display,environment_var);
  		flag=0;
  		break;
  	}
  	case '\t':{
  		display_options(buffer,top_buffer,display,environment_var);
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
  } else {
  	if(!iscntrl(c) && flag){
  	    	write(STDOUT_FILENO,&c, 1);
  	    	buffer[top_buffer++]=c;}
  }

  if(!enter_flag)
  		buffer[top_buffer]='\0';

  return enter_flag;
}

void PS1Display(string display){
	write(STDOUT_FILENO, display.c_str(),display.size());
}

void sendinput(char* buffer,string display,unordered_map <string,string> environment_var){
	// enableRawMode();
	int it=1;
	int top_buffer=0;
	PS1Display(display);
	while (it) {
		// write(STDOUT_FILENO, "INSIDE", 6);
		it=logkey(buffer,top_buffer,display,environment_var);
	}
	// disableRawMode();
	// cout<<buffer<<endl;
}