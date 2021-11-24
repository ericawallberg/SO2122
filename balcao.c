#include "CB_communication.h"
#include "MB_communication.h"
#include "balcao.h"

cliente *clientes = NULL; int clientestotal=0;

int main(int argc, char **argv){
    pthread_t clientes_thread, medicos_thread;
    settings settings;
    TCLIENTES tclientes;
    pthread_mutex_t arrayclientes_mutex;
    char comando[PATH_MAX];
    char* tmp;
    if((tmp = getenv("MAXMEDICOS"))!=NULL){
        settings.maxmedicos = atoi(tmp);
        fprintf(stderr, "Variavel de ambiente MAXMEDICOS utilizada.\n");
    } else{
        settings.maxmedicos=MAXMEDICOS;
        fprintf(stderr, "Variavel de ambiente MAXMEDICOS não utilizada.\n");
    } 
    fprintf(stderr, "MAXMEDICOS=%d\n\n",settings.maxmedicos);
    if((tmp = getenv("MAXCLIENTES"))!=NULL){
        settings.maxclientes = atoi(tmp);
        fprintf(stderr, "Variavel de ambiente MAXCLIENTES utilizada.\n");
    } else{
        settings.maxclientes=MAXCLIENTES;
        fprintf(stderr, "Variavel de ambiente MAXMEDICOS não utilizada.\n");
    }
    fprintf(stderr, "MAXCLIENTES=%d\n",settings.maxclientes);

    pthread_mutex_init(&arrayclientes_mutex,NULL);
    tclientes.settings.maxclientes=settings.maxclientes;
    tclientes.settings.maxmedicos=settings.maxmedicos;
    tclientes.arrayclientes_mutex=arrayclientes_mutex;
    if(pthread_create(&(clientes_thread),NULL,clientes_thread_handler, (void*)&settings)!=0){
        myAbort("[BALCAO] Client THread creation failed",EXIT_FAILURE);
    }
    /*
    if(pthread_create(&(medicos_thread),NULL,medicos_thread_handler, (void*)&settings)!=0){
        myAbort("[BALCAO] Client THread creation failed",EXIT_FAILURE);
    }*/
    /*
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
    }*/
    while(1){
        fflush(stdin);
        fprintf(stderr, "\nIndique comando: ");
        scanf(" %[^\n]s",comando);
        fprintf(stderr, "\n<%s>", comando);
        if(strcmp(comando,"utentes")==0){
            pthread_mutex_lock(&arrayclientes_mutex);
            mostra_clientes(clientes, &clientestotal);
            pthread_mutex_unlock(&arrayclientes_mutex);
        }
        /*else if(strcmp(comando,"especialistas")==0){

        } 
        else if(strncmp(comando,"delut",strlen("delut"))==0){

        } else if(strncmp(comando,"delesp",strlen("delesp"))==0){
            
        } else if(strncmp(comando,"freq",strlen("freq"))==0){
            
        } */
        else if(strcmp(comando,"encerra")==0){
            if(pthread_cancel(clientes_thread)!=0){
                myAbort("Thread cancelation failed", EXIT_FAILURE);
            }
            fprintf(stderr, "\npthread funcionou. ");
            break;
        }
    }
    fprintf(stderr, "\nantes de join ");
    void* retval;
    if(pthread_join(clientes_thread,&retval)!=0){
        myAbort("[BALCAO] pthread_join failed", EXIT_FAILURE);
    }

    fprintf(stderr,"\ndps do join");

    if(clientes!=NULL)
        free(clientes);

    fprintf(stderr,"\nfim mesmo");

}

void *clientes_thread_handler(void* arg){
    TCLIENTES* tclientes = (TCLIENTES*)arg;
    int CBpipe_fd,clientpipe_fd;
    int nbytes_read, nbytes_write; 
    pedidoCB pedidoCB;
    respostaBC respostaBC;
    
    if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL)!=0){
        myAbort("Thread pthread_setcancelstate failed", EXIT_FAILURE);
    }
    if(pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL)!=0){
        myAbort("Thread pthread_setcanceltype failed", EXIT_FAILURE);
    }

    CBpipe_fd = open_CBpipe();

    while(1){
        //fprintf(stdout, "[BALCAO] Waiting for client requests ... \n");
        nbytes_read = read(CBpipe_fd,&pedidoCB, sizeof(pedidoCB));
        //fprintf(stdout,"[BALCAO] Read returns <%d> bytes.\n", nbytes_read);

        if(nbytes_read == -1){
            myAbort("[BALCAO] Error while reading server pipe!\n", EXIT_FAILURE);
        }else if(nbytes_read != sizeof(pedidoCB)){
            fprintf(stderr, "[BALCAO] Unexpected request size. Ignoring it!\n");
            continue;
        }
        pthread_mutex_lock(&tclientes->arrayclientes_mutex);
        if(clientestotal<tclientes->settings.maxclientes){

            cliente novo;
            strcpy(novo.nomepipe,pedidoCB.nomepipe);
            strcpy(novo.nome,pedidoCB.nome);
            strcpy(novo.sintomas,pedidoCB.sintomas);

            clientes = adiciona_cliente(clientes, &clientestotal, &novo);

            //mostra_clientes(clientes, &clientestotal);
            
            sprintf(respostaBC.resposta,"Novo cliente adicionado. clientestotal:\t%d\n", clientestotal);

        }
        else strcpy(respostaBC.resposta,"Limite maximo de clientes atingido no servidor.\n");

        pthread_mutex_unlock(&tclientes->arrayclientes_mutex);
        
        //fprintf(stdout, "[BALCAO] client pipe name is <%s>.\n", pedidoCB.nomepipe);

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

    

}


int open_CBpipe(){
    char CBpipe[PATH_MAX];
    int CBpipe_fd;

    sprintf(CBpipe, "%s%s", PIPE_DIRECTORY, CLIENTE_BALCAO_PIPE_NAME);
    //fprintf(stdout, "Balcao is opening its pipe <%s> to receive client requests ... \n", CBpipe);

    //check if named pipe already exists
    if(access(CBpipe,F_OK)==-1){
        if(mkfifo(CBpipe, S_IRUSR | S_IWUSR)!=0)
            myAbort("[BALCAO] Error while opening server pipe!", EXIT_FAILURE);
    }

    if((CBpipe_fd = open(CBpipe,O_RDWR))==-1)   //nao ha situação de broken pipe
        myAbort("[BALCAO] Error while opening server pipe.",EXIT_FAILURE);

    return CBpipe_fd;
}

void myAbort(const char *msg, int exit_status){
    perror(msg); exit(exit_status);
}
