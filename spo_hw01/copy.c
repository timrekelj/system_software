#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 128

int main(int argc, char *argv[]) {
    int read_file, copy_file, n;
    char buff[BUFF_SIZE];
    char copy[] = "_kopija";

    if (argc != 2) {
        perror("Program needs 2 arguments");
        exit(-1);
    }

    read_file = open(argv[1], O_RDONLY);
    if (read_file < 0) {
        perror("Error opening read file");
        exit(1);
    }

    char *extension = strrchr(argv[1], '.');
    char *copy_file_name;
    if (extension != NULL) {
        int index = extension - argv[1];
        strncpy(copy, argv[1], index);
        copy[index] = '\0';
        strcat(copy, "_kopija");
        copy_file_name = strcat(copy, extension);
    } else {
        copy_file_name = strcat(argv[1], copy);
    }

    copy_file = open(copy_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (copy_file < 0) {
        perror("Error opening copy file");
        exit(1);
    }

    do {
        n = read(read_file, buff, BUFF_SIZE);
        if (n < 0) perror("Error reading file");
        if (write(copy_file, buff, n) != n) perror("Error writing to file");
    } while (n != 0);

    if (close(read_file) != 0) perror("Error closing read file");
    if (close(copy_file) != 0) perror("Error closing copy file");

    return 0;
}

