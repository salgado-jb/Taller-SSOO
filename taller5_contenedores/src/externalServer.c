#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256

int calcular(char *expresion) {
    int num1, num2, resultado;
    char operador;

    // Usamos sscanf para extraer los dos números y el operador de la expresión
    if (sscanf(expresion, "%d%c%d", &num1, &operador, &num2) != 3) {
        printf("Formato incorrecto\n");
        return 0;  // En caso de error, retornamos 0.
    }
    // Realizamos la operación según el operador
    switch (operador) {
        case '+':
            resultado = num1 + num2;
            break;
        case '-':
            resultado = num1 - num2;
            break;
        case '*':
            resultado = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
            } else {
                printf("Error: División por cero\n");
                return 0;  // Si hay división por cero, retornamos 0.
            }
            break;
        default:
            printf("Operador no reconocido\n");
            return 0;  // Si el operador no es válido, retornamos 0.
    }

    return resultado;
}

int main() {
    setvbuf(stdout, NULL, _IOLBF, 0);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    while(1){
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Conexion aceptada\n");
        if (fork() == 0) {
            ssize_t bytes_recibidos = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
            if(bytes_recibidos <= 0) {
                printf("Error recibiendo datos\n");
                exit(-1);
            }
            buffer[bytes_recibidos] = '\0';

            printf("Recibido: %s\n", buffer);
            int resultado = calcular(buffer);
            snprintf(buffer, 5, "%d", resultado);
            printf("Respuesta: %s\n", buffer);

            if(send(client_socket, buffer, strlen(buffer), 0) <= 0) {
                printf("Error enviando datos\n");
                exit(-1);
            }

            printf("Conexion terminada\n");
            exit(0);
        }
    }
    close(server_socket);
    exit(0);
}

