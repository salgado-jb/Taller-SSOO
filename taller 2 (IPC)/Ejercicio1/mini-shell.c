#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

static int run(char ***progs, size_t count)
{	
	int r, status;
	int pipes[count][2];
	for(int i =0; i< count; i++){
		pipe(pipes[i]);
	}
	enum {READ, WRITE};
	int hijo_numero;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);

	int pid_padre = getpid();
	for(int i = 0; i<count; i++){
		if(getpid()==pid_padre){
			int pid_hijo = fork();
			if(pid_hijo !=0){
				children[i] = pid_hijo;
			}else{
				hijo_numero = i;
			}
		}
	}
	
	

	//TODO: Pensar cuantos procesos necesito
	//TODO: Pensar cuantos pipes necesito.

	if(getpid() == pid_padre){
		dup2(WRITE,pipes[count][WRITE]);
		void* resultado;
		read(pipes[count][READ], &resultado, sizeof(void*));
	}else{
		if(hijo_numero != count){
			dup2(pipes[hijo_numero][WRITE],WRITE);
			if(hijo_numero != 0){
				dup2(pipes[hijo_numero-1][READ],READ);
			}else{
				dup2(pipes[hijo_numero][READ],READ);
			}
		}else{
				dup2(pipes[hijo_numero-1][READ],READ);
				dup2(pipes[hijo_numero][WRITE],WRITE);
		}

		if(hijo_numero == 0){
			void* result = execlp(progs[0][0],progs[0],NULL);
		}else{
			void* result = execlp(progs[hijo_numero][0],progs[hijo_numero],NULL);
		}


		exit(0);
	}	
	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	return r;
	exit(0);
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
