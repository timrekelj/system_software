#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uporaba: %s <strežnik_IP> <vrata>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    const int server_port = atoi(argv[2]);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Napaka pri ustvarjanju vtičnice");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("Napaka pri pretvarjanju IP naslova");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Napaka pri povezovanju");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 5; i++) {
        char buffer[MAX_BUFFER_SIZE];
        printf("Vnesite niz za pošiljanje strežniku: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        write(client_socket, buffer, strlen(buffer));

        memset(buffer, 0, sizeof(buffer));
        read(client_socket, buffer, sizeof(buffer));
        printf("Strežnik je vrnil: %s\n", buffer);
    }


    close(client_socket);
    return 0;
}

