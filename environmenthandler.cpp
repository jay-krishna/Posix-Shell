#include "environmenthandler.h"

void PutPS1(unordered_map <string,string> environment_var){

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

void traverse(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var){
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

void FetchEnvironmentVariables(unordered_map <string,string> &environment_var,unordered_map <string,string> &executable_var){
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
	make_path(env_var[0],env_val[0],environment_var);
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
					make_path(env_var[2],env_val[2],environment_var);
				}
				if(count==6){
					env_val[1]=token;
					make_path(env_var[1],env_val[1],environment_var);
				}
				++count;
				token = strtok(nullptr, delim);
			}
		}
	}


  	pw = getpwuid(uid);
  	env_val[2] = pw->pw_name;
	make_path(env_var[2],env_val[2],environment_var);

	char buffer[128];
	gethostname(buffer,HOST_NAME_MAX);
	make_path(env_var[3],buffer,environment_var);

	// for (auto x : environment_var) 
 //      cout << x.first << " " << x.second << endl;

	traverse(environment_var,executable_var);
}