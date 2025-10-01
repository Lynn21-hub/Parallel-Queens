// user/login.c — file-based login for xv6
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAXLINE 128

static void readline(char *buf, int sz) {
  int i = 0;
  while (i + 1 < sz) {
    char c;
    int n = read(0, &c, 1);
    if (n < 1) break;
    if (c == '\r') continue;
    if (c == '\n') break;
    buf[i++] = c;
  }
  buf[i] = 0;
}

static int equals(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

static int check_passwd(const char *user, const char *pass) {
  int fd = open("passwd", O_RDONLY);
  if (fd < 0) {
    printf(1, "login: cannot open passwd file\n");
    return 0;
  }

  char line[MAXLINE];
  int i = 0;
  char ch;
  int n;

  while ((n = read(fd, &ch, 1)) == 1) {
    if (ch == '\n' || i >= MAXLINE - 1) {
      // terminate current line
      line[i] = 0;
      // parse "name:pw"
      if (i > 0) {
        char *colon = 0;
        for (int k = 0; k < i; k++) {
          if (line[k] == ':') { colon = &line[k]; break; }
        }
        if (colon) {
          *colon = 0;
          const char *name = line;
          const char *pw   = colon + 1;
          if (equals(name, user) && equals(pw, pass)) {
            close(fd);
            return 1;
          }
        }
      }
      i = 0; // reset for next line
    } else {
      line[i++] = ch;
    }
  }

  // handle last line if it didn't end with newline
  if (i > 0) {
    line[i] = 0;
    char *colon = 0;
    for (int k = 0; k < i; k++) {
      if (line[k] == ':') { colon = &line[k]; break; }
    }
    if (colon) {
      *colon = 0;
      const char *name = line;
      const char *pw   = colon + 1;
      if (equals(name, user) && equals(pw, pass)) {
        close(fd);
        return 1;
      }
    }
  }

  close(fd);
  return 0;
}

int
main(void)
{
  char user[32], pass[32];

  printf(1, "\n=== xv6 login ===\n");
  for (;;) {
    printf(1, "Username: ");
    readline(user, sizeof user);

    // NOTE: xv6 console echoes input; password won't be hidden.
    printf(1, "Password: ");
    readline(pass, sizeof pass);

    if (check_passwd(user, pass)) {
      printf(1, "Login ok. Starting sh...\n");
      char *argv[] = { "sh", 0 };
      exec("sh", argv);
      printf(1, "login: exec sh failed\n");
      exit();
    } else {
      printf(1, "Login failed. Try again.\n");
    }
  }
}
