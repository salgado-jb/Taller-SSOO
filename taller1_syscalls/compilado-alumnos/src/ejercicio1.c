#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
int rondas;
int* lista_hijos;

int generate_random_number(){
    //Función para cargar nueva semilla para el número aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void sigterm_handler(int signum) {    
    int rand = generate_random_number();
    if (rand == numero_maldito){
        printf("Soy proceso hijo: %d, mori\n", getpid());
        exit(0);
    }
    //Si no le tocó el numero maldito, vuelve al while.
}

void sigchld_handler(int signum){
    //Anoto que murió el hijo
    int pid_del_hijo_que_murio = wait(NULL);
    for (int i = 0; i < n; i++){
        if (lista_hijos[i] == pid_del_hijo_que_murio){
            lista_hijos[i] = -1;
        }
    }
    return;
}

int main(int argc, char const *argv[]){
    
    n = atoi(argv[1]);
	rondas = atoi(argv[2]);
	numero_maldito = atoi(argv[3]);
    
    // Completar
    lista_hijos = (int*) malloc(sizeof(int)*n);

    for (int hijo_actual = 0; hijo_actual < n; hijo_actual++){
        int hijo_pid;
        hijo_pid = fork();
        if (hijo_pid != 0){
            lista_hijos[hijo_actual] = hijo_pid;
        }
        else{//Los hijos no salen de acá
            signal(SIGTERM, sigterm_handler);
            while (1){}
        }
    }

    //Solo el padre llega a este punto
    signal(SIGCHLD, sigchld_handler);
    for (int i = 0; i< rondas; i++){
        for (int hijo_actual = 0; hijo_actual < n; hijo_actual++){
            if (lista_hijos[hijo_actual] != -1){
                kill(lista_hijos[hijo_actual], SIGTERM);
                sleep(1);
            }
        }
    }

    printf("Los hijos ganadores:\n");
    for(int i = 0; i < n; i++){
        if(lista_hijos[i] != -1){
            printf("El hijo numero %d, de pid: %d gano\n", i+1, lista_hijos[i]);
            kill(lista_hijos[i], SIGKILL);
        }
    }
    
    exit(0);
}

/*
Al final no hacía falta guardar el pid del padre porque los usabamos para dos ifs al final
que veían si el proceso actual es el padre, pero como los hijos se quedaron atrapados en
un while anterior, el padre es el único que llega a ese punto.
*/