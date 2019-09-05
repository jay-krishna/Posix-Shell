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
unordered_map <string,string> alias_var;
unordered_map <string,string> new_environment_var;
unordered_map <string,string> new_alias_var;

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

	FetchEnvironmentVariables(environment_var,executable_var,new_environment_var);

	while(flag){
		cout<<getpid()<<" "<<getppid()<<endl;
		char buffer[4096];
		// cout<<"Above"<<endl;
		FetchBashrcVariables(environment_var,executable_var,alias_var,new_environment_var,new_alias_var);
		PutPS1(environment_var);
		int i=0;
		char c;
		bool flag_inside=false,flag_repeat=false;

		do{
    		c=getchar();
    		// cout<<"Inside"<<endl;
    		if(c=='\t'){
    			printf("Tab is pressed\n");
    		}
    		else if(c==' ' && flag_repeat){
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
    		// else if(c=='\t'){
    		// 	printf("Tab is pressed\n");
    		// }
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
		else if((buffer[0]=='.')&&(buffer[1]=='/')){

				// string s(buffer);
				// commands[0]="./s.sh";
				// cout<<"I am in"<<endl;
				// execve(commands[0],NULL,NULL);
			ExecuteScript(buffer,environment_var,commands);
		}
		else{

			string command_check(buffer);
			command_check=CheckForAlias(command_check,alias_var);
			MakeCharArray(command_check,buffer);
			// cout<<"CheckPoint1"<<endl;

			bool check_flag=break_command(buffer,environment_var,executable_var,commands);
			// cout<<"CheckPoint2"<<endl;
			if((buffer[0]=='c') && (buffer[1]=='d') && (buffer[2]==' ')){
				chdir(commands[1]);
			}
			else if((strcmp(commands[0],"path")==0)||(strcmp(commands[0],"PATH")==0)){
				// cout<<"One"<<endl;
				// cout<<commands[0]<<endl;
				// cout<<commands[1]<<endl;
				// cout<<commands[2]<<endl;
				AddPath(commands,new_environment_var);
			}
			else if((strcmp(commands[0],"ALIAS")==0)||(strcmp(commands[0],"alias")==0)){
				AddAlias(commands,new_alias_var);
			}
			else if((buffer[0]=='e')&&(buffer[1]=='c')&&(buffer[2]=='h')&&(buffer[3]=='o')&&(buffer[4]==' ')){
				// cout<<"three"<<endl;
				// commands[0]=(char*)"echo";
				// cout<<commands[0]<<endl;
				// cout<<commands[1]<<endl;
				// cout<<commands[2]<<endl;
			}
			// else if((buffer[0]=='.')&&(buffer[1]=='/')){
			// 	string s(buffer);
			// 	commands[0]="./shell";
			// 	execve(commands[0],NULL,NULL);
			// }
			else if(!check_flag){
				printf("Invalid Command\n");
			}
			else{
				// cout<<"execute"<<endl;
				cout<<commands[0]<<endl;
				execute(buffer,environment_var,executable_var,commands);
			}
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
		for(int i=0;commands[i];++i){
			commands[i]=NULL;
		}
	}

	return 0;
}