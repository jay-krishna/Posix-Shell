#include<iostream>
#include<unistd.h>

using namespace std;

int main()
{
	// char *environ[8]={"/usr/local/sbin","/usr/local/bin","/usr/sbin","/usr/bin","/bin","/usr/games","/usr/local/games",NULL};
	// char *environ[1];

	char *environ[] =
    {
        "HOME=/",
        "PATH=/home/singular/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games",
        // "TZ=UTC0",
        "USER=singular",
        "LOGNAME=singular",
        0
    };

    char *argv[] = { "/bin/cat", "-/home/singular/Documents/OS-Assignment-1/final/ev.cpp", 0 };



	string p = "/bin/cat";
	environ[0] = (char *)p.c_str();
	string s = "cat";
	string ss = "/home/singular/Documents/OS-Assignment-1/final/ev.cpp";
	string sss = "ls";
	char *comm[3];
	comm[0] = (char *)s.c_str();
	comm[1] = (char *)ss.c_str();
	comm[2] = NULL;
	if((execve(argv[0], &argv[0],environ)) == -1) cout << "Not found" << endl;

    return 0;

}