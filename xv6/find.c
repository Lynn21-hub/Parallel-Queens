#include "types.h"
#include "stat.h"
#include "fs.h"
#include "user.h"

void find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        printf(1, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        printf(1, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR){
        close(fd);
        return;
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) continue;
        if(!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;

        // Build path
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0) continue;

        if(strcmp(de.name, target) == 0){
            printf(1, "%s\n", buf);
        }

        if(st.type == T_DIR){
            find(buf, target);
        }
    }

    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf(1, "Usage: find <directory> <filename>\n");
        exit();
    }

    find(argv[1], argv[2]);
    exit();
}
