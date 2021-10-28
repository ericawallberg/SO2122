#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "info.h"

int open_BCpipe(){
    char BCpipe[PATH_MAX];
    int BCpipe_fd;

    sprintf(BCpipe, "%s%s", PIPE_DIRECTORY, BALCAO_PIPE_NAME);
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
    int BCpipe_fd;
    int nbytes_read;
    int nbytes_write;
    int clientpipe_fd;
    pedidoCB pedidoCB;
    respostaBC respostaBC;

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
            fprintf(stderr,"[BALCAO] Unexpected number of bytes written <%d/%d>. Ignoring it.\n", nbytes_write, sizeof(respostaBC));
        }
    }

}

void myAbort(const char *msg, int exit_status){
    perror(msg); exit(exit_status);
}