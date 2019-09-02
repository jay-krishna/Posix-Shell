#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <regex>

using namespace std;

int main(){

	ifstream infile("/etc/environment");
	string line;
	getline(infile,line);
	// cout<<line<<endl;
	const char *delim=":";

	regex r("\"(.*?)\"");
	smatch m; 
  
    // regex_search() for searching the regex pattern 
    // 'r' in the string 's'. 'm' is flag for determining 
    // matching behavior. 
    regex_search(line, m, r);
    cout<<m[1]<<endl;
    // cout<<"*****"<<endl;
    // for (auto x : m) 
    //     cout << "123  "<<x << endl;//" "; 
    // cout<<"+++++"<<endl;
	// char *token = strtok(const_cast<char*>(line.c_str()), "=");
	// token = strtok(nullptr,delim);
	// token = strtok(nullptr,delim);
	// cout<<token<<endl;

	// line.erase(0,6);
	// line.erase(line.end()-1);
	// cout<<line<<endl;
    infile.close();

    int uid=1000;
    ifstream infile2("/etc/passwd");
    // regex r1("(.*)(:1000:)(.*)");
    string pattrn="(.*)(:"+to_string(uid)+":)(.*)";
  	regex r1(pattrn);
    int count=1;
    // char *token = strtok(const_cast<char*>(line.c_str()), delim);
    while (getline(infile2, line))
	{
		// cout<<line<<endl;
		if(regex_match(line,r1)){
			// cout<<line<<endl;

			char *token = strtok(const_cast<char*>(line.c_str()), delim);
			// token = strtok(nullptr,delim);
			while (token != nullptr)
			{
				// dirent *pdir;
				// DIR *dir = opendir(token);

				// if (dir == NULL) {
				// 	return;
				// }

				// while ((pdir = readdir(dir))){
				// 	if(strcmp(pdir->d_name,".")==0||strcmp(pdir->d_name,"..")==0)
				// 		continue;
				// 	string real=string(token)+"/"+pdir->d_name;
				// 	executable_var.insert(make_pair(pdir->d_name,real));
				// }
		  //  		closedir(dir);
				if(count==1 || count==6){
					cout<<token<<endl;}
					// cout<<count<<endl;
					++count;
				token = strtok(nullptr, delim);
			}


		}

	    // process pair (a,b)
	}






	infile.close();

	return 0;
}