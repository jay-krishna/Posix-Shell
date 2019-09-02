#include<iostream>
#include <cstdlib>
#include <unordered_map> 
#include <string>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

using namespace std;

char* commands[512];

unordered_map <string,string> env_map;
unordered_map <string,string> execut;

void traverse(){
	cout<<env_map.find("PATH")->second<<endl;
	string temp=env_map.find("PATH")->second;
	int i=0;
	const char *delim=":";
	char *token = strtok(const_cast<char*>(temp.c_str()), delim);
	while (token != nullptr)
	{
		// out.push_back(std::string(token));
		// env_map.insert(make_pair(vr,string(token)));

		dirent *pdir;
		DIR *dir = opendir(token);


		if (dir == NULL) {
		return;
	}

	while (pdir = readdir(dir)){
		if(strcmp(pdir->d_name,".")==0||strcmp(pdir->d_name,"..")==0)
			continue;
		string real=string(token)+"/"+pdir->d_name;
		execut.insert(make_pair(pdir->d_name,real));
	}
   closedir(dir);
		token = strtok(nullptr, delim);
	}


}

void make_path(const char* var, char* val){
	string vr(var);
	string temp(val);
	env_map.insert(make_pair(vr,temp));

}

void break_command(char buffer[]){

	string temp="";
	int cn=0;
	int count=0;
	char del=' ';
	char til='~';
	// struct passwd *pw;
 //  				uid_t uid;
	// 			uid = geteuid();
 //  				pw = getpwuid(uid);
 //  				string display_user(pw->pw_name);
 //  				display_user="/home/"+display_user;
	for (unsigned int i = 0; i<strlen(buffer); ++i)
	{
		if(buffer[i]==del){
			if(cn==0){
				temp=execut.find(temp)->second;
				++cn;
			}
			commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
			strcpy(commands[count-1],temp.c_str());
			temp="";
		}
		else{
			// if(buffer[i]==til)
			// 	temp+=display_user;
			// else
				temp+=buffer[i];}
	}
	if(cn==0){
				temp=execut.find(temp)->second;
				++cn;
			}
	commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
	strcpy(commands[count-1],temp.c_str());
	commands[count]=NULL;

}


int main(){
	char buffer[4096];




 const char *env_var[5] = {"PATH","HOME","USER","HOSTNAME","PS1"};
	char *env_val[5];
	for(int i=0; i<3; i++)
	{
		env_val[i] = getenv(env_var[i]);
			make_path(env_var[i],env_val[i]);}

			// char buffer[128];
			gethostname(buffer,HOST_NAME_MAX);
	make_path(env_var[3],buffer);


			for (auto itr = env_map.begin(); itr != env_map.end(); ++itr) 
    { 
        cout  <<  '\t' << itr->first 
              <<  '\t' << itr->second << '\n'; 
    } 
    cout << endl;


    traverse();
    string q;
    bool flag=true;
    while(flag){
    	cout<<flag<<endl;
    	fgets(buffer,sizeof(buffer),stdin);
    	if(buffer[strlen(buffer)-1]=='\n')
			buffer[strlen(buffer)-1]='\0';
    	break_command(buffer);
    	string pathpass="PATH="+env_map.find("PATH")->second;
    	// cout<<pathpass<<endl;
    	char* e[2]={(char*)pathpass.c_str(),NULL};
    	cout<<commands[0]<<endl;
    	cout<<flag<<endl;
    	execve(commands[0],commands,e);
    	exit(1);
    	// cout<<flag<<endl;
    	// if(e1<0)
    	// 	printf("Your command FAILED!\n");
    }

	return 0;
}