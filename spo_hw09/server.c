#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void process_client(int client_socket) {
    char buffer[MAX_BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    read(client_socket, buffer, sizeof(buffer));

    // Implementacija iskanja niza v datoteki (npr. ./server.c)
    // Primer: Pošljite vrstice, kjer se nahaja niz, na odjemalca

    // Simulacija odgovora
    const char *response = "response";  // Vrstice, kjer se nahaja niz
    write(client_socket, response, strlen(response));

    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uporaba: %s <vrata>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int server_port = atoi(argv[1]);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Napaka pri ustvarjanju vtičnice");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(server_port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Napaka pri vezavi na vtičnico");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Napaka pri poslušanju na vtičnici");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Strežnik čaka na povezave...\n");

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);

        if (client_socket == -1) {
            perror("Napaka pri sprejemanju povezave");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip));
        printf("Povezan odjemalec: %s:%d\n", client_ip, ntohs(client_address.sin_port));

        process_client(client_socket);
    }

    close(server_socket);
    return 0;
}

