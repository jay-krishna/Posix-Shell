/*
	2019201019
	Helper Module
*/
#include "helper.h"

void make_path(const char* var, char* val,unordered_map <string,string> &environment_var){
	string vr(var);
	string temp(val);
	environment_var.insert(make_pair(vr,temp));

}

void MakeCharArray(string command_check,char* buff){
	int i;
	for(i=0;i<(int)command_check.size();++i)
		buff[i]=command_check[i];

	buff[i]='\0';

	// return buffer;
}

bool CheckContains(char* buff,string pattern){
	size_t loc = string(buff).find(pattern); 
    if (loc!=string::npos)
    	return true;
    else
    	return false;
}

bool CheckContainsS(string buff,string pattern){
	size_t loc = string(buff).find(pattern); 
    if (loc!=string::npos)
    	return true;
    else
    	return false;
}

string CheckForAlias(string command,unordered_map <string,string> alias_var){
	stringstream command_object(command);
	string word;
	string new_command;
	int count=0;

	while(command_object>>word){
		if(count==0){
			auto itr=alias_var.find(word);
			if(itr!=alias_var.end()){
				new_command=itr->second;
			}
			else
				new_command=word;
		}
		else{
			new_command+=" "+word;
		}
		count=1;
	}



	// char buffer_temp[4096];
	// string store=it->second;
	// for(int j=0;j<store.size();++j)
	// 	buffer_temp[j]=store[j];
	// for(int j=i;j<strlen(buffer);++j)
	// 	buffer_temp[j]=buffer[j];
	// strncpy(buffer, buffer_temp, strlen(buffer_temp));

	return new_command;
}

bool break_command(char buffer[],unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var, char* commands[]){

	string temp="";
	int count=0,cn=0;
	char del=' ';
	char til='~';
	string display_user=environment_var.find("HOME")->second;
	bool flag_valid=true;

	// for (auto x : alias_var) 
 //      cout << x.first << " " << x.second << endl;

  	// if(alias_var.find("ll")==alias_var.end())
  	// 	cout<<"YES"<<endl;
  	// else
  	// 	cout<<"******"<<endl;
  	// else
  	// 	cout<"YAA"<<endl;
	
	for (unsigned int i = 0; i<strlen(buffer); ++i)
	{
		if(buffer[i]==del){
			if(cn==0){
			// 	auto itr=alias_var.find(temp);
			// if(itr!=alias_var.end()){
			// 	check_alias(buffer,alias_var,itr,i);
			// }
			// 				temp=alias_var.find(temp)->second;}

				auto it=executable_var.find(temp);
				if(it!=executable_var.end())
					temp=executable_var.find(temp)->second;
				else{
					// temp=t;
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
		// auto itr=alias_var.find(temp);
		// if(itr!=alias_var.end())
		// 	temp=alias_var.find(temp)->second;
		auto it=executable_var.find(temp);
		if(it!=executable_var.end())
			temp=executable_var.find(temp)->second;
		else
			flag_valid=false;
		++cn;
	}
	commands[count++]=(char*)malloc(strlen(temp.c_str())+1);
	strcpy(commands[count-1],temp.c_str());
	commands[count]=NULL;

	// cout<<count<<endl;
	// cout<<commands[1]<<endl;

	return flag_valid;

}

void RecordHistory(char buffer[],unordered_map<string,string> environment_var){
	string filename1=environment_var.find("HOME")->second+"/"+".myrc";
	string filename2=environment_var.find("HOME")->second+"/"+"my_history.txt";

	ifstream infile1(filename1);
	if(infile1.fail()){
		infile1.close();
		ofstream outfile1(filename1);
		outfile1<<"HISTLEN#50"<<endl;
		outfile1.close();
		infile1.open(filename1);
	}
	string line;
	getline(infile1,line);
	int loc=line.find("#");
	string h(line,loc+1,line.size()-loc-1);
	infile1.close();
	int hlen=stoi(h);

	fstream outfile2(filename2,ios::out|ios::in);
	int count=0;
	string data="";
	while (getline(outfile2,line)){
		// cout<<line<<endl;
		if(count>hlen){
			break;
		}
		data+=line;
		data+="\n";
		++count;
		// cout<<data<<endl;
	}
	if(count==hlen){
		int loc=data.find("\n");
		string data2(data,loc+1,data.size()-loc-1);
		data=data2;
	}


	data+=string(buffer);
	outfile2.close();
	outfile2.open(filename2,ios::out|ios::trunc);
	outfile2<<data<<endl;
	outfile2.close();
}