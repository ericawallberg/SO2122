#ifndef BALCAO_H
#define BALCAO_H
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#define MAXCLIENTES 10
#define MAXMEDICOS  10
#define NUMTHREADS 2
#define FILAESPERA 5

typedef struct cliente{
    char nomepipe[PATH_MAX];
    char nome[PATH_MAX];
    char sintomas[PATH_MAX];
} cliente;

typedef struct medico{
    char nomepipe[PATH_MAX];
    char nome[PATH_MAX];
    char especialidade[PATH_MAX];
}medico;

typedef struct settings{
    int maxclientes, maxmedicos;
}settings;

typedef struct{
    settings settings;
    pthread_mutex_t arrayclientes_mutex;
}TCLIENTES;

//criar possivel storage.h
cliente* adiciona_cliente(cliente* tab, int* n, cliente* novo);
cliente* elimina_cliente(cliente* tab, int*n, char *nomepipe);
void mostra_clientes(cliente* tab, int* n);
medico* adiciona_medico(medico* tab, int* n, medico* novo);
medico* elimina_medico(medico* tab, int*n, char *nomepipe);
void mostra_medicos(medico* tab, int* n);

//handlers
void *clientes_thread_handler(void* arg);
void *medicos_thread_handler(void* arg);
void termina_clientes_handler(int s);


//pipes
int open_CBpipe();
#endif