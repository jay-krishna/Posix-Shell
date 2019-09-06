#include "shellrunner.h"

string ResolveD(char * buffer,unordered_map <string,string> &environment_var,unordered_map <string,string> &local_var){
	string argument(buffer);
	// cout<<argument<<endl;
	string pattern="$";
	string value;

	int i=0;
	for(i=0;i<(int)argument.size();++i){
		// cout<<argument[i]<<endl;
		// string temp=argument[i];
		if(argument[i]=='$')
			break;
	}
	// cout<<i<<endl;
	// if(i==(int)argument.size()){
	// 	return argument;
	// }
	string operand(argument,i+1,argument.size());
	string front(argument,0,i);

	if(environment_var.find(operand)!=environment_var.end())
		value=environment_var.find(operand)->second;
	else if(local_var.find(operand)!=local_var.end())
		value=local_var.find(operand)->second;
	else{
		value="";
		cout<<"Last"<<endl;
		return value;
	}

	// cout<<operand<<endl;
	value=front+value;

	// cout<<front<<endl;
	// cout<<"Value "<<value<<endl;
	return value;
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

void EchoExecute(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]){
	// cout<<"This is my echo"<<endl;
	string echo_command(buffer);

	int loc=echo_command.find(" ");
	if(echo_command[loc+1]=='('){
		string print(echo_command,loc+2,echo_command.size()-loc-3);
		cout<<print<<endl;
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='$')){
		cout<<"$$"<<endl;
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='?')){
		cout<<"$?"<<endl;
	}
	else{
		if(CheckContainsS(echo_command,"~")){
				loc=echo_command.find("~");
				// cout<<echo_command<<endl;
				echo_command.replace(loc,1,environment_var.find("HOME")->second);
				cout<<echo_command<<endl;
		}
		string print(echo_command,loc+1,echo_command.size()-loc-1);
		// print=ResolveD(buffer,environment_var,local_var);
		// cout<<print.size()<<endl;
		// cout<<print<<endl;
		// cout<<print<<endl;
		if(print.size()==0){
			cout<<"Variable not found"<<endl;
		}
		else{
			loc=echo_command.find(" ");
			// cout<<"Lower "<<loc<<endl;
			string print_resolved(print,loc+1,print_resolved.size()-loc);
			cout<<print_resolved<<endl;
		}
	}
}

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

void ExecuteKernel(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]){

	string command_check(buffer);
	command_check=CheckForAlias(command_check,alias_var);
	MakeCharArray(command_check,buffer);

	bool check_flag=break_command(buffer,environment_var,executable_var,commands);

	if(CheckContains(buffer,"=")){
			// cout<<"Yes"<<endl;
		string value;
		if(CheckContains(buffer,"$$")){
			cout<<"$$"<<endl;
		}
		else if(CheckContains(buffer,"$?")){
			cout<<"$?"<<endl;	
		}
		else if(CheckContains(buffer,"$")){
			value=ResolveD(buffer,environment_var,local_var);
			command_check=value;
		}
		else{}
			int loc;//=command_check.find("~");
		// cout<<command_check<<endl;
			if(CheckContainsS(command_check,"~")){
				loc=command_check.find("~");
				command_check.replace(loc,1,environment_var.find("HOME")->second);
			}
			loc=command_check.find("=");
			string part1(command_check,0,loc);
			string part2(command_check,loc+1,command_check.size());

			if(part1=="PS1"){
				new_environment_var.erase(part1);
				new_environment_var.insert(make_pair(part1,part2));
			}
			else{
				local_var.erase(part1);
				local_var.insert(make_pair(part1,part2));
			}
		// }
	}
	else if((buffer[0]=='c') && (buffer[1]=='d') && (buffer[2]==' ')){
		chdir(commands[1]);
	}
	else if((strcmp(commands[0],"addpath")==0)||(strcmp(commands[0],"ADDPATH")==0)){
		// cout<<"Inside"<<endl;
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
		EchoExecute(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
	}
	else if(!check_flag){
		printf("Invalid Command\n");
	}
	else{
		// cout<<"execute"<<endl;
		// cout<<commands[0]<<endl;
		execute(buffer,environment_var,executable_var,commands);
	}
}