#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "/tmp/servidortaller"

int calcular(const char *expresion) {
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
     
    // COMPLETAR. Este es un ejemplo de funcionamiento básico.
    // La expresión debe ser recibida como un mensaje del cliente hacia el servidor.
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

    int listenfd = socket(AF_UNIX,SOCK_STREAM,0);
    unlink(SOCK_PATH);
    bind(listenfd,(struct sockaddr*) &addr, sizeof(addr));
    listen(listenfd, 5);

    while(1){

        int clientefd = accept(listenfd,NULL,NULL);
        int pid_hijo_servidor = fork();

        if(pid_hijo_servidor == 0){
           while(1){
            void* operacion;
            ssize_t bytes = recv(clientefd, &operacion, sizeof(void*), 0);
            if(bytes == 0){
                break;
            }
            int resultado = calcular(&operacion);
            send(clientefd, &resultado, sizeof(int),0);
            printf("El resultado de la operación es: %d\n", resultado);
            }
            exit(0);
        }

    }
    exit(0);
}

