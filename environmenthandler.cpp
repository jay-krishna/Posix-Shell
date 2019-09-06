#include "environmenthandler.h"

vector<string> CheckProfileFile(unordered_map <string,string> environment_var){
	// cout<<"Called"<<endl;
	string filepath=environment_var.find("HOME")->second+"/.my_profile";
	ifstream infile(filepath);
	if(infile.fail()){
		// cout<<"Inside"<<endl;
		infile.close();
		ofstream outfile(filepath,ios::out);
		outfile<<"PATH:/etc/environment"<<endl;
		outfile<<"HOME:/etc/passwd"<<endl;
		outfile.close();
	}
	else{
		infile.close();
	}

	infile.open(filepath);
	const char *delim=":";
	string value;
	// int count;

	vector<string> v;
	while(getline(infile,value))
    {
    	char *token = strtok(const_cast<char*>(value.c_str()), delim);
    	token = strtok(nullptr, delim);
    	v.push_back(string(token));
    }

    infile.close();

    return v;
}

void CheckBashrcFile(unordered_map <string,string> &environment_var,const char *env_var[],unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var){
	// cout<<"Called"<<endl;
	string filepath=environment_var.find("HOME")->second+"/.my_bashrc";
	ifstream infile(filepath);
	// cout<<filepath<<endl;
	if(infile.fail()){
		// cout<<"Inside"<<endl;
		infile.close();
		ofstream outfile(filepath,ios::out);
		outfile<<"PS1:>"<<endl;
		outfile.close();
	}
	else{
		infile.close();
	}

	infile.open(filepath);
	const char *delim=":";
	string value;
	string myprofile_path=environment_var.find("PATH")->second;
	// int count;

	// vector<string> v;

	while(getline(infile,value))
    {
    	char *token = strtok(const_cast<char*>(value.c_str()), delim);
    	string pth="PATH";
    	string ps="PS1";
    	string alis="ALIAS";

    	string loop_string(token);
    	if(loop_string==pth){
    		token = strtok(nullptr, delim);
    		myprofile_path+=":"+string(token);
    		environment_var.erase("PATH");
    		// char *send=myprofile_path.c_str();
    		char cstr[myprofile_path.size() + 1];
			strcpy(cstr, myprofile_path.c_str());

    		make_path(env_var[0],cstr,environment_var);
    	}
    	else if(loop_string==ps){
    		token = strtok(nullptr, delim);
    		make_path(env_var[4],token,environment_var);
    	}
    	else if(loop_string==alis){
    		token = strtok(nullptr, delim);
    		char* save_token=token;
    		token = strtok(nullptr, delim);
    		make_path(save_token,token,alias_var);
    	}
    	else
    		continue;
    	// v.push_back(string(token));
    }

    unordered_map<string,string>::iterator it1;
    it1=new_environment_var.find("PATH");
    unordered_map<string,string>::iterator it2;
    it2=environment_var.find("PATH");

    // cout<<new_environment_var.size()<<endl;
    if(it1!=new_environment_var.end()){
    	// cout<<"Inside"<<endl;
    	value=it1->second+":"+it2->second;
    	environment_var.erase("PATH");
    	char cstr1[value.size() + 1];
		strcpy(cstr1,value.c_str());
    	make_path(env_var[0],cstr1,environment_var);
    }
    // else
    	// cout<<"Outside";

    it1=new_environment_var.find("PS1");
    // unordered_map<string,string>::iterator it2;
    // it2=environment_var.find("PS1");

    if(it1!=new_environment_var.end()){
    	// cout<<"Inside"<<endl;
    	value=it1->second;
    	environment_var.erase("PS1");
    	char cstr1[value.size() + 1];
		strcpy(cstr1,value.c_str());
    	make_path(env_var[4],cstr1,environment_var);
    	// cout<<"Diff "<<cstr1<<endl;
    }

    for(auto it3=new_alias_var.begin();it3!=new_alias_var.end();++it3){
    	string a=it3->first;
    	string b=it3->second;
    	char cstr[a.size() + 1];
		strcpy(cstr,a.c_str());
		char cstr1[b.size() + 1];
		strcpy(cstr1,b.c_str());
		make_path(cstr,cstr1,alias_var);
    }

    infile.close();
}

string PutPS1(unordered_map <string,string> &environment_var){

  	char buffer[2048];
  	string display="";
  	string display_save;
  	bool flag=false;
  	uid_t uid=geteuid();

  	if(uid==0)
  		flag=true;
  	if(environment_var.find("PS1")->second=="DEFAULT"){
	  	string display_user=environment_var.find("USER")->second;
	  	display_user+="@";

	  	string display_host=environment_var.find("HOSTNAME")->second;
	  	display_host+=": ";

	  	getcwd(buffer,FILENAME_MAX);
	  	// cout<<"0 "<<buffer<<endl;
	  	string display_cwd(buffer);
	  	display_save=display_user+display_host+display_cwd;
	  	if(flag)
	  		display_cwd+="# ";
	  	else
	  		display_cwd+="$ ";

	  	// cout<<"1 "<<display_user<<endl;
	  	// cout<<"2 "<<display_user<<endl;
	  	// cout<<"3 "<<display_cwd<<endl;
	  	display+=display_user+display_host+display_cwd;
	  }
	else{
		display=environment_var.find("PS1")->second;
		display_save=display;
		if(flag)
	  		display+="# ";
	  	else
	  		display+="$ ";
	}
  	// printf("%s",display.c_str());
  	// cout<<display;

  	// for (auto x : environment_var) 
   //    cout << x.first << " " << x.second << endl;

  	environment_var.erase("PS1");
  	// // str
  	// cout<<"This is display "<<display<<endl;

  // for (auto x : environment_var) 
  //     cout << x.first << " " << x.second << endl;


  	environment_var.insert(make_pair("PS1",display_save));

  	return display;

}

void AddPath(char* commands[],unordered_map <string,string> &new_environment_var){
	// Format addpath <path link>
	// cout<<"Called"<<endl;
	auto it=new_environment_var.find("PATH");
	const char* pth="PATH";
	if(it!=new_environment_var.end()){
		// cout<<"CheckPoint 1"<<endl;
		string new_path=it->second+':'+commands[1];
		new_environment_var.erase(string(pth));
		char cstr[new_path.size() + 1];
		strcpy(cstr, new_path.c_str());
		make_path(pth,cstr,new_environment_var);
		// new_environment_var.insert(new_path);
	}
	else{
		// cout<<"Else"<<endl;
		// cout<<commands[1]<<endl;
		make_path(pth,commands[1],new_environment_var);
	}

	// for (auto x : new_environment_var) 
 //      cout << x.first << " " << x.second << endl;
}

void AddAlias(char* commands[],unordered_map <string,string> &new_alias_var){
	// Format alias <alias name> <actual command>
	string alias_body="";
	// const char* als="ALIAS";

	for(int i=2;commands[i]!=NULL;++i)
		alias_body+=string(commands[i])+" ";

	alias_body.pop_back();
	// cout<<commands[1]<<" "<<alias_body<<endl;

	char cstr[alias_body.size() + 1];
	strcpy(cstr, alias_body.c_str());
	make_path(commands[1],cstr,new_alias_var);
}

void traverse(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var){
	// environment_var.clear();
	executable_var.clear();

	string temp=environment_var.find("PATH")->second;
	// cout<<temp<<endl;
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

void FetchHome(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[]){
	char *env_val;
	uid_t uid;
	string line;
	const char *delim=":";

	uid = geteuid();
  	ifstream infile("/etc/passwd");
  	string pattrn="(.*)(:"+to_string(uid)+":)(.*)";;
  	regex r1(pattrn);
  	int count=1;
    while (getline(infile, line))
	{
		if(regex_match(line,r1)){
			char *token = strtok(const_cast<char*>(line.c_str()), delim);
			while (token != nullptr)
			{
				if(count==6){
					env_val=token;
					make_path(env_var[1],env_val,environment_var);
				}
				++count;
				token = strtok(nullptr, delim);
			}
		}
	}

	infile.close();
}

void FetchUserHostname(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[],vector<string>locations){

	char *env_val[5];
	// struct passwd *pw;
  	uid_t uid;
  	const char *delim=":";
  	string line;

	uid = geteuid();
  	ifstream infile(locations[1]);
  	string pattrn="(.*)(:"+to_string(uid)+":)(.*)";;
  	regex r1(pattrn);
  	int count=1;
    while (getline(infile, line))
	{
		if(regex_match(line,r1)){
			char *token = strtok(const_cast<char*>(line.c_str()), delim);
			while (token != nullptr)
			{
				if(count==1){
					env_val[2]=token;
					make_path(env_var[2],env_val[2],environment_var);
					break;
				}
				++count;
				token = strtok(nullptr, delim);
			}
		}
	}

	char buffer[128];
	gethostname(buffer,HOST_NAME_MAX);
	make_path(env_var[3],buffer,environment_var);

}

void FetchPath(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,const char *env_var[],vector<string>locations){
	char *env_val;
	
	ifstream infile(locations[0]);
	string line;
	getline(infile,line);

	regex r("\"(.*?)\"");
	smatch m;
	regex_search(line, m, r);
	string s(m[1]);
	env_val=(char*)s.c_str();
	make_path(env_var[0],env_val,environment_var);

	infile.close();
}

void FetchEnvironmentVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var){

	environment_var.clear();
	executable_var.clear();
	new_environment_var.clear();
	new_alias_var.clear();
	local_var.clear();

	const char *env_var[5] = {"PATH","HOME","USER","HOSTNAME","PS1"};
	// cout<<"Fetch"<<endl;
	FetchHome(environment_var,executable_var,env_var);
	vector<string> locations=CheckProfileFile(environment_var);
	FetchPath(environment_var,executable_var,env_var,locations);
	FetchUserHostname(environment_var,executable_var,env_var,locations);

	// for (auto x : environment_var) 
 //      cout << x.first << " " << x.second << endl;
}

void FetchBashrcVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var,unordered_map <string,string> &alias_var,unordered_map <string,string> &new_environment_var,unordered_map <string,string> &new_alias_var,unordered_map <string,string> &local_var){
	const char *env_var[7] = {"PATH","HOME","USER","HOSTNAME","PS1","ALIAS","PS1_val"};
	alias_var.clear();
	// for (auto x : new_environment_var) 
 //      cout << x.first << " " << x.second << endl;
	CheckBashrcFile(environment_var,env_var,alias_var,new_environment_var,new_alias_var);

	// for (auto x : environment_var) 
 //      cout << x.first << " " << x.second << endl;
 //  cout<<"%%%%%%%%\n";
  	for (auto x : local_var) 
      cout << x.first << " " << x.second << endl;

	traverse(environment_var,executable_var);
}