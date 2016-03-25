#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define non_rec 0
#define rec 1

void do_ls(char[], int);
void dostat(char *, char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

main(int ac, char *av[])
{
if(ac==1) do_ls(".", non_rec);
else if (ac==2 && strcmp(av[1],"-R")==0){
do_ls(".", rec);
}
else{
while(--ac) do_ls(*++av, non_rec);
}
}

void do_ls(char dirname[], int check_rec)
{
printf("%s:\n", dirname);
DIR *dir_ptr;
struct dirent *direntp;
if((dir_ptr = opendir(dirname)) == NULL )
fprintf(stderr, "ls1: cannot open %s\n", dirname);
else
{
while((direntp = readdir(dir_ptr))!=NULL)
dostat(direntp->d_name,dirname);
closedir(dir_ptr);
printf("\n");

if(check_rec)
{
dir_ptr = opendir(dirname);
while((direntp = readdir(dir_ptr)) != NULL)
{
char sub_dir[255];
struct stat info;
if(strcmp(direntp->d_name,".") == 0 || strcmp(direntp->d_name,"..") == 0) continue;
sprintf(sub_dir, "%s/%s", dirname, direntp->d_name);
stat(sub_dir, &info);
if(S_ISDIR(info.st_mode)) do_ls(sub_dir, rec);
}
closedir(dir_ptr);
}
}
}

void dostat(char*filename, char*dirname)
{
char dir[255];
struct stat info;
sprintf(dir, "%s/%s",dirname,filename);
if(stat(dir, &info) == -1)
perror(filename);
else
show_file_info(filename, &info);
}

void show_file_info(char *filename, struct stat *info_p)
{
char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
void mode_to_letters();
char modestr[11];

mode_to_letters(info_p->st_mode, modestr);

printf( "%s"	,modestr);
printf( "%4d "	,(int)info_p->st_nlink);
printf( "%-15s "	,uid_to_name(info_p->st_uid));
printf( "%-15s "	,gid_to_name(info_p->st_gid));
printf( "%8d "	,(long)info_p->st_size);
printf( "%.12s "	,4+ctime(&info_p->st_mtime));
printf( "%s\n"	,filename);
}

void mode_to_letters(int mode, char str[])
{
strcpy(str, "----------" );
if(S_ISDIR(mode)) str[0] = 'd';
if(S_ISCHR(mode)) str[0] = 'c';
if(S_ISBLK(mode)) str[0] = 'b';

if(mode & S_IRUSR) str[1] = 'r';
if(mode & S_IWUSR) str[2] = 'w';
if(mode & S_IXUSR) str[3] = 'x';

if(mode & S_IRGRP) str[4] = 'r';
if(mode & S_IWGRP) str[5] = 'w';
if(mode & S_IXGRP) str[6] = 'x';

if(mode & S_IROTH) str[7] = 'r';
if(mode & S_IWOTH) str[8] = 'w';
if(mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid)
{
struct passwd *getpwuid(), *pw_ptr;
static char numstr[10];
if((pw_ptr = getpwuid(uid)) == NULL) {
sprintf(numstr, "%d", uid);
return numstr;
}
else
return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
struct group *getgrgid(), *grp_ptr;
static char numstr[10];
if(( grp_ptr = getgrgid(gid)) == NULL ){
sprintf(numstr, "%d", gid);
return numstr;
}
else
return grp_ptr->gr_name;
}
