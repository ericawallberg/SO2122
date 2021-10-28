#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "info.h"


int main(int argc, char **argv){
    char nome[PATH_MAX];
    pedidoCB pedidoCB;

    //verificar se balcao existe

    //verifica nome
    if(argc==2){
        strcpy(nome, argv[1]);
        printf("%s", nome);
    }
    else{
        printf("Numero de argumentos invalido. Indique nome");
        return 0;
    }

    char sintomas[PATH_MAX];
    printf("Indique sintomas:\n");
    scanf("%[^\n]s", sintomas);

    sprintf(pedidoCB.nomepipe,"%s%s%s",PIPE_DIRECTORY,CLIENT_BC_NAME_PATTERN,getpid());
    strcpy(pedidoCB.sintomas,sintomas);
    strcpy(pedidoCB.nome,nome);
    

}