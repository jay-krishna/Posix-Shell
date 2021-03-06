/*
	2019201019
*/
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <unordered_map>
#include <termios.h>
#include <stdio.h>
#include <queue>
#include <ctime>
#include <utility>
#include <sys/types.h>

#include "environmenthandler.h"
#include "helper.h"
#include "shellrunner.h"
#include "getinput.h"
#include "shellrunnerscript.h"

using namespace std;

char* commands[512];
unordered_map <string,string> environment_var;
unordered_map <string,string> executable_var;
unordered_map <string,string> alias_var;
unordered_map <string,string> new_environment_var;
unordered_map <string,string> new_alias_var;
unordered_map <string,string> local_var;
unordered_map<time_t,pid_t> alarmchilds;
priority_queue <time_t,vector<int>,greater<int>> allalarms;
unordered_map <time_t,string> alarmmessage;
vector <string> executable_var2;

void sigint_handler(int signum){
  //give warning that the signal has been disabled
  // wait for ENTER to be pressed before returning to the command line.
  printf("\nInterrupt Signal disabled.Press 'Enter' to continue.Use 'quit' command to exit.\n");
  fflush(stdout);
}

void CreateAlarmChild(string buffer_string,string display,unordered_map<time_t,pid_t> &alarmchilds){
	// string buffer_string(buffer);
	GetAlarmChild(buffer_string,display,alarmchilds);
	int loc1=buffer_string.find("(");
	int loc2=buffer_string.find(",");
	string message_string(buffer_string,loc1+1,(loc2-loc1-1));
	string time_string(buffer_string,loc2+1,buffer_string.size()-loc2-2);
	int timeduration=stoi(time_string);
	time_t unixtimenow=time(nullptr);
	time_t alarmtime=unixtimenow+timeduration;

	allalarms.push(alarmtime);
	alarmmessage[alarmtime]=message_string;
}

void CheckMissedAlarms(unordered_map <string,string> environment_var,string display,unordered_map<time_t,pid_t> &alarmchilds){
	string filename=environment_var.find("HOME")->second+"/"+".missedalarms"+"_.txt";
	ifstream infile(filename);
	string line;
	time_t unixtimenow=time(nullptr);

	while(getline(infile,line)){
		// cout<<line<<endl;
		int loc=line.find(" ");
		string front(line,0,loc);
		string back(line,loc+1,line.size()-loc-1);
		time_t prevalarmtime=(time_t)stoi(front);

		// cout<<"Read "<<prevalarmtime<<" "<<unixtimenow<<endl;
		
		if(prevalarmtime>=unixtimenow){
			string send="alarm("+back+","+to_string(prevalarmtime-unixtimenow)+")";
			// cout<<"Send "<<send<<endl;
			CreateAlarmChild(send,display,alarmchilds);
		}
		else{
			cout<<"Missed Alarm: "<<back<<endl;
		}
	}

	remove(filename.c_str());
}

