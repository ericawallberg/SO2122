#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "MB_communication.h"

int open_balcaopipe(void);
int open_medicopipe(char *medicopipe);
void catch_sigint(int signo){
    if(signo==SIGINT){
        //cleanup();  //ve se o pipe está aberto, fecha-o e limpa-lo do filesystem
        signal(signo,SIG_DFL); //sig default
        kill(getpid(),signo); //vou enviar-me um sigint e ter um comportamento default que é terminar
    }
}

int main(int argc, char **argv){
    char medicopipe[PATH_MAX], balcaopipe[PATH_MAX];
    char nome[PATH_MAX], especialidade[PATH_MAX];
    int balcaopipe_fd, medicopipe_fd;
    

    sprintf(medicopipe,"%s%s",PIPE_DIRECTORY,BM_PIPE_NAME_PATTERN);
    sprintf(medicopipe,medicopipe,getpid());

    //verificar se balcao existe
    balcaopipe_fd = open_balcaopipe();
    medicopipe_fd = open_clientpipe(medicopipe);

    //Ignores the SIGPIPE signal forcing the write() system call to return -1 instead of terminating the process
    signal(SIGPIPE,SIG_IGN);
    //handle ^C SIGINT to perform final cleanup
    signal(SIGINT, catch_sigint);

    //verifica nome
    if(argc==3){
        strcpy(nome, argv[1]);
        printf("%s\n", nome);
        strcpy(especialidade, argv[2]);
        printf("%s\n", especialidade);
    }
    else{
        printf("Numero de argumentos invalido. Indique nome e especialidade.\n");
        return 0;
    }


}

int open_balcaopipe(){
    int balcaopipe_fd;
    char balcaopipe[PATH_MAX];

    sprintf(balcaopipe,"%s%s", PIPE_DIRECTORY,MB_PIPE_NAME);
    fprintf(stdout, "[MEDICO] Waiting for the server to open its pipe.\n");

    if(access(balcaopipe,F_OK)==-1)
        myAbort("[MEDICO] The server pipe doesn't exist. Probably the server is not running.\n",EXIT_FAILURE);
    

    if((balcaopipe_fd = open(balcaopipe, O_WRONLY))==-1){
        myAbort("[MEDICO] Error while opening the server pipe!\n",EXIT_FAILURE);
        //fazer MEDICO, queres esperar pelo balcao? sim/nao. se nao termina
    }

    return balcaopipe_fd;
}

int open_medicopipe(char *medicopipe){
    int medicopipe_fd;
  
    fprintf(stdout, "[MEDICO] Attempt to create the client pipe <%s>.\n", medicopipe);
    if(access(medicopipe,F_OK)==-1){
        if(mkfifo(medicopipe, S_IRUSR | S_IWUSR)!=0)
            myAbort("[MEDICO] Error while creating the client pipe!.\n",EXIT_FAILURE);
    }else{
        myAbort("[MEDICO] The client pipe already exists. Unable to move forward\n",EXIT_FAILURE);
    }

    fprintf(stdout, "[MEDICO] Attempt to open the client pipe <%s>.\n", medicopipe);
    if((medicopipe_fd = open(medicopipe, O_RDWR))==-1){
        myAbort("[MEDICO] Error while opening the client pipe!\n",EXIT_FAILURE);
        //fazer MEDICO, queres esperar pelo balcao? sim/nao. se nao termina
    }

    return medicopipe_fd;
}
