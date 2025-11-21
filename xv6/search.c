#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFSIZE 512

// simple strstr for xv6
char* substr(const char *str, const char *sub) {
    int i, j;
    for(i = 0; str[i] != '\0'; i++){
        for(j = 0; sub[j] != '\0' && str[i + j] == sub[j]; j++);
        if(sub[j] == '\0')
            return (char*)&str[i];
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf(1, "Usage: search <file> <keyword>\n");
        exit();
    }

    int fd = open(argv[1], 0);
    if(fd < 0){
        printf(1, "search: cannot open %s\n", argv[1]);
        exit();
    }

    char buffer[BUFSIZE];
    char line[BUFSIZE];
    int n, pos = 0;

    while((n = read(fd, buffer, sizeof(buffer))) > 0){
        for(int i = 0; i < n; i++){
            if(buffer[i] == '\n' || pos == BUFSIZE-1){
                line[pos] = '\0';

                if(substr(line, argv[2]))
                    printf(1, "%s\n", line);

                pos = 0;
            } else {
                line[pos++] = buffer[i];
            }
        }
    }

    close(fd);
    exit();
}
