#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buffer[512];
    char *xargs_argv[MAXARG];
    int i, j, k, n;

    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args ...]\n");
        exit(1);
    }

    // Copy the initial command and arguments
    for (i = 0; i < argc - 1; i++) {
        xargs_argv[i] = argv[i + 1];
    }

    while ((n = read(0, buffer, sizeof(buffer))) > 0) {
        j = 0;
        while (j < n) {
            // Prepare arguments for each line of input
            k = i;
            while (j < n && buffer[j] != '\n') {
                xargs_argv[k++] = &buffer[j];
                while (j < n && buffer[j] != ' ' && buffer[j] != '\n') j++;
                if (buffer[j] == ' ' || buffer[j] == '\n') buffer[j++] = 0;
            }

            if (k < MAXARG) {
                xargs_argv[k] = 0;
                if (fork() == 0) {
                    exec(xargs_argv[0], xargs_argv);
                    fprintf(2, "exec %s failed\n", xargs_argv[0]);
                    exit(1);
                } else {
                    wait(0);
                }
            }
        }
    }

    exit(0);
}

