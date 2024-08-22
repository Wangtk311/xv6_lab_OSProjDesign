#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[128] = { 0 };
  int spid = 0;
  int fpid = 0;
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  
  if (fork() == 0) {
    close(p1[1]);
    close(p2[0]);
    spid = getpid();
    if (read(p1[0], buf, 1) == 1){
      printf("%d: received ping\n", spid);
      write(p2[1], buf, 1);
    }
  }
  else { 
    close(p2[1]);
    close(p1[0]);
    fpid = getpid();
    write(p1[1], buf, 1);
    if (read(p2[0], buf, 1) == 1){
      printf("%d: received pong\n", fpid);
    }
  }
  exit(0);
}
