#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define ASK "Interrupted! OK to quit (y/n)?"

int get_ok_char();

void main()
{
void ctrl_c_handler(int);
int i;

signal(SIGINT, ctrl_c_handler);

for(i=0; i<5; i++){
printf("hello\n");
sleep(1);
}
}

void ctrl_c_handler(int signum){
int status;
printf("%s",ASK);
status = tolower(get_ok_char());
if(status == 'y' || status == 'Y') exit(1);
}

int get_ok_char(){
int status;
while((status = getchar()) != EOF && strchr("yYnN", status) == NULL);
return status;
}

