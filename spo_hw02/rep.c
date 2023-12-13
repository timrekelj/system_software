#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 256

void error(char mess[]) {
    char err[100];
    strcpy(err, "Error: ");
    strcat(err, mess);
    write(1, err, strlen(err));
    exit(1);
}

int main(int argc, char *argv[]) {
    int read_file;
    int n = 5;
    char buff[BUFF_SIZE];

    if (argc != 2 && argc != 4) {
        error("The right program arguments are: ./a.out <filename> or ./a.out -n 5 <filename>");
        exit(-1);
    }
    if (argc == 2) {
        read_file = open(argv[1], O_RDONLY);
        if (read_file < 0)
            error("Error opening read file");
    } else {
        n = atoi(argv[2]);
        read_file = open(argv[3], O_RDONLY);
        if (read_file < 0)
            error("Error opening read file");
    }

    int currPos;
    if ((currPos = lseek(read_file, 0, SEEK_END)) == -1)
        error("lseek error");
    
    // go n lines back or to the beginning of the file
    int i = 0;
    while (i < n+1 && currPos != 0) {
        currPos--;
        if (lseek(read_file, currPos, SEEK_SET) == -1)
            error("lseek error");
        read(read_file, buff, 1);
        if (buff[0] == '\n') i++;
    }

    // print the file from the current position
    do {
        n = read(read_file, buff, BUFF_SIZE);
        if (n < 0)
            error("Error reading file");
        if (write(2, buff, n) != n)
            error("Error writing to file");
    } while (n != 0);

    if (close(read_file) != 0)
        error("Error closing read file");

    return 0;
}
