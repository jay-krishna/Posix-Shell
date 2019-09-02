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
#include <unordered_map>
#include <dirent.h>

using namespace std;

char* commands[512];
unordered_map <string,string> environment_var;
unordered_map <string,string> executable_var;

void PutPS1();

void sigint_handler(int signum){
  //give warning that the signal has been disabled
  // wait for ENTER to be pressed before returning to the command line.
  printf("\nInterrupt Signal disabled.Press 'Enter' to continue.Use 'quit' command to exit.\n");
  fflush(stdout);
}

void traverse(){
	// cout<<env_map.find("PATH")->second<<endl;
	string temp=environment_var.find("PATH")->second;
	// int i=0;
	const char *delim=":";
	char *token = strtok(const_cast<char*>(temp.c_str()), delim);
	while (token != nullptr)
	{
		dirent *pdir;
		DIR *dir = opendir(token);

		if (dir == NULL) {
			return;
		}

		while ((pdir = readdir(dir))){
			if(strcmp(pdir->d_name,".")==0||strcmp(pdir->d_name,"..")==0)
				continue;
			string real=string(token)+"/"+pdir->d_name;
			executable_var.insert(make_pair(pdir->d_name,real));
		}
   		closedir(dir);
		token = strtok(nullptr, delim);
	}
}

void make_path(const char* var, char* val){
	string vr(var);
	string temp(val);
	environment_var.insert(make_pair(vr,temp));

}

void FetchEnvironmentVariables(){
	const char *env_var[5] = {"PATH","HOME","USER","HOSTNAME","PS1"};
	char *env_val[5];
	for(int i=0; i<3; i++)
	{
		env_val[i] = getenv(env_var[i]);
		make_path(env_var[i],env_val[i]);
	}
	
	char buffer[128];
	gethostname(buffer,HOST_NAME_MAX);
	make_path(env_var[3],buffer);

	traverse();
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
	int count=0,cn=0;
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
			if(cn==0){
				temp=executable_var.find(temp)->second;
				++cn;
			}
			commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
			strcpy(commands[count-1],temp.c_str());
			temp="";
		}
		else{
			if(buffer[i]==til)
				temp+=display_user;
			else
				temp+=buffer[i];
		}
	}

	if(cn==0){
		temp=executable_var.find(temp)->second;
		++cn;
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

		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* e1[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
		auto e = execve(commands[0],commands,e1);
	
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
	bool flag=true;

	FetchEnvironmentVariables();

	while(flag){
		char buffer[4096];
		PutPS1();
		// fgets(buffer,sizeof(buffer),stdin);
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
    		else{
    			buffer[i]=c;
    			++i;
    			if(flag_repeat)
    				flag_repeat=!flag_repeat;
    		}

    		// buffer[i]=c;
    		// 	++i;
  		}while(c!='\n');

		if(buffer[strlen(buffer)-1]=='\n')
			buffer[strlen(buffer)-1]='\0';

		printf("Buffer is %s\n",buffer);

		if(strcmp(buffer,"exit")==0){
			cout<<"Inside"<<endl;
				flag=false;
		}
		else{
			execute(buffer);
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
	}

	return 0;
}