#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

// safer indent printer for xv6 (no %*s)
void print_indent(int n) {
  for (int i = 0; i < n; i++)
    printf(1, " ");
}

void tree(char *path, int depth) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // open current path
  if((fd = open(path, 0)) < 0){
    printf(1, "tree: cannot open %s\n", path);
    return;
  }

  // get stat info
  if(fstat(fd, &st) < 0){
    printf(1, "tree: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // if it's a file → just print it
  if(st.type == T_FILE){
    print_indent(depth * 2);
    printf(1, "%s\n", path);
    close(fd);
    return;
  }

  // print directory name
  print_indent(depth * 2);
  printf(1, "%s/\n", path);

  // read directory entries
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue;
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // get stat of each entry to know if dir or file
    if(stat(buf, &st) >= 0){
      if(st.type == T_DIR){
        tree(buf, depth + 1);
      } else {
        print_indent((depth + 1) * 2);
        printf(1, "%s\n", de.name);
      }
    }
  }

  close(fd);
}

int main(int argc, char *argv[]){
  if(argc < 2)
    tree(".", 0);
  else
    tree(argv[1], 0);
  exit();
}
