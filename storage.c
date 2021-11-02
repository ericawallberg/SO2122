#include "balcao.h"

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


medico* adiciona_medico(medico* tab, int*n, medico* novo){
    medico* aux;
    aux = realloc(tab, sizeof(medico)*(*n+1));
    if(aux!=NULL){
        tab=aux;
        strcpy(tab[*n].nomepipe,novo->nomepipe);
        strcpy(tab[*n].nome,novo->nome);
        strcpy(tab[*n].especialidade,novo->especialidade);

        (*n)++;
    }
    return tab;
}

medico* elimina_medico(medico* tab, int*n, char *nomepipe){
    medico *aux,t;
    int i;
    for(i=0;i<*n && strcmp(tab[*n].nomepipe, nomepipe)!=0;i++);
    if(i==*n){
        fprintf(stderr, "[BALCAO] medico a eliminar nao existe\n");
        return tab;
    }
    else if(*n==1){
        free(tab); *n=0; return NULL;
    }
    else{
        t=tab[i];
        tab[i]=tab[*n-1];
        aux=realloc(tab,sizeof(medico)*(*n-1));
        if(aux!=NULL){
            tab=aux;
            (*n)--;
        }
        else tab[i]=t;
        return tab;
    }
}

void mostra_medicos(medico* tab, int*n){
    for(int i=0;i<*n;i++){
        fprintf(stdout, "\n\n----------------");
        fprintf(stdout, "\nNome do Pipe:\t%s", tab[i].nomepipe);
        fprintf(stdout, "\nNome:\t\t%s", tab[i].nome);
        fprintf(stdout, "\nEspecialidade:\t%s", tab[i].especialidade);
        fprintf(stdout, "\n----------------\n");
    }
}