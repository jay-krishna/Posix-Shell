#include "shellrunnerscript.h"

string ResolveDS(char * buffer,unordered_map <string,string> &environment_var,unordered_map <string,string> &local_var){
	string argument(buffer);
	// cout<<argument<<endl;
	string pattern="$";
	string value;
	// cout<<"Called"<<endl;
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
	string front(argument,0,i);
	if(argument[i+1]=='$'){
		// cout<<"Inside"<<endl;
		auto id=(int)getpid();
		value=to_string(id);
	}
	else if(argument[i+1]=='?'){}
	else{
		string operand(argument,i+1,argument.size());

		if(environment_var.find(operand)!=environment_var.end())
			value=environment_var.find(operand)->second;
		else if(local_var.find(operand)!=local_var.end())
			value=local_var.find(operand)->second;
		else{
			value="";
			// cout<<"Last"<<endl;
			return value;
		}
	}

	// cout<<operand<<endl;
	value=front+value;

	// cout<<front<<endl;
	// cout<<"Value "<<value<<endl;
	return value;
}

void executeS(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);
		// int errorlo;
		// wait(&errorlo);
		// if(	WIFEXITED(errorlo)){
		// 	auto error=WEXITSTATUS(errorlo);
		// 	environment_var.erase("Error");
		// 	environment_var.insert(make_pair("Error",to_string(error)));
		// }
		cout<<"YES"<<endl;
		string filename1=environment_var.find("HOME")->second+"/"+"scriptoutput.txt";
		string filename2=environment_var.find("HOME")->second+"/"+"scriptoutput_temp.txt";

		fstream file1(filename1,ios::in|ios::out|ios::app);
		fstream file2(filename2,ios::in);
		// cout<<string(buffer)<<endl;
		file1<<string(buffer)<<endl;
		string line;

		while(getline(file2,line)){
			file1<<line<<endl;
		}
		file1.close();
		file2.close();

		remove(filename2.c_str());

	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	// cout<<commands[0]

    	string filename=environment_var.find("HOME")->second+"/"+"scriptoutput_temp.txt";
		int fd=open(filename.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    	dup2(fd,1);



		auto e = execve(commands[0],commands,env_array);
		cout<<e<<endl;
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

void WriteOutput(string one, string two,unordered_map<string,string>environment_var){
	string filename1=environment_var.find("HOME")->second+"/"+"scriptoutput.txt";

	fstream file1(filename1,ios::in|ios::out|ios::app);
	// cout<<string(buffer)<<endl;
	file1<<one<<endl;
	file1<<two<<endl;
	file1.close();
}

void EchoExecuteS(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]){
	// cout<<"This is my echo"<<endl;
	string echo_command(buffer);

	int loc=echo_command.find(" ");
	if(echo_command[loc+1]=='('){
		string print(echo_command,loc+2,echo_command.size()-loc-3);
		cout<<print<<endl;
		WriteOutput(echo_command,print,environment_var);
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='$')){
		cout<<getpid()<<endl;
		WriteOutput(echo_command,to_string((int)getpid()),environment_var);
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='?')){
		cout<<environment_var["Error"]<<endl;
	}
	else if(echo_command[loc+1]=='$'){
		// string print(echo_command,loc+1,echo_command.size()-loc-1);
		string print=ResolveDS(buffer,environment_var,local_var);
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
			WriteOutput(echo_command,print_resolved,environment_var);
		}
	}
	else{
		if(CheckContainsS(echo_command,"~")){
				loc=echo_command.find("~");
				// cout<<echo_command<<endl;
				echo_command.replace(loc,1,environment_var.find("HOME")->second);
		}

		string print(echo_command,loc+1,echo_command.size()-loc-1);
		cout<<print<<endl;
		WriteOutput(echo_command,print,environment_var);
	}
}

