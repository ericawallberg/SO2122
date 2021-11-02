#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "info.h"

typedef struct cliente{
    char nomepipe[PATH_MAX];
    char nome[PATH_MAX];
    char sintomas[PATH_MAX];
} cliente;

cliente* adiciona_cliente(cliente* tab, int* n, cliente* novo);
void mostra_clientes(cliente* tab, int* n);
int open_BCpipe(){
    char BCpipe[PATH_MAX];
    int BCpipe_fd;

    sprintf(BCpipe, "%s%s", PIPE_DIRECTORY, CLIENTE_BALCAO_PIPE_NAME);
    fprintf(stdout, "Balcao is opening its pipe <%s> to receive client requests ... \n", BCpipe);

    //check if named pipe already exists
    if(access(BCpipe,F_OK)==-1){
        if(mkfifo(BCpipe, S_IRUSR | S_IWUSR)!=0)
            myAbort("[BALCAO] Error while opening server pipe!", EXIT_FAILURE);
    }

    if((BCpipe_fd = open(BCpipe,O_RDWR))==-1)   //nao ha situação de broken pipe
        myAbort("[BALCAO] Error while opening server pipe.",EXIT_FAILURE);

    return BCpipe_fd;
}


int main(int argc, char **argv){
    int BCpipe_fd,clientpipe_fd;
    int nbytes_read, nbytes_write; 
    pedidoCB pedidoCB;
    respostaBC respostaBC;
    cliente *clientes = NULL; int clientestotal=0;

    BCpipe_fd = open_BCpipe();

    while(1){
        fprintf(stdout, "[BALCAO] Waiting for client requests ... \n");
        nbytes_read = read(BCpipe_fd,&pedidoCB, sizeof(pedidoCB));
        fprintf(stdout,"[BALCAO] Read returns <%d> bytes.\n", nbytes_read);

        if(nbytes_read == -1){
            myAbort("[BALCAO] Error while reading server pipe!\n", EXIT_FAILURE);
        }else if(nbytes_read != sizeof(pedidoCB)){
            fprintf(stderr, "[BALCAO] Unexpected request size. Ignoring it!\n");
            continue;
        }
        
        cliente novo;
        strcpy(novo.nomepipe,pedidoCB.nomepipe);
        strcpy(novo.nome,pedidoCB.nome);
        strcpy(novo.sintomas,pedidoCB.sintomas);

        clientes = adiciona_cliente(clientes, &clientestotal, &novo);

        mostra_clientes(clientes, &clientestotal);

        strcpy(respostaBC.resposta,"oi");
        fprintf(stdout, "[BALCAO] client pipe name is <%s>.\n", pedidoCB.nomepipe);

        clientpipe_fd = open(pedidoCB.nomepipe, O_WRONLY);
        if(clientpipe_fd == -1){
            fprintf(stderr, "[BALCAO] Error while opening the client pipe! Ignoring it.\n");
            continue;
        }
        nbytes_write= write(clientpipe_fd, &respostaBC, sizeof(respostaBC));
        if(nbytes_write == -1){
            fprintf(stdout, "[BALCAO] Error while writing to the client pipe! Ignoring it.\n");
        }
        else if(nbytes_write!=sizeof(respostaBC)){
            fprintf(stderr,"[BALCAO] Unexpected number of bytes written <%d/%ld>. Ignoring it.\n", nbytes_write, sizeof(respostaBC));
        }
    }

    if(clientes!=NULL)
        free(clientes);

}

cliente* adiciona_cliente(cliente* tab, int* n, cliente* novo){
    cliente* aux;
    aux = realloc(tab, sizeof(cliente)*(*n+1));
    if(aux!=NULL){
        tab=aux;
        strcpy(tab[*n].nomepipe,novo->nomepipe);
        strcpy(tab[*n].nome,novo->nome);
        strcpy(tab[*n].sintomas,novo->sintomas);

        (*n)++;
    }
    return tab;
}

cliente* elimina_cliente(cliente* tab, int*n, char *nomepipe){
    cliente *aux,t;
    int i;
    for(i=0;i<*n && strcmp(tab[*n].nomepipe, nomepipe)!=0;i++);
    if(i==*n){
        fprintf(stderr, "[BALCAO] Cliente a eliminar nao existe\n");
        return tab;
    }
    else if(*n==1){
        free(tab); *n=0; return NULL;
    }
    else{
        t=tab[i];
        tab[i]=tab[*n-1];
        aux=realloc(tab,sizeof(cliente)*(*n-1));
        if(aux!=NULL){
            tab=aux;
            (*n)--;
        }
        else tab[i]=t;
        return tab;
    }
}

void mostra_clientes(cliente* tab, int* n){
    for(int i=0;i<*n;i++){
        fprintf(stdout, "\n\n----------------");
        fprintf(stdout, "\nNome do Pipe:\t%s", tab[i].nomepipe);
        fprintf(stdout, "\nNome:\t\t%s", tab[i].nome);
        fprintf(stdout, "\nSintomas:\t%s", tab[i].sintomas);
        fprintf(stdout, "\n----------------\n");
    }
}

void myAbort(const char *msg, int exit_status){
    perror(msg); exit(exit_status);
}
