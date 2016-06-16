#include <stdio.h>
#include <unistd.h>

#define oops(m,x) {perror(m); exit(x);}

void main(int ac, char **av)
{
int thepipe1[2], thepipe2[2], newfd, pid1, pid2;

if(ac != 4){
fprintf(stderr, "usage: pipe cmd1 cmd2 cmd3 \n");
exit(1);
}
if(pipe(thepipe1) == -1) oops("Cannot get a pipe1", 1);
if((pid1 = fork()) == -1) oops("Cannot fork1", 2);

if(pid1 > 0){
close(thepipe1[1]);
if(dup2(thepipe1[0], 0) == -1) oops("Could not redirect stdin", 3);
close(thepipe1[0]);
execlp(av[3], av[3], NULL);
oops(av[3], 4);
}

else{
if(pipe(thepipe2) == -1) oops("Cannot get a pipe2", 5);
if((pid2 = fork()) == -1) oops("Cannot fork2", 6);
if(pid2 > 0){
close(thepipe1[0]);
if(dup2(thepipe1[1], 1) == -1) oops("Could not redirect stdout", 7);
close(thepipe1[1]);

close(thepipe2[1]);
if(dup2(thepipe2[0], 0) == -1) oops("Could not redirect stdin2", 8);
close(thepipe2[0]);
execlp(av[2], av[2], NULL);
oops(av[2], 9);
}

else{
close(thepipe2[0]);
if(dup2(thepipe2[1], 1) == -1) oops("Could not redirect stdout2", 10);
close(thepipe2[1]);
execlp(av[1], av[1], NULL);
oops(av[1], 11);
}
}
}