int main(){
	// cout<<"Above"<<endl;
	signal(SIGINT, sigint_handler);
	bool flag=true;
	bool flag_alarm=true;
	bool export_flag=true;
	bool script_flag=false;

	FetchEnvironmentVariables(environment_var,executable_var,new_environment_var,new_alias_var,local_var);

	while(flag){
		// cout<<"while"<<getpid()<<" "<<getppid()<<endl;
		char buffer_before[4096];
		char buffer[4096];
		// cout<<"Above"<<endl;
		FetchBashrcVariables(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,export_flag,executable_var2);
		string display=PutPS1(environment_var);
		int i=0,j;
		char c;
		bool flag_inside=false,flag_repeat=false;

		if(flag_alarm){
			CheckMissedAlarms(environment_var,display,alarmchilds);
			flag_alarm=false;
		}

		struct termios initial_state=enable();
		sendinput(buffer_before,display,environment_var,executable_var2);
		disable(initial_state);

		// cout<<"From Input"<<buffer_before<<endl;

		for(j=0;j<(int)strlen(buffer_before);++j){
    		c=buffer_before[j];
    		// cout<<"Inside"<<endl;
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
  		}
		buffer[i]='\0';

		RecordHistory(buffer,environment_var);
		// cout<<buffer<<endl;
		if(strcmp(buffer,"exit")==0){
				int id=getpid();
				string filename=environment_var.find("HOME")->second+"/"+"."+to_string(id)+"_.txt";
				string filename2=environment_var.find("HOME")->second+"/"+".missedalarms"+"_.txt";
				remove(filename.c_str());
				ofstream outfile(filename2,ios::trunc);

				time_t unixtimenow=time(nullptr);

				while (!allalarms.empty()) 
			    { 
			    	if(allalarms.top()>=unixtimenow){
				        cout << allalarms.top()<<"\t";
				        // auto it=alarmmessage.find()
				        // cout<<alarmmessage[allalarms.top()]<<endl;
				        pid_t pid=alarmchilds.find(allalarms.top())->second;
				        kill(pid,SIGKILL);
				        string data=to_string(allalarms.top())+" "+alarmmessage[allalarms.top()];

				        outfile<<data<<endl;
			    	}
			    	allalarms.pop();
			    }

			    outfile.close();

				flag=false;
		}
		else if((strcmp(buffer,"record start")==0)||(strcmp(buffer,"record stop")==0))
		{
			// cout<<"Record"<<endl;
			if(strcmp(buffer,"record start")==0){
				script_flag=true;
				string filename2=environment_var.find("HOME")->second+"/"+"scriptoutput.txt";
				ofstream outfile(filename2,ios::out|ios::trunc);
				outfile.close();
			}
			else{
				script_flag=false;
			}
		}
		else if(strlen(buffer)==0){
			continue;
		}
		else if((buffer[0]=='.')&&(buffer[1]=='/')){
			ExecuteScript(buffer,environment_var,commands,local_var);
		}
		else if((buffer[0]=='a')&&(buffer[1]='l')&&(buffer[2]=='a')&&(buffer[3]=='r')&&(buffer[4]=='m')){
			CreateAlarmChild(buffer,display,alarmchilds);
		}
		else if((buffer[0]=='r')&&(buffer[1]='u')&&(buffer[2]=='n')){
			// CreateAlarmChild(buffer,display,alarmchilds);
			cout<<"open command"<<endl;
			string s(buffer);
			int loc=s.find(".");
			int loc1=s.find(" ");
			string ex(s,loc+1,s.size()-loc-1);
			string name(s,loc1+1,loc-loc1-1);
			// cout<<ex;
			string filename3=environment_var.find("HOME")->second+"/"+".myrc";
			ifstream file(filename3);
			string line;
			string app;
			getline(file,line);
			// cout<<line<<endl;
			while(getline(file,line)){
				int loc2=line.find(" ");
				string front(line,0,loc2);
				string back(line,loc2+1,line.size()-loc2-1);
				// cout<<front<<" "<<back<<endl;
				if(front==ex){
					app=back;
					break;
				}
			}


			// loc=s.find(" ");
			// string temp(s,loc+1,s.size()-loc-1);
			// // cout<<temp<<endl;
			// // char char_array[n + 1];
			string temp=app+" "+name+"."+ex;
			strcpy(buffer, temp.c_str());
			buffer[temp.size()]='\0';
			cout<<buffer<<endl;
			// // commands[0]=
			ExecuteKernel(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);

		}
		else if(CheckContains(buffer,"|")){
			// cout<<"pipe"<<endl;
			int count=0;
			const char *delim2="|";
			char *token = strtok(const_cast<char*>(buffer), delim2);
			while(token!=nullptr){
				ExecuteKernelP(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,token,count);
				token = strtok(nullptr, delim2);
				++count;
			}
			--count;
			if(CheckContains(buffer,">") || CheckContains(buffer,">>")){

			}
			else if(count%2){
				string filename1="odd.txt";
				ifstream file(filename1);
				string line;
				while(getline(file,line))
					cout<<line<<endl;
				file.close();
			}
			else{
				string filename1="even.txt";
				ifstream file(filename1);
				string line;
				while(getline(file,line))
					cout<<line<<endl;
				file.close();
			}
			remove("odd.txt");
			remove("even.txt");
		}
		else if(!script_flag){
			ExecuteKernel(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
		}
		else{
			// cout<<"script command"<<endl;
			ExecuteKernelS(environment_var,executable_var,alias_var,new_environment_var,new_alias_var,local_var,commands,buffer);
		}
		fflush(stdin);
		memset(buffer, 0, strlen(buffer));
		for(int i=0;commands[i];++i){
			commands[i]=NULL;
		}
	}
	// kill(2);
	return 0;
}