#include "shellrunner.h"

void ExecuteScript(char buffer[],unordered_map <string,string> &environment_var,char* commands[]){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);
	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	// cout<<commands[0]
    	string temp(buffer);
    	commands[0]=(char*)malloc(strlen(temp.c_str())+1);
		strcpy(commands[0],temp.c_str());
		commands[1]=NULL;


		auto e = execve(commands[0],commands,env_array);
	
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}

}

void execute(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);
	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	// cout<<commands[0]
		auto e = execve(commands[0],commands,env_array);
	
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}