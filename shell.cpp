#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <unordered_map>

#include "environmenthandler.h"
#include "helper.h"
#include "shellrunner.h"

using namespace std;

char* commands[512];
unordered_map <string,string> environment_var;
unordered_map <string,string> executable_var;

void sigint_handler(int signum){
  //give warning that the signal has been disabled
  // wait for ENTER to be pressed before returning to the command line.
  printf("\nInterrupt Signal disabled.Press 'Enter' to continue.Use 'quit' command to exit.\n");
  fflush(stdout);
}

int main(){
	signal(SIGINT, sigint_handler);
	bool flag=true;

	// CheckFile();
	FetchEnvironmentVariables(environment_var,executable_var);

	while(flag){
		char buffer[4096];
		PutPS1(environment_var);
		int i=0;
		char c;
		bool flag_inside=false,flag_repeat=false;

		do {
    		c=getchar();
    		if(c==' ' && flag_repeat){
    			continue;
    		}
    		else if(c==' ' && !flag_repeat){
    			buffer[i]=c;
    			++i;
    			if(flag_inside)
    				continue;
    			flag_repeat=true;
    		}
    		else if(c=='"'){
    			buffer[i]=c;
    			++i;
    			flag_inside=!flag_inside;
    		}
    		else if(c=='\t'){
    			printf("Tab");
    		}
    		else{
    			buffer[i]=c;
    			++i;
    			if(flag_repeat)
    				flag_repeat=!flag_repeat;
    		}
  		}while(c!='\n');
		if(buffer[strlen(buffer)-1]=='\n')
			buffer[strlen(buffer)-1]='\0';

		if(strcmp(buffer,"exit")==0){
				flag=false;
		}
		else if(strlen(buffer)==0){
			continue;
		}
		else{
			bool check_flag=break_command(buffer,environment_var,executable_var,commands);
			if((buffer[0]=='c') && (buffer[1]=='d') && (buffer[2]==' ')){
				chdir(commands[1]);
			}
			else if(!check_flag){
				printf("Invalid Command\n");
			}
			else{
				// cout<<"execute"<<endl;
					execute(buffer,environment_var,executable_var,commands);}
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
		for(int i=0;commands[i];++i){
			commands[i]=NULL;
		}
	}

	return 0;
}