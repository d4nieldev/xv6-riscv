#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(2, "Usage: %s <affinity_mask>\n", argv[0]);
    exit(1, 0);
  }
  
  int affinity_mask = argv[1] ? atoi(argv[1]) : 0;
  setaffinity(affinity_mask);
  for (;;) {
    printf("My PID is %d\n", getpid());
  }
  exit(0, 0);
}
