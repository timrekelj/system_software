#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 16 * 1024 // Max message size

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int countWords(const char *str) {
    int count = 0;
    int inWord = 0;

    while (*str) {
        if (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
            if (inWord) {
                inWord = 0;
                count++;
            }
        } else {
            inWord = 1;
        }
        str++;
    }

    if (inWord) {
        count++;
    }

    return count;
}

int main() {
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
    // Reading message
    ssize_t bytesRead = msgrcv(msg_id, &msg, MSG_SIZE, 1, 0);
    if (bytesRead == -1) {
        perror("Error reading message");
        exit(EXIT_FAILURE);
    }

    // Counting words
    int wordCount = countWords(msg.msg_text);
    printf("Number of words in message: %d\n", wordCount);

    // Removing message queue
    if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
        perror("Error removing message queue");
        exit(EXIT_FAILURE);
    }

    return 0;
}

