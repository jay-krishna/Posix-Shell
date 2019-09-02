#include<iostream>
#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */
// #if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */
#include <sys/wait.h>

#define	MAXLINE	4096

using namespace std;

main(void)
{
char buf[MAXLINE]; /* from apue.h */
pid_t pid;
int status;
printf("%% "); /* print prompt (printf requires %% to print %) */
while (fgets(buf, MAXLINE, stdin) != NULL) {
	printf("%i\n",(int)strlen(buf));
if (buf[strlen(buf) - 1] == '\n')
buf[strlen(buf) - 1] = '\0'; /* replace newline with null */
if ((pid = fork()) < 0) {
printf("fork error\n");
} else if (pid == 0) { /* child */
// cout<<"Buffer is "<<buf<<endl;
printf("Buffer is: %c\n", buf[0]);

char *command[3];
	string s = "/bin/cat";
	string ss = "/home/singular/Documents/OS-Assignment-1/try.cpp";
	string p="PATH=/home/singular/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games";
	command[0] = (char *)s.c_str();
	command[1] = (char *)ss.c_str();
	command[2] = NULL;

	char *pth[2];
	pth[0]=(char *)p.c_str();
	pth[1]=NULL;


	auto e = execve(command[0],command,p);
//	e = execvp("/bin/ls",command);
  //e = execve("/bin/ls", argv, envp);
  if (e == -1)
      fprintf(stderr, "Error: %s\n", strerror(errno));






// execlp(buf, buf, (char *)0);
perror("couldn’t execute:");// %s\n", buf);
exit(127);
}
/* parent */
if ((pid = waitpid(pid, &status, 0)) < 0)
printf("waitpid error\n");
printf("%% ");
}
exit(0);
}