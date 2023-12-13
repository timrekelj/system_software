#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void main(int argc, char *argv[]) {
    DIR* dir;
    struct dirent* dirp;
    struct stat buf;

    if ((dir = opendir(".")) == NULL) {
        perror("Can't open directory");
        exit(1);
    }

    while ((dirp = readdir(dir)) != NULL) {
        if (lstat(dirp->d_name, &buf) == -1) {
            perror("Can't get file status");
            exit(1);
        }

        if (S_ISDIR(buf.st_mode)) printf("d");
        else if (S_ISLNK(buf.st_mode)) printf("l");
        else if (S_ISREG(buf.st_mode)) printf("-");
        else if (S_ISCHR(buf.st_mode)) printf("c");
        else if (S_ISBLK(buf.st_mode)) printf("b");
        else if (S_ISFIFO(buf.st_mode)) printf("p");
        else if (S_ISSOCK(buf.st_mode)) printf("s");
        else printf("?");

        if (buf.st_mode & S_IRUSR) printf("r");
        else printf("-");
        if (buf.st_mode & S_IWUSR) printf("w");
        else printf("-");
        if (buf.st_mode & S_IXUSR) {
            if (buf.st_mode & S_ISUID) printf("s");
            else printf("x");
        } else {
            if (buf.st_mode & S_ISUID) printf("S");
            else printf("-");
        }

        if (buf.st_mode & S_IRGRP) printf("r");
        else printf("-");
        if (buf.st_mode & S_IWGRP) printf("w");
        else printf("-");
        if (buf.st_mode & S_IXGRP) {
            if (buf.st_mode & S_ISGID) printf("s");
            else printf("x");
        } else {
            if (buf.st_mode & S_ISGID) printf("S");
            else printf("-");
        }

        if (buf.st_mode & S_IROTH) printf("r");
        else printf("-");
        if (buf.st_mode & S_IWOTH) printf("w");
        else printf("-");
        if (buf.st_mode & S_IXOTH) {
            if (buf.st_mode & S_ISVTX) printf("t");
            else printf("x");
        } else {
            if (buf.st_mode & S_ISVTX) printf("T");
            else printf("-");
        }
        
        printf(" %d %s\n",buf.st_nlink , dirp->d_name);
    };

    closedir(dir);
}
