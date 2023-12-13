#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

// ./mojChmod 0777 mojChmod.c
// ./mojChmod g+w mojChmod.c

void main(int argc, char *argv[]) {

    // Check if there are enough arguments
    if (argc != 3) {
        printf("Usage: %s <mode> <file>\n", argv[0]);
        exit(1);
    }

    char *mode = argv[1];
    char *file = argv[2];
    struct stat buf;
    
    // Check if file exists
    if (stat(file, &buf) == -1) {
        perror("Error opening file");
        exit(1);
    }
    
    if (strlen(mode) == 3) { // If mode is 3 characters long, it means that it is in the form of g+w
        // Get current permissions
        int currentPermissions = buf.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

        // Get new permissions
        int newPermissions = 0;

        if (mode[1] == '+') {
            if (mode[2] == 'r') {
                if (mode[0] == 'u') newPermissions = currentPermissions | S_IRUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions | S_IRGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions | S_IROTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else if (mode[2] == 'w') {
                if (mode[0] == 'u') newPermissions = currentPermissions | S_IWUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions | S_IWGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions | S_IWOTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else if (mode[2] == 'x') {
                if (mode[0] == 'u') newPermissions = currentPermissions | S_IXUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions | S_IXGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions | S_IXOTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
                
            } else if (mode[2] == 's') {
                if (mode[0] == 'u') newPermissions = currentPermissions | S_ISUID;
                else if (mode[0] == 'g') newPermissions = currentPermissions | S_ISGID;
                else if (mode[0] == 'o') newPermissions = currentPermissions | S_ISVTX;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else {
                printf("Usage: %s <mode> <file>\n", argv[0]);
                exit(1);
            }
        } else if (mode[1] == '-') {
            if (mode[2] == 'r') {
                if (mode[0] == 'u') newPermissions = currentPermissions & ~S_IRUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions & ~S_IRGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions & ~S_IROTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else if (mode[2] == 'w') {
                if (mode[0] == 'u') newPermissions = currentPermissions & ~S_IWUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions & ~S_IWGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions & ~S_IWOTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else if (mode[2] == 'x') {
                if (mode[0] == 'u') newPermissions = currentPermissions & ~S_IXUSR;
                else if (mode[0] == 'g') newPermissions = currentPermissions & ~S_IXGRP;
                else if (mode[0] == 'o') newPermissions = currentPermissions & ~S_IXOTH;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else if (mode[2] == 's') {
                if (mode[0] == 'u') newPermissions = currentPermissions & ~S_ISUID;
                else if (mode[0] == 'g') newPermissions = currentPermissions & ~S_ISGID;
                else if (mode[0] == 'o') newPermissions = currentPermissions & ~S_ISVTX;
                else {
                    printf("Usage: %s <mode> <file>\n", argv[0]);
                    exit(1);
                }
            } else {
                printf("Usage: %s <mode> <file>\n", argv[0]);
                exit(1);
            }
        } else {
            printf("Usage: %s <mode> <file>\n", argv[0]);
            exit(1);
        }

        // Set new permissions
        chmod(file, newPermissions);
    } else { // If mode is 4 characters long, it means that it is in the form of 0777
        chmod(file, strtol(mode, 0, 8));
    }

    // Update file permissions
    if (stat(file, &buf) == -1) {
        perror("Error opening file");
        exit(1);
    }

    // Print new permissions
    printf("Nova dovoljenja: ");
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
    } printf("\n");
}
