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
#include <regex>
#include <fstream>

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

void CheckFile(){
	ifstream infile(".my_bashrc");
	if(infile.fail()){
		infile.close();
		ofstream outfile(".my_bashrc",ios::out);
		outfile<<"PATH:/etc/environment"<<endl;
		outfile<<"HOME:/etc/passwd"<<endl;


	}
	else
		infile.close();
}

void traverse(){
	string temp=environment_var.find("PATH")->second;
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
	environment_var.clear();
	executable_var.clear();
	const char *env_var[5] = {"PATH","HOME","USER","HOSTNAME","PS1"};
	char *env_val[5];
	struct passwd *pw;
  	uid_t uid;
  	const char *delim=":";

	// for(int i=0; i<2; i++)
	// {
	// 	env_val[i] = getenv(env_var[i]);
	// 	make_path(env_var[i],env_val[i]);
	// }
  	ifstream infile("/etc/environment");
	string line;
	getline(infile,line);

	regex r("\"(.*?)\"");
	smatch m;
	regex_search(line, m, r);
	string s(m[1]);
	env_val[0]=(char*)s.c_str();
	make_path(env_var[0],env_val[0]);
	infile.close();
	
	uid = geteuid();
  	ifstream infile2("/etc/passwd");
  	string pattrn="(.*)(:"+to_string(uid)+":)(.*)";;
  	regex r1(pattrn);
  	int count=1;
    while (getline(infile2, line))
	{
		if(regex_match(line,r1)){
			char *token = strtok(const_cast<char*>(line.c_str()), delim);
			while (token != nullptr)
			{
				if(count==1){
					env_val[2]=token;
					make_path(env_var[2],env_val[2]);
				}
				if(count==6){
					env_val[1]=token;
					make_path(env_var[1],env_val[1]);
				}
				++count;
				token = strtok(nullptr, delim);
			}
		}
	}


  	pw = getpwuid(uid);
  	env_val[2] = pw->pw_name;
	make_path(env_var[2],env_val[2]);

	char buffer[128];
	gethostname(buffer,HOST_NAME_MAX);
	make_path(env_var[3],buffer);

	// for (auto x : environment_var) 
 //      cout << x.first << " " << x.second << endl;

	traverse();
}

void PutPS1(){

  	char buffer[2048];
  	string display="";
  	bool flag=false;
  	uid_t uid=geteuid();

  	string display_user=environment_var.find("USER")->second;
  	if(uid==0)
  		flag=true;
  	display_user+="@";

  	string display_host=environment_var.find("HOSTNAME")->second;
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

bool break_command(char buffer[]){

	string temp="";
	int count=0,cn=0;
	char del=' ';
	char til='~';
	string display_user=environment_var.find("HOME")->second;
	
	for (unsigned int i = 0; i<strlen(buffer); ++i)
	{
		if(buffer[i]==del){
			if(cn==0){
				auto it=executable_var.find(temp);
				if(it!=executable_var.end())
					temp=executable_var.find(temp)->second;
				else
					return false;
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
		auto it=executable_var.find(temp);
		if(it!=executable_var.end())
			temp=executable_var.find(temp)->second;
		else
			return false;
		++cn;
	}
	commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
	strcpy(commands[count-1],temp.c_str());
	commands[count]=NULL;

	return true;

}

void execute(char buffer[]){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);
	}
	else if(pid==0){
		// auto itr=
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

	CheckFile();
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
		// cout<<strlen(buffer)<<endl;
		if(strcmp(buffer,"exit")==0){
				flag=false;
		}
		else if(strlen(buffer)==0){
			continue;
		}
		else{
			if(!break_command(buffer)){
				printf("Invalid Command\n");
				continue;
			}
			if(strcmp(commands[0],"cd")==0){
				chdir(commands[1]);
			}
			else
				execute(buffer);
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
	}

	return 0;
}