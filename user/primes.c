#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winfinite-recursion"

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 35
int p[2];
int r, w;
int temp;
int n;
int first = 1;
  
void recursive_check();

int
main(int argc, char *argv[])
{
  recursive_check();
  exit(0);
}

void recursive_check() {
  if (first == 1){
    first = 0;
    pipe(p);
    r = p[0];
    w = p[1];
    for (n = 2; n <= MAX; n++){
      write(w, (void *)&n, 4);
    }
    close(w);
  }
  
  if (fork() == 0){
    if (read(r, (void *)&temp, 4)){
      pipe(p);
      w = p[1];
      printf("prime %d\n", temp);
    }
    while (read(r, (void *)&n, 4) != 0){
      if (n % temp != 0){
        write(w, (void *)&n, 4);
      }
    }
    r = p[0];
    close(w);
    recursive_check();
  }
  else {
    wait((int *)0);
    close(r);
  }
  exit(0);
}

#pragma GCC diagnostic pop

