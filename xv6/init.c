// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"



static void ensure_passwd(void){
    int fd=open("passwd",O_RDONLY);
    if(fd>=0){close(fd); return;}  //already exists
    fd=open("passwd", O_CREATE| O_WRONLY);
    if(fd<0){
      printf(1,"init : Can't create passwd\n");
      return;
    }
   const char *l1 = "lynn:l523Z@aub\n";
   const char *l2 = "GhinaSabbagh:Ghinasab5678\n";

  write(fd, l1, strlen(l1));
  write(fd, l2, strlen(l2));
  close(fd);
}
 

int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
   dup(0);  // stdout
   dup(0);  // stderr
   ensure_passwd(); 

  for(;;){
    printf(1, "init: starting login\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      char *argv_login[] = { "login", 0 };
      exec("login", argv_login);
      printf(1, "init: exec login failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }

}
