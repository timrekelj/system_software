#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 256
#define TRUE 1

void error(char message[]) {
    char err[100];
    strcpy(err, "Error: ");
    strcat(err, message);
    strcat(err, "\n");
    write(1, err, strlen(err));
    exit(1);
}

int main(int argc, char *argv[]) {
    int read_file, write_file;
    char buff[BUFF_SIZE], buff_byte[1];
    int n = 5;

    // check for arguments 
    if ( argc != 4)
        error("the right program arguments are: ./a.out -n 5 <filename>");

    // check for -n and open files
    n = atoi(argv[2]);
    read_file = open(argv[3], O_RDONLY);
    if (read_file < 0)
        error("opening read file");

    write_file = open(argv[3], O_RDWR);
    if (write_file < 0)
        error("opening read file");

    // go to n-th line
    int i = 0;
    while (i < (n-1)) {
        if (read(read_file, buff_byte, 1) != 1)
            error("Error reading file");
        if (buff_byte[0] == '\n') i++;
    }

    // set the location of write_file current location of read_file
    int current_position = lseek(read_file, 0, SEEK_CUR);
    if (lseek(write_file, current_position, SEEK_SET) == -1)
        error("Error seeking file");

    // move read_file to the next line
    int read_bytes = 0;
    while (TRUE) {
        if (read(read_file, buff_byte, 1) != 1)
            error("Error reading file");
        read_bytes++;
        if (buff_byte[0] == '\n')
            break;
    }

    // copy the rest of the file
    // this copies last line twice
    int bytes_read;
    while ((bytes_read = read(read_file, buff, BUFF_SIZE)) > 0)
        if (write(write_file, buff, bytes_read) != bytes_read)
            error("Error writing file");

    // go to thie end of file
    if (lseek(write_file, 0, SEEK_END) == -1)
        error("Error seeking file");

    // delete last line
    if (ftruncate(write_file, lseek(write_file, 0, SEEK_CUR) - read_bytes) == -1)
        error("Error truncating file");

    // close files
    if (close(write_file) != 0)
        error("Error closing write file");
    if (close(read_file) != 0)
        error("Error closing read file");

    return 0;
}
