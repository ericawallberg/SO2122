#ifndef INFO_H
#define INFO_H
#include <linux/limits.h>
#define PIPE_DIRECTORY "/tmp"   //dirtoria para ficheiros temporários
#define CLIENT_BC_NAME_PATTERN "BC-%d.pipe"
#define BALCAO_PIPE_NAME "balcao.pipe"

typedef struct pedidoCB{
    char nomepipe[PATH_MAX];
    char nome[PATH_MAX];
    char sintomas[PATH_MAX];
} pedidoCB;

typedef struct respostaBC{
    char resposta[PATH_MAX];
}respostaBC;


#endif