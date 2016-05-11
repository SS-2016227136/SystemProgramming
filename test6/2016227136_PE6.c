#include <stdio.h>
#include <signal.h>

#define MAXARGS 20
#define ARGLEN 100

void main()
{
char *arglist[MAXARGS+1];
int numargs;
char argbuf[ARGLEN];
char *makestring();
void ctrl_c_handler(int);

numargs = 0;

while(numargs < MAXARGS){
signal(SIGINT, ctrl_c_handler);
printf("Arg[%d]?", numargs);
if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
arglist[numargs++] = makestring(argbuf);
else{
if(numargs > 0){
arglist[numargs] = NULL;
execute(arglist);
numargs = 0;
}
}
}
}

void execute(char *arglist[])
{
int pid,exitstatus;
void ctrl_c_handler2(int);
signal(SIGINT, ctrl_c_handler2);
pid = fork();
switch(pid){
case -1:
perror("fork failed");
exit(1);
case 0:
execvp(arglist[0], arglist);
perror("execvp failed");
exit(1);
default:
while(wait(&exitstatus) != pid);

}
}

char *makestring(char *buf)
{
char *cp, *malloc();
buf[strlen(buf)-1] = '\0';
cp = malloc(strlen(buf)+1);
if(cp == NULL) {
fprintf(stderr, "no memory\n");
exit(1);
}
strcpy(cp, buf);
return cp;
}

void ctrl_c_handler(int signum)
{
printf("\n");
exit(1);
}

void ctrl_c_handler2(int signum)
{
printf("\n");
}
