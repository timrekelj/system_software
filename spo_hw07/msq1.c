#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 16 * 1024

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("Error creating key");
        exit(EXIT_FAILURE);
    }

    int msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("Error creating message queue");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer msg;
    msg.msg_type = 1;

    // Reading file content
    size_t bytesRead = fread(msg.msg_text, 1, MSG_SIZE, file);

    // Sending message
    if (msgsnd(msg_id, &msg, bytesRead, 0) == -1) {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("Message sent\n");

    return 0;
}

