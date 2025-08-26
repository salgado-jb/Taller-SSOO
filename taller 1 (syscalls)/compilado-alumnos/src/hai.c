#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador_ya_va = 0;

void sigurg_handler(int signum){
	contador_ya_va++;
    printf("ya va!\n");
    return;
}

void sigint_handler(int signum){
	wait(NULL);
}

int main(int argc, char* argv[]) {
  	// Completar
	char* funcion = argv[1];
	char* argumentos[argc];

	for (int i = 1; i < argc; i++){
		argumentos[i - 1] = argv[i];
	}
	argumentos[argc - 1] = NULL;

	int pid_hijo = fork();

	if (pid_hijo != 0) {
		signal(SIGINT, sigint_handler);
		for(int i = 0; i < 5; i++) {
			sleep(1);
			printf("sup!\n");
			kill(pid_hijo, SIGURG);
		}
	} else {
		signal(SIGURG, sigurg_handler);
		while(contador_ya_va < 5) {}
		kill(getppid(), SIGINT);
		execvp(funcion, argumentos);
	}
	return 0;
}

//cuando uso execve tengo que poner: ./hai /bin/echo "Hola"
//cuando uso execvp puedo usar ./hai echo "Hola"
//El ultimo elemento del array de argumentos tiene que ser NULL y el primero el nombre de la funcion
//Se apagaba por problema de la computadora??
//Puede que falte un wait al final

/*
1) dos procesos

2) 
*/