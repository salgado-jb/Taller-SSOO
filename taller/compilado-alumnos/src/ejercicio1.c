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
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void sigterm_handler(int signum) {    
    int rand = generate_random_number();

    if (rand == numero_maldito){
        printf("Soy proceso hijo: %d, mori", getpid());
        exit(0);
    }
}

void sigchld_handler(int signum){
    //ANOTO QUE MURIO EL HIJO
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
    lista_hijos = (int*) malloc(sizeof(int)*n);
    int pid_padre = getpid();
    
    
    // Completar
    for (int hijo_actual = 0; hijo_actual < n; hijo_actual++){
        int hijo_pid;
        hijo_pid = fork();
        if (hijo_pid != 0){
            lista_hijos[hijo_actual] = hijo_pid;
        }
        else{
            break;
        }
    }
    
    if (getpid() == pid_padre){
        signal(SIGCHLD, sigchld_handler);
        for (int i = 0; i< rondas; i++){
            for (int hijo_actual = 0; hijo_actual < n; hijo_actual++){
                kill(lista_hijos[hijo_actual], SIGTERM);
                sleep(1);
            }
        }
    }
    else{
        signal(SIGTERM, sigterm_handler);
        while (1){}
    }
    
    
    if (getpid() == pid_padre){
        printf("Los hijos ganadores:\n");
        for(int i = 0; i < n; i++){
            if(lista_hijos[i] != -1){
                printf("El hijo numero %d, de pid: %d gano\n", i+1, lista_hijos[i]);
                kill(lista_hijos[i], SIGKILL);
            }
        }
    }
    exit(0);
}
