#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <unordered_map>
#include <termios.h>
#include <stdio.h>

#include "environmenthandler.h"
#include "helper.h"
#include "shellrunner.h"
#include "getinput.h"

using namespace std;

char* commands[512];
unordered_map <string,string> environment_var;
unordered_map <string,string> executable_var;
unordered_map <string,string> alias_var;
unordered_map <string,string> new_environment_var;
unordered_map <string,string> new_alias_var;
unordered_map <string,string> local_var;

void sigint_handler(int signum){
  //give warning that the signal has been disabled
  // wait for ENTER to be pressed before returning to the command line.
  printf("\nInterrupt Signal disabled.Press 'Enter' to continue.Use 'quit' command to exit.\n");
  fflush(stdout);
}

int main(){
	// cout<<"Above"<<endl;
	signal(SIGINT, sigint_handler);
	bool flag=true;

	FetchEnvironmentVariables(environment_var,executable_var,new_environment_var,new_alias_var,local_var);

	while(flag){
		// cout<<"while"<<getpid()<<" "<<getppid()<<endl;
		char buffer_before[4096];
		char buffer[4096];
		// cout<<"Above"<<endl;
		FetchBashrcVariables(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var);
		string display=PutPS1(environment_var);
		int i=0,j;
		char c;
		bool flag_inside=false,flag_repeat=false;

		struct termios initial_state=enableRawMode();
		sendinput(buffer_before,display);
		disableRawMode(initial_state);

		// cout<<"From Input"<<buffer_before<<endl;

		for(j=0;j<(int)strlen(buffer_before);++j){
    		c=buffer_before[j];
    		// cout<<"Inside"<<endl;
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
    		else{
    			buffer[i]=c;
    			++i;
    			if(flag_repeat)
    				flag_repeat=!flag_repeat;
    		}
  		}
		buffer[i]='\0';

		if(strcmp(buffer,"exit")==0){
				int id=getpid();
				string filename=environment_var.find("HOME")->second+"/"+"."+to_string(id)+"_.txt";
				// cout<<filename<<endl;
				remove(filename.c_str());
		// 		if( remove(filename.c_str()) != 0 )
  //   perror( "Error deleting file" );
  // else
  //   puts( "File successfully deleted" );
				// remove(filename.c_str());
				flag=false;
		}
		else if(strlen(buffer)==0){
			continue;
		}
		else if((buffer[0]=='.')&&(buffer[1]=='/')){
			ExecuteScript(buffer,environment_var,commands,local_var);
		}
		else{
			ExecuteKernel(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
		for(int i=0;commands[i];++i){
			commands[i]=NULL;
		}
	}

	return 0;
}