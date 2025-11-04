#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define BUFFER_SIZE 256

int main() {
    setvbuf(stdout, NULL, _IOLBF, 0);
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un server_addr = {0};
    char buffer[BUFFER_SIZE];

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("Conectado al servidor local!!!\n");
    printf("------------------------\n\n");

    while(1){
        printf("Escribe un mensaje: \n");
        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "exit", 4) == 0) break;

        if(send(client_socket, buffer, strlen(buffer), 0) <= 0) {
			printf("Error enviando datos\n");
			break;
		}

        ssize_t bytes_recibidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if(bytes_recibidos <= 0) {
			printf("Error recibiendo datos\n");
			break;
		}
        buffer[bytes_recibidos] = '\0';

        printf("Respuesta del servidor local: %s\n\n", buffer);
    }
    
    close(client_socket);
}