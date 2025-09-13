#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

int generate_random_number(){
	return (rand() % 50);
}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);
	int pid_padre;
	int hijo_numero;
	int pipes[n][2];
	for(int i =0; i< n; i++){
		pipe(pipes[i]);
	}
	int pipe_resultado[2][2];
	pipe(pipe_resultado[1]);
	enum {READ, WRITE};
	int numero_secreto;
	int children[n];
	int result;

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
	/* COMPLETAR */
	printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
	
	pid_padre = getpid();
	for(int i = 0; i<n; i++){
		if(getpid()==pid_padre){
			int pid_hijo = fork();
			if(pid_hijo ==0){
				hijo_numero = i;
			}else{
				children[i] = pid_hijo;
			}
		}
	}
	if(getpid() == pid_padre){
		dup2(READ, pipe_resultado[0]);
		write(pipes[start][WRITE],&buffer, sizeof(int));
		wait(NULL);
		read(pipe_resultado[1][READ],&result,sizeof(int) );
		hijo_numero = -1;
		for(int i = 1; i<n; i++){
			if(i != start){
				kill(children[i], 9);
			}
		}
	}else{
		if(hijo_numero ==start ){
			int primer_numero;
			read(pipes[hijo_numero][READ],&primer_numero,sizeof(int));
			numero_secreto = generate_random_number();
			
			/*if(hijo_numero != 0){
				dup2(WRITE,pipes[hijo_numero % n][READ]);
				dup2(READ, pipes[hijo_numero-1][WRITE]);
			}else{
				dup2(WRITE,pipes[hijo_numero % n][READ]);
				dup2(READ, pipes[n][WRITE]);
			}*/
			write(pipes[hijo_numero][WRITE], &primer_numero, sizeof(int));

		}else{
			/*if(hijo_numero !=0){
				dup2(WRITE,pipes[hijo_numero % n][READ]);
				dup2(READ, pipes[hijo_numero -1][WRITE]);
			}else{
				dup2(WRITE,pipes[hijo_numero ][READ]);
				dup2(READ, pipes[n][WRITE]);
			}*/
		}
		
	}	
	
    if(hijo_numero == start){
		while(1){
			int numero_recibido;
			if (hijo_numero == 0){
				read(pipes[n][READ],&numero_recibido, sizeof(int) );
			}
			else{
				read(pipes[hijo_numero-1][READ],&numero_recibido, sizeof(int) );
			}
			if(numero_recibido >= numero_secreto){
				/*dup2(WRITE, pipe_resultado[1]);*/
				write(pipe_resultado[1][WRITE], &numero_recibido, sizeof(int));
				exit(0);
			}else{
				numero_recibido++;
				write(pipes[hijo_numero][WRITE], &numero_recibido, sizeof(int));
			}
		}
	}else{
		if(hijo_numero != -1){
		while(1){
			int numero_recibido;
			if (hijo_numero == 0){
				read(pipes[n][READ],&numero_recibido, sizeof(int) );
			}
			else{
				read(pipes[hijo_numero-1][READ],&numero_recibido, sizeof(int) );
			}
			numero_recibido++;
			write(pipes[hijo_numero][WRITE], &numero_recibido, sizeof(int));
		}
		}
	}
	
	printf("el resultado es: %d \n", result);
	return 0;
	/* COMPLETAR */
    
}