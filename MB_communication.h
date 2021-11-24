#ifndef MB_COMMUNICATION_H
#define MB_COMMUNICATION_H
#include <linux/limits.h>
#define PIPE_DIRECTORY "/tmp/"   //dirtoria para ficheiros temporários
#define BM_PIPE_NAME_PATTERN "BM-%d.pipe"
#define MB_PIPE_NAME "medico-balcao.pipe"

typedef struct pedidoMB{
    char nomepipe[PATH_MAX];
    char nome[PATH_MAX];
    char especialidade[PATH_MAX];
} pedidoMB;

typedef struct respostaBM{
    char resposta[PATH_MAX];
}respostaMB;

void myAbort(const char *msg, int exit_status);

#endif
