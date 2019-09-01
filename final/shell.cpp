#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <vector>
#include <algorithm>

using namespace std;

char* commands[512];

void PutPS1();

void sigint_handler(int signum){
  //give warning that the signal has been disabled
  // wait for ENTER to be pressed before returning to the command line.
  printf("\nInterrupt Signal disabled.Press 'Enter' to continue.Use 'quit' command to exit.\n");
  fflush(stdout);
}

void PutPS1(){

	struct passwd *pw;
  	uid_t uid;
  	char buffer[2048];
  	string display="";
  	bool flag=false;

	uid = geteuid();
  	pw = getpwuid(uid);
  	string display_user(pw->pw_name);
  	if(display_user=="root")
  		flag=true;
  	display_user+="@";

  	gethostname(buffer,HOST_NAME_MAX);
  	string display_host(buffer);
  	display_host+=": ";

  	getcwd(buffer,FILENAME_MAX);
  	string display_cwd(buffer);
  	if(flag)
  		display_cwd+="# ";
  	else
  		display_cwd+="$ ";

  	display+=display_user+display_host+display_cwd;
  	printf("%s",display.c_str());

}

void break_command(char buffer[]){

	string temp="";
	int count=0;
	char del=' ';
	char til='~';
	struct passwd *pw;
  				uid_t uid;
				uid = geteuid();
  				pw = getpwuid(uid);
  				string display_user(pw->pw_name);
  				display_user="/home/"+display_user;
	for (unsigned int i = 0; i<strlen(buffer); ++i)
	{
		if(buffer[i]==del){
			commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
			strcpy(commands[count-1],temp.c_str());
			temp="";
		}
		else{
			if(buffer[i]==til)
				temp+=display_user;
			else
				temp+=buffer[i];}
	}

	commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
	strcpy(commands[count-1],temp.c_str());
	commands[count]=NULL;

}

void execute(char buffer[]){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);
	}
	else if(pid==0){
		break_command(buffer);
		// printf("%s\n",commands[0]);
		// printf("%s\n",commands[1]);
		// printf("%s\n",commands[2]);
		auto e = execvp(commands[0],commands);
	
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

int main(){
	signal(SIGINT, sigint_handler);
	// string n;
	char buffer[4096];
	bool flag=true;

	while(flag){

		PutPS1();
		fgets(buffer,sizeof(buffer),stdin);
		if(buffer[strlen(buffer)-1]=='\n')
			buffer[strlen(buffer)-1]='\0';

		if(strcmp(buffer,"exit")==0){
				flag=false;}
		else{
			execute(buffer);
		}

	}

	return 0;
}