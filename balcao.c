
#include "CB_communication.h"
#include "MB_communication.h"
#include "balcao.h"

int open_CBpipe(){
    char CBpipe[PATH_MAX];
    int CBpipe_fd;

    sprintf(CBpipe, "%s%s", PIPE_DIRECTORY, CLIENTE_BALCAO_PIPE_NAME);
    fprintf(stdout, "Balcao is opening its pipe <%s> to receive client requests ... \n", CBpipe);

    //check if named pipe already exists
    if(access(CBpipe,F_OK)==-1){
        if(mkfifo(CBpipe, S_IRUSR | S_IWUSR)!=0)
            myAbort("[BALCAO] Error while opening server pipe!", EXIT_FAILURE);
    }

    if((CBpipe_fd = open(CBpipe,O_RDWR))==-1)   //nao ha situação de broken pipe
        myAbort("[BALCAO] Error while opening server pipe.",EXIT_FAILURE);

    return CBpipe_fd;
}


int main(int argc, char **argv){
    int CBpipe_fd,clientpipe_fd;
    int nbytes_read, nbytes_write; 
    pedidoCB pedidoCB;
    respostaBC respostaBC;
    cliente *clientes = NULL; int clientestotal=0;
    settings settings;
    char* tmp;
    if((tmp = getenv("MAXMEDICOS"))!=NULL){
        settings.maxmedicos = atoi(tmp);
    } else settings.maxmedicos=MAXMEDICOS;
    if((tmp = getenv("MAXCLIENTES"))!=NULL){
        settings.maxclientes = atoi(tmp);
    } else settings.maxclientes=MAXCLIENTES;

    CBpipe_fd = open_CBpipe();

    while(1){
        fprintf(stdout, "[BALCAO] Waiting for client requests ... \n");
        nbytes_read = read(CBpipe_fd,&pedidoCB, sizeof(pedidoCB));
        fprintf(stdout,"[BALCAO] Read returns <%d> bytes.\n", nbytes_read);

        if(nbytes_read == -1){
            myAbort("[BALCAO] Error while reading server pipe!\n", EXIT_FAILURE);
        }else if(nbytes_read != sizeof(pedidoCB)){
            fprintf(stderr, "[BALCAO] Unexpected request size. Ignoring it!\n");
            continue;
        }
        if(clientestotal<settings.maxclientes){

            cliente novo;
            strcpy(novo.nomepipe,pedidoCB.nomepipe);
            strcpy(novo.nome,pedidoCB.nome);
            strcpy(novo.sintomas,pedidoCB.sintomas);

            clientes = adiciona_cliente(clientes, &clientestotal, &novo);

            mostra_clientes(clientes, &clientestotal);



            sprintf(respostaBC.resposta,"Novo cliente adicionado. clientestotal:\t%d\n", clientestotal);

        }
        else strcpy(respostaBC.resposta,"Limite maximo de clientes atingido no servidor.\n");

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


void myAbort(const char *msg, int exit_status){
    perror(msg); exit(exit_status);
}
