#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFERSIZE 2
#define CHARBUF 255
#define COPYMODE 0644

void main(int ac, char *av[])
{
if(ac != 3){
printf("error");
exit(0);
}

int in_fd, out_fd;
char temp[CHARBUF], buf[BUFFERSIZE];
struct stat info;

in_fd = open(av[1], O_RDONLY);
if(in_fd == -1){
printf("in_fd error");
exit(0);
}

if(stat(av[2], &info) != -1){
if(S_ISDIR(info.st_mode)){
strcpy(temp, av[2]);
strcat(temp, "/");
strcat(temp, av[1]);

out_fd = creat(temp, COPYMODE);
while(read(in_fd, buf, 1) > 0) write(out_fd, buf, 1);
}
}
else{
out_fd = open(av[2], O_CREAT|O_TRUNC|O_WRONLY);
while(read(in_fd, buf, 1) > 0) write(out_fd, buf, 1);
}

remove(av[1]);
close(in_fd);
close(out_fd);
}
