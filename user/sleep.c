#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2){
    printf("exec sleep: sleep {seconds}\n");
    exit(0);
  }
  else {
    int tick = atoi(argv[1]);
    sleep(tick);
    exit(0);
  }
}
