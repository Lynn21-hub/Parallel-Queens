#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  getprocs();   // kernel will print the table
  exit();
}
