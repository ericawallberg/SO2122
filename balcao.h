#ifndef BALCAO_H
#define BALCAO_H
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAXCLIENTES 4
#define MAXMEDICOS  4
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


//criar possivel storage.h
cliente* adiciona_cliente(cliente* tab, int* n, cliente* novo);
cliente* elimina_cliente(cliente* tab, int*n, char *nomepipe);
void mostra_clientes(cliente* tab, int* n);
medico* adiciona_medico(medico* tab, int* n, medico* novo);
medico* elimina_medico(medico* tab, int*n, char *nomepipe);
void mostra_medicos(medico* tab, int* n);



#endif