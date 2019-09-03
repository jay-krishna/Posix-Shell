#include "helper.h"

void make_path(const char* var, char* val,unordered_map <string,string> &environment_var){
	string vr(var);
	string temp(val);
	environment_var.insert(make_pair(vr,temp));

}

bool break_command(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){

	string temp="";
	int count=0,cn=0;
	char del=' ';
	char til='~';
	string display_user=environment_var.find("HOME")->second;
	bool flag_valid=true;
	
	for (unsigned int i = 0; i<strlen(buffer); ++i)
	{
		if(buffer[i]==del){
			if(cn==0){
				auto it=executable_var.find(temp);
				if(it!=executable_var.end())
					temp=executable_var.find(temp)->second;
				else{
					temp=" ";
					flag_valid=false;
				}
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

	return flag_valid;

}