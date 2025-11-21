#include "types.h"
#include "stat.h"
#include "user.h"

void cpu_job(int loops, int id) {
  for (int i = 0; i < loops; i++) {
    // do something CPU intensive
    asm("nop");
    if (i % (loops / 10) == 0)
      printf(1, "[PID %d] progress: %d%%\n", getpid(), (i * 100) / loops);
  }
  printf(1, "Process %d DONE\n", getpid());
}

int main(int argc, char *argv[]) {
  printf(1, "\n====== MLFQ TEST START ======\n");

  int p1 = fork();
  if (p1 == 0) {
    printf(1, "Child A started (CPU-bound)\n");
    cpu_job(50 * 1000000, 1);
    exit();
  }

  int p2 = fork();
  if (p2 == 0) {
    printf(1, "Child B started (CPU-bound)\n");
    cpu_job(50 * 1000000, 2);
    exit();
  }

  int p3 = fork();
  if (p3 == 0) {
    printf(1, "Child C started (CPU-bound)\n");
    cpu_job(50 * 1000000, 3);
    exit();
  }

  // parent waits
  wait();
  wait();
  wait();

  printf(1, "====== MLFQ TEST END ======\n");
  exit();
}
