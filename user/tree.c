#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void tree(char *path, int depth){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "tree: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (depth == 0) {
        if (strcmp(path, "/") == 0) {
            printf("/\n");
        } else {
            printf("%s/\n", path); 
        }
    }
    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        printf("%s\n", path);
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("tree: path too long\n");
            break;
        }
        strcpy(buf,path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0){
                continue;
            }
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("tree: cannot stat %s\n", buf);
                continue;
            }
            for(int i = 0; i < depth + 1; i++){
                printf("  ");
            }
            if(st.type == T_DIR){
                printf("%s/\n", de.name);
                tree(buf, depth + 1);
            }
            else{
                printf("%s\n", de.name);
            }
        }
    }
    close(fd);
}
int main(int argc, char *argv[]){
    if(argc < 2){
        tree(".", 0);
    }
    else{
        tree(argv[1], 0);
    }
    exit(0);
}