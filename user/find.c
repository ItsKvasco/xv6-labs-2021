#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *append(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

void find(char *path, char *fileName)
{
    int fd, fd1;
    char buff[256];
    struct stat st;
    struct dirent de;
    
    //opens file, if cant open print
    if((fd = open(path, 0)) < 0){ 
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    //reading from fd, saving values to dirent
    while(read(fd, &de, sizeof(de))==sizeof(de))
    {
            //copying path to buffer
            strcpy(buff, path);
            //continue on . and ..
            if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")){
                continue;
            }
            //appending / and file to buff
            if(strcmp(buff, "/")){
                append(buff, "/");
                append(buff, de.name);
            }
            if((fd1 = open(buff, 0)) < 0){
                fprintf(2, "find: cannot open %s\n", de.name);
                return;
            }
            if(fstat(fd1, &st) < 0){
                continue;
            }
            //directory
            if(st.type == 1){
                //recursive call 
                find(buff, fileName);
            }
            // file or console
            else{
                if(!strcmp(de.name,fileName)){
                    printf("%s\n", buff); 
                }
            }
        close(fd1);
    }
    close(fd);
}


int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "usage: find <path> <file name>\n");
        exit(-1);
    }

    char *fileName = argv[2];
    char *path = argv[1];
   
    find(path, fileName);
    exit(0);
}
