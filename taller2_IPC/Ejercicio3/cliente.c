#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCK_PATH "/tmp/servidortaller"

void siginthandler(int signum){
	exit(0);
}

int main() {
	signal(2, siginthandler);
	struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_PATH);

	int socketfd = socket(AF_UNIX, SOCK_STREAM,0);

	while(!connect(socketfd, (struct sockaddr*) &addr, sizeof(addr)));
	while(1){
		char* operacion;
		scanf("%s", &operacion);
		if(strncmp(&operacion, "exit",4)!=0){
			send(socketfd, &operacion, sizeof(operacion),0 );
		}else{
			break;
		}
		int result; 
		recv(socketfd, &result, sizeof(int), 0);
		printf("El resultado de la operación es: %d\n", result);
	}
	
	exit(0);
}
