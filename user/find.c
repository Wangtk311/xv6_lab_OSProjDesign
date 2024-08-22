#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void search(char *base, char *target) {
    char fullpath[512];
    int cur_file;
    struct dirent dir_entry;
    struct stat filestat;

    if ((cur_file = open(base, 0)) < 0) {
        fprintf(2, "search: cannot open: %s\n", base);
        return;
    }

    if (fstat(cur_file, &filestat) < 0) {
        fprintf(2, "search: cannot get stat: %s\n", base);
        close(cur_file);
        return;
    }

    if (filestat.type == T_FILE) {
        if (strcmp(base + strlen(base) - strlen(target), target) == 0) {
            printf("%s\n", base);
        }
    }
    else if (filestat.type == T_DIR) {
        if (strlen(base) + DIRSIZ + 2 > sizeof(fullpath)) {
            fprintf(2, "search: path too long\n");
            close(cur_file);
            return;
        }

        strcpy(fullpath, base);
        char *p = fullpath + strlen(fullpath);
        *p++ = '/';

        while (read(cur_file, &dir_entry, sizeof(dir_entry)) == sizeof(dir_entry)) {
            if (dir_entry.inum == 0) continue;

            memmove(p, dir_entry.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if (stat(fullpath, &filestat) < 0) continue;

            if (strcmp(dir_entry.name, ".") != 0 && strcmp(dir_entry.name, "..") != 0) {
                search(fullpath, target);
            }
        }
    }

    close(cur_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "usage: search [directory] [filename]\n");
        exit(1);
    }
    search(argv[1], argv[2]);
    exit(0);
}

