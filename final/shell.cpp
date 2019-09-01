#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <cstring>

using namespace std;

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
		printf("%s\n",buffer);


		if(strcmp(buffer,"quit")==0)
			flag=false;

	}

	return 0;
}