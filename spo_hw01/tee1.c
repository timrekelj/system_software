#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 128

int main(int argc, char *argv[]) {
    int n, write_file;
    char buff[BUFF_SIZE];

    if (argc == 3 && strcmp(argv[1], "-a") == 0) write_file = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if(argc == 2) write_file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else {
        printf("Wrong arguments\n");
        return 1;
    }

    if (write_file < 0) {
        perror("Error opening write file");
        exit(1);
    }

    while ((n = read(0, buff, BUFF_SIZE)) > 0) {
        if (n < 0) perror("Error reading file");
        if (write(2, buff, n) != n) printf("No reading done\n");
        if (write(write_file, buff, n) != n) printf("No reading done\n");
    };

    if (n < 0) perror("Read error");

    return 0;
}