void ExecuteKernelS(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]){

	string command_check(buffer);
	command_check=CheckForAlias(command_check,alias_var);
	MakeCharArray(command_check,buffer);

	bool check_flag=break_command(buffer,environment_var,executable_var,commands);

	if(CheckContains(buffer,"=")){
			// cout<<"Yes"<<endl;
		string value;
		if(CheckContains(buffer,"$$")){
			// cout<<"$$"<<endl;
			value=ResolveDS(buffer,environment_var,local_var);
			command_check=value;
			// cout<<command_check<<endl;
		}
		else if(CheckContains(buffer,"$?")){
			string cmd=string(buffer);
			int loc=cmd.find("$");
			string front(cmd,0,loc);
			string command_check=front+environment_var.find("Error")->second;
			// cout<<command_check<<endl;
		}
		else if(CheckContains(buffer,"$")){
			value=ResolveDS(buffer,environment_var,local_var);
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
			else if(part1=="HOME"){
				environment_var.erase(part1);
				environment_var.insert(make_pair(part1,part2));
			}
			// else if(part1=="PATH"){
			// 	new_environment_var.erase(part1);
			// 	new_environment_var.insert(make_pair(part1,part2));
			// }
			// else if(part1=="PS1"){
			// 	new_environment_var.erase(part1);
			// 	new_environment_var.insert(make_pair(part1,part2));
			// }
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
	else if((strcmp(commands[0],"history")==0)){
		string filename2=environment_var.find("HOME")->second+"/"+"my_history.txt";
		string line;
	    ifstream infile1(filename2);
	    while (getline(infile1,line)){
	        cout<<line<<endl;
	    }
	}
	else if((buffer[0]=='e')&&(buffer[1]=='c')&&(buffer[2]=='h')&&(buffer[3]=='o')&&(buffer[4]==' ')){
		// cout<<"three"<<endl;
		// commands[0]=(char*)"echo";
		// cout<<commands[0]<<endl;
		// cout<<commands[1]<<endl;
		// cout<<commands[2]<<endl;
		EchoExecuteS(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
	}
	else if(strcmp(commands[0],"export")==0){
		// cout<<"export"<<endl;
		// cout<<commands[1]<<endl;
		// string export_command(commands[0]);
		// cout<<"export"<<getpid()<<" "<<getppid()<<endl;
		string front(commands[1]);
		string value;
		string data;
		int id=(int)getpid();
		string filename=environment_var.find("HOME")->second+"/"+"."+to_string(id)+"_.txt";
		cout<<filename<<endl;
		if(front=="PATH"){
			cout<<"path"<<endl;
			value=environment_var.find("PATH")->second;
			cout<<"Value "<<value<<endl;
		}
		else if(front=="HOME"){
			// cout<<"path"<<endl;
			value=environment_var.find("HOME")->second;
			// cout<<"Value "<<value<<endl;
		}
		else if(front=="PS1"){
			// cout<<"path"<<endl;
			value=environment_var.find("PS1")->second;
			// cout<<"Value "<<value<<endl;
		}
		else{
			// cout<<"other"<<endl;
			value=local_var.find(front)->second;
		}
		data=front+"#"+value;
		cout<<data<<endl;
		fstream outfile(filename,ios::out|ios::app);
		outfile<<data<<endl;
		outfile.close();

		// int loc=export_command.find(" ");
		// string var(export_command,loc+1,export_command.size()-loc-1);

		cout<<data<<endl;
	}
	else if(!check_flag){
		printf("Invalid Command\n");
	}
	else{
		// cout<<"execute"<<endl;
		// cout<<commands[0]<<endl;
		// string filename="";
		// if(CheckContains(buffer,">")){
		// 	int z;
		// 	for(z=0;commands[z];++z);
		// 	filename=commands[z-1];
		// }
			executeS(buffer,environment_var,executable_var,commands);
	}
}

void executeP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count){
	auto pid=fork();
	// int status;
	if(pid>0){
		// wait(NULL);
		int errorlo;
		wait(&errorlo);
		if(	WIFEXITED(errorlo)){
			auto error=WEXITSTATUS(errorlo);
			environment_var.erase("Error");
			environment_var.insert(make_pair("Error",to_string(error)));
			// environment_var["Error"]=error;
			// cout<<"Error"<<error<<endl;
			// cout<<"Yes"<<endl;
			// cout<<environment_var.find("Error")->second<<endl;
			// cout<<"No"<<endl;
		}

	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	cout<<commands[0];

    	int fd0,fd1;
    	string filename1="even.txt";
    	string filename2="odd.txt";

    	if(count==0){
    		fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		dup2(fd1,1);
    	}
    	else if(count%2==0){
    		fd0=open(filename2.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		dup2(fd1,1);
    	}
    	else{
    		fd0=open(filename1.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		fd1=open(filename2.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		dup2(fd1,1);
    	}
    	// else{
    	// 	fd0=open(filename2.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    	// 	dup2(fd0,0);
    	// 	fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    	// 	dup2(fd1,1);
    	// }
    	// cout<<count<<endl;
    	// cout<<commands[0]<<endl;
		auto e = execve(commands[0],commands,env_array);
		cout<<e<<endl;
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

void executeRedAP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);

	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	
    	int z=0;
    	for(;commands[z]!=NULL;++z){}
    	string filename=commands[z-1];
    	// cout<<filename<<endl;
    	commands[z-2]=NULL;
    	z=0;
    	// for(;commands[z]!=NULL;++z){
    	//     		cout<<commands[z]<<endl;}
    	int fd0,fd1;
    	string filename1="even.txt";
    	string filename2="odd.txt";

    	// if(count==0){
    	// 	fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    	// 	dup2(fd1,1);
    	// }
    	if(count%2==0){
    		fd0=open(filename2.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		// fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		// dup2(fd1,1);
    	}
    	else{
    		fd0=open(filename1.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		// fd1=open(filename2.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		// dup2(fd1,1);
    	}

    	int fd=open(filename.c_str(),O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
    	dup2(fd,1);


		auto e = execve(commands[0],commands,env_array);
		cout<<e<<endl;
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

void executeRedP(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[],int count){
	auto pid=fork();
	// int status;
	if(pid>0){
		wait(NULL);

	}
	else if(pid==0){
		// auto itr=
		string pathpass="PATH="+environment_var.find("PATH")->second;
    	char* env_array[3]={(char*)pathpass.c_str(),(char*)"TERM=xterm-256color",NULL};
    	
    	int z=0;
    	for(;commands[z]!=NULL;++z){}
    	string filename=commands[z-1];
    	// cout<<filename<<endl;
    	commands[z-2]=NULL;
    	z=0;
    	// for(;commands[z]!=NULL;++z){
    	//     		cout<<commands[z]<<endl;}

    	int fd0,fd1;
    	string filename1="even.txt";
    	string filename2="odd.txt";

    	// if(count==0){
    	// 	fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    	// 	dup2(fd1,1);
    	// }
    	if(count%2==0){
    		fd0=open(filename2.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		// fd1=open(filename1.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		// dup2(fd1,1);
    	}
    	else{
    		fd0=open(filename1.c_str(),O_CREAT|O_RDONLY,S_IRUSR|S_IWUSR);
    		dup2(fd0,0);
    		// fd1=open(filename2.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    		// dup2(fd1,1);
    	}

    	// int fd=open(filename.c_str(),O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
    	// dup2(fd,1);

    	int fd=open(filename.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);
    	dup2(fd,1);


		auto e = execve(commands[0],commands,env_array);
		cout<<e<<endl;
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

void ExecuteKernelP(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[],int count){

	string command_check(buffer);
	command_check=CheckForAlias(command_check,alias_var);
	MakeCharArray(command_check,buffer);

	bool check_flag=break_command(buffer,environment_var,executable_var,commands);
	// cout<<commands[0]<<endl;
	// cout<<commands[1]<<endl;
	if(!check_flag){
		printf("Invalid Command\n");
	}
	// else if(strcmp(commands[0],"openapp")==0){
	// 	cout<<"OPne"<<endl;
	// }
	else{
		if(CheckContains(buffer,">>")){
			// cout<<"no"<<endl;
			executeRedAP(buffer,environment_var,executable_var,commands,count);	
		}
		else if(CheckContains(buffer,">")){
			// cout<<"yes"<<endl;
			executeRedP(buffer,environment_var,executable_var,commands,count);	
		}
		else{
					executeP(buffer,environment_var,executable_var,commands,count);}
	}
}