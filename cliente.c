#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "info.h"

int open_balcaopipe(void);
int open_clientpipe(char *clientpipe);
void cleanup(void) __attribute__ ((destructor));
void catch_sigint(int signo){
    if(signo==SIGINT){
        //cleanup();  //ve se o pipe está aberto, fecha-o e limpa-lo do filesystem
        signal(signo,SIG_DFL); //sig default
        kill(getpid(),signo); //vou enviar-me um sigint e ter um comportamento default que é terminar
    }
}

int main(int argc, char **argv){
    char nome[PATH_MAX],balcaopipe[PATH_MAX];
    int balcaopipe_fd; int clientpipe_fd;
    int nbytes_read, nbytes_write;
    pedidoCB pedidoCB; respostaBC respostaBC;
    char clientpipe[PATH_MAX];

    sprintf(clientpipe,"%s%s",PIPE_DIRECTORY,CLIENT_BC_NAME_PATTERN);
    sprintf(clientpipe,clientpipe,getpid());

    //verificar se balcao existe
    balcaopipe_fd = open_balcaopipe();
    clientpipe_fd = open_clientpipe(clientpipe);

    //Ignores the SIGPIPE signal forcing the write() system call to return -1 instead of terminating the process
    signal(SIGPIPE,SIG_IGN);
    //handle ^C SIGINT to perform final cleanup
    signal(SIGINT, catch_sigint);

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

    strcpy(pedidoCB.nomepipe,clientpipe);
    strcpy(pedidoCB.sintomas,sintomas);
    strcpy(pedidoCB.nome,nome);

    fprintf(stdout, "[CLIENTE] Sendind pedidoCB a balcao.\n");
    nbytes_write = write(balcaopipe_fd, &pedidoCB, sizeof(pedidoCB));
    if(nbytes_write == -1){
        myAbort("[CLIENTE] Error while writing to the server pipe!", EXIT_FAILURE);
    } else if(nbytes_write != sizeof(pedidoCB)){
        fprintf(stdout, "[CLIENTE] Unexpected number of bytes written <%d/%ld>. Discarding this operation!\n", nbytes_write, sizeof(pedidoCB));
    }
    fprintf(stdout, "[CLIENTE] The request was successfuly sent to balcao <%d/%ld>.\n", nbytes_write, sizeof(pedidoCB));

    nbytes_read = read(clientpipe_fd, &respostaBC, sizeof(respostaBC));
    if(nbytes_read == -1){
        myAbort("[CLIENTE] Error while writing to the server pipe!", EXIT_FAILURE);
    } else if(nbytes_read != sizeof(respostaBC)){
        fprintf(stdout, "[CLIENTE] Unexpected number of bytes written <%d/%ld>. Discarding this operation!\n", nbytes_write, sizeof(respostaBC));
    }
    fprintf(stdout, "[CLIENTE] The request was successfuly sent to balcao <%d/%ld>.\n", nbytes_write, sizeof(respostaBC));

    fprintf(stdout, "[CLIENTE] resposta do balcao: <%s>\n", respostaBC.resposta);

    
}


int open_balcaopipe(){
    int balcaopipe_fd;
    char balcaopipe[PATH_MAX];

    sprintf(balcaopipe,"%s%s", PIPE_DIRECTORY,CLIENTE_BALCAO_PIPE_NAME);
    fprintf(stdout, "[CLIENTE] Waiting for the server to open its pipe.\n");

    if(access(balcaopipe,F_OK)==-1)
        myAbort("[CLIENTE] The server pipe doesn't exist. Probably the server is not running.\n",EXIT_FAILURE);
    

    if((balcaopipe_fd = open(balcaopipe, O_WRONLY))==-1){
        myAbort("[CLIENTE] Error while opening the server pipe!\n",EXIT_FAILURE);
        //fazer cliente, queres esperar pelo balcao? sim/nao. se nao termina
    }

    return balcaopipe_fd;
}

int open_clientpipe(char *clientpipe){
    int clientpipe_fd;
  
    fprintf(stdout, "[CLIENTE] Attempt to create the client pipe <%s>.\n", clientpipe);
    if(access(clientpipe,F_OK)==-1){
        if(mkfifo(clientpipe, S_IRUSR | S_IWUSR)!=0)
            myAbort("[CLIENTE] Error while creating the client pipe!.\n",EXIT_FAILURE);
    }else{
        myAbort("[CLIENTE] The client pipe already exists. Unable to move forward\n",EXIT_FAILURE);
    }

    fprintf(stdout, "[CLIENTE] Attempt to open the client pipe <%s>.\n", clientpipe);
    if((clientpipe_fd = open(clientpipe, O_RDWR))==-1){
        myAbort("[CLIENTE] Error while opening the client pipe!\n",EXIT_FAILURE);
        //fazer cliente, queres esperar pelo balcao? sim/nao. se nao termina
    }

    return clientpipe_fd;
}

/* void cleanup(void){ //implica usar variaveis globais
    fprintf(stdout, "\nB[CLIENTE] Beginning resources clean up.\n");
    if(clientpipe_fd!=-1){
        close(clientepipe_fd);
        if(unlink(clientpipe_name)==0){
            fprintf(stdout, "[CLIENTE] Client pipe <%s> correctly deleted.\n",clientpipe_name);
        }
    }
} */

void myAbort(const char *msg, int exit_status){
    perror(msg); exit(exit_status);
}
