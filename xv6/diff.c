#include "types.h"
#include "stat.h"
#include "user.h"

#define MAXLINE 256

// Reads one line from fd into buf, removing \n and \r.
// Returns -1 at EOF, or number of chars read.
int getline(int fd, char *buf) {
  int i = 0;
  char c;

  // clear buffer each time to avoid leftover characters
  for (int j = 0; j < MAXLINE; j++)
    buf[j] = 0;

  while (read(fd, &c, 1) == 1) {
    if (c == '\n' || c == '\r' || i == MAXLINE - 1)
      break;
    buf[i++] = c;
  }

  if (i == 0 && c != '\n' && c != '\r')
    return -1; // EOF
  buf[i] = 0;
  return i;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf(1, "Usage: diff file1 file2\n");
    exit();
  }

  int fd1 = open(argv[1], 0);
  int fd2 = open(argv[2], 0);
  if (fd1 < 0 || fd2 < 0) {
    printf(1, "diff: cannot open file(s)\n");
    exit();
  }

  char buf1[MAXLINE], buf2[MAXLINE];
  int line = 1;

  while (1) {
    int r1 = getline(fd1, buf1);
    int r2 = getline(fd2, buf2);

    if (r1 < 0 && r2 < 0)
      break;

    // Only print if they differ
    if (r1 < 0 || r2 < 0 || strcmp(buf1, buf2) != 0)
      printf(1, "%d: %s | %s\n", line, buf1, buf2);

    line++;
  }

  close(fd1);
  close(fd2);
  exit();
}
