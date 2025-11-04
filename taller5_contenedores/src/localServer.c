#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define BUFFER_SIZE 256

int enviarACalcular(char *expresion) {
    const char *host = getenv("EXTERNAL_HOST");
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct addrinfo hints = {0}, *server_addr = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int rc = getaddrinfo(host, "8080", &hints, &server_addr);

    connect(client_socket, server_addr->ai_addr, server_addr->ai_addrlen);
    printf("Conectado al servidor externo!!!\n\n");

    send(client_socket, expresion, strlen(expresion), 0);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_recibidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    buffer[bytes_recibidos] = '\0';

    printf("Respuesta del servidor externo: %s\n", buffer);

    close(client_socket);
    return atoi(buffer);
}

int main() {
    setvbuf(stdout, NULL, _IOLBF, 0);
    unlink(SOCKET_PATH);

    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un server_addr = {0};
    char buffer[BUFFER_SIZE];

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    while(1){
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Conexion aceptada\n");
        if (fork() == 0) {
            while(1){
                printf("------------------------\n");
                ssize_t bytes_recibidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
                if(bytes_recibidos == 0) {
                    break;
                }
                else if(bytes_recibidos < 0) {
                    printf("Error recibiendo datos\n");
                    break;
                }
                buffer[bytes_recibidos] = '\0';

                printf("Recibido: %s\n", buffer);
                int resultado = enviarACalcular(buffer);
                snprintf(buffer, 5, "%d", resultado);

                if(send(client_socket, buffer, strlen(buffer), 0) <= 0) {
                    printf("Error enviando datos\n");
                    break;
                }
            }
            printf("Conexion terminada\n");
            exit(0);
        }
    }
    printf("Conexion terminada while\n");
    close(server_socket);
    unlink(SOCKET_PATH);
    exit(0);
}
