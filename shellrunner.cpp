#include "shellrunner.h"

string ResolveD(char * buffer,unordered_map <string,string> &environment_var,unordered_map <string,string> &local_var){
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

void GetAlarmChild(string buffer_string,string display,unordered_map<time_t,pid_t> &alarmchilds){
	// string buffer_string(buffer);
	int loc1=buffer_string.find("(");
	int loc2=buffer_string.find(",");
	// cout<<loc<<endl;
	string message_string(buffer_string,loc1+1,(loc2-loc1-1));
	// cout<<message_string<<endl;
	string time_string(buffer_string,loc2+1,buffer_string.size()-loc2-2);
	// cout<<time_string<<endl;
	int timeduration=stoi(time_string);
	time_t unixtimenow=time(nullptr);
	time_t alarmtime=unixtimenow+timeduration;
	cout<<unixtimenow<<endl;
	cout<<alarmtime<<endl;

	auto pid=fork();
	if(pid>0){
		alarmchilds[alarmtime]=pid;
	}
	if(pid==0){
		bool flag=true;

		while(flag){
			time_t unixtimecurrent=time(nullptr);
			if(unixtimecurrent>=alarmtime){
				cout<<"Alarm: "<<message_string<<endl;
				flag=false;
			}
		}
		cout<<display;
		exit(1);
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
		cout<<e<<endl;
		if (e == -1)
			fprintf(stderr, "Error: %s\n", strerror(errno));

		exit(1);
	}
	else{
		printf("Fork Error\n");
	}
}

void executeRedA(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){
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

void executeRed(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){
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

    	int fd=open(filename.c_str(),O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
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

void EchoExecute(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var,char* commands[],char buffer[]){
	// cout<<"This is my echo"<<endl;
	string echo_command(buffer);

	int loc=echo_command.find(" ");
	if(echo_command[loc+1]=='('){
		string print(echo_command,loc+2,echo_command.size()-loc-3);
		cout<<print<<endl;
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='$')){
		cout<<getpid()<<endl;
	}
	else if((echo_command[loc+1]=='$')&&(echo_command[loc+2]=='?')){
		cout<<"$?"<<endl;
	}
	else if(echo_command[loc+1]=='$'){
		// string print(echo_command,loc+1,echo_command.size()-loc-1);
		string print=ResolveD(buffer,environment_var,local_var);
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
	else{
		if(CheckContainsS(echo_command,"~")){
				loc=echo_command.find("~");
				// cout<<echo_command<<endl;
				echo_command.replace(loc,1,environment_var.find("HOME")->second);
		}

		string print(echo_command,loc+1,echo_command.size()-loc-1);
		cout<<print<<endl;
	}
}

void ExecuteScript(char buffer[],unordered_map <string,string> &environment_var,char* commands[],unordered_map <string,string> &local_var){

	int id=getppid();
	string filename=environment_var.find("HOME")->second+"/"+"."+to_string(id)+"_.txt";
	ifstream infile(filename);
	int id2=getpid();
	string filename2=environment_var.find("HOME")->second+"/"+"."+to_string(id2)+"_.txt";
	ofstream outfile(filename2,ios::out|ios::app);
	string line;
	const char *delim="#";

	while (getline(infile, line))
	{
		char* token = strtok(const_cast<char*>(line.c_str()), delim);
		string front(token);
		// token = strtok(nullptr, delim);
		string value;//(token);

		// cout<<front<<" "<<value<<endl;
		if(front=="PATH"){
			value=environment_var.find(front)->second;
			// cout<<"Value export "<<value<<endl;
		}
		// if(front=="PS1"){
		// 	value=environment_var.find(front)->second;
		// 	// cout<<"Value export "<<value<<endl;
		// }
		// if(front=="HOME"){
		// 	value=environment_var.find(front)->second;
		// 	// cout<<"Value export "<<value<<endl;
		// }
		else{
			value=local_var.find(front)->second;
		}

		string data=front+"#"+value;
		outfile<<data<<endl;
	}


	infile.close();
	outfile.close();

	infile.open(filename2);
	// outfile.open('temp.txt',ios::out|ios::trun);
	string global_data="";

	while (getline(infile, line))
	{
		char* token = strtok(const_cast<char*>(line.c_str()), delim);
		string front(token);
		// token = strtok(nullptr, delim);
		string value;//(token);

		// cout<<front<<" "<<value<<endl;
		if(front=="PATH"){
			value=environment_var.find(front)->second;
			// cout<<"Value export "<<value<<endl;
		}
		// if(front=="PS1"){
		// 	value=environment_var.find(front)->second;
		// 	// cout<<"Value export "<<value<<endl;
		// }
		// if(front=="HOME"){
		// 	value=environment_var.find(front)->second;
		// 	// cout<<"Value export "<<value<<endl;
		// }
		else{
			value=local_var.find(front)->second;
		}

		string data=front+"#"+value;
		global_data+=data+'\n';
	}
	infile.close();
	outfile.open(filename2,ios::trunc);
	outfile<<global_data;
	outfile.close();

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
			// cout<<"$$"<<endl;
			value=ResolveD(buffer,environment_var,local_var);
			command_check=value;
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
		EchoExecute(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
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
		if(CheckContains(buffer,">>")){
			// cout<<"no"<<endl;
			executeRedA(buffer,environment_var,executable_var,commands);	
		}
		else if(CheckContains(buffer,">")){
			// cout<<"yes"<<endl;
			executeRed(buffer,environment_var,executable_var,commands);	
		}
		else
			execute(buffer,environment_var,executable_var,commands);
	}
}