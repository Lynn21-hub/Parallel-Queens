#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"

#define BUFSIZE 512

void make_name(char *name, int index) {
    name[0] = 'x';
    name[1] = 'a' + (index / 26);
    name[2] = 'a' + (index % 26);
    name[3] = '\0';
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        printf(1, "Usage: split <file> <size>\n");
        exit();
    }

    int chunk = atoi(argv[2]);
    if(chunk <= 0){
        printf(1, "split: invalid size\n");
        exit();
    }

    int fd = open(argv[1], 0);
    if(fd < 0){
        printf(1, "split: cannot open %s\n", argv[1]);
        exit();
    }

    char buf[BUFSIZE], outname[4];
    int out = -1;
    int index = 0;
    int written = 0;
    int n;

    make_name(outname, index);
    out = open(outname, O_CREATE | O_WRONLY);

    while((n = read(fd, buf, sizeof(buf))) > 0){
        for(int i = 0; i < n; i++){
            if(written == chunk){
                close(out);
                index++;
                make_name(outname, index);
                out = open(outname, O_CREATE | O_WRONLY);
                written = 0;
            }

            write(out, &buf[i], 1);
            written++;
        }
    }

    close(out);
    close(fd);
    exit();
}
