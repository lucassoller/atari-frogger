#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

// salva o estado do jogo em um arquivo binario
int salva_estado_jogo(ESTADO estado)
{
    FILE *arq = NULL;
    char nome_aquivo[TAM];
    strcpy(nome_aquivo, estado.jogador.nome);
    strcat(nome_aquivo, ".bin");

    // abre o arquivo
    if((arq = fopen(nome_aquivo, "wb")))
    {
        // escreve no arquivo
        fwrite(&estado, sizeof(ESTADO), 1, arq);
        // fecha o arquivo
        fclose(arq);
        // retorna 0
        return 0;
    }
    // retorna 1 se não foi possivel abrir/criar o arquivo
    return 1;
}

// le os dados do arquivo binario
int le_jogo_salvo(ESTADO *estado, char nome_usuario[])
{
    FILE *arq = NULL;
    char nome_aquivo[TAM];
    strcpy(nome_aquivo, nome_usuario);
    strcat(nome_aquivo, ".bin");

    // abre o arquivo
    if((arq = fopen(nome_aquivo, "rb")))
    {
        // enquanto nao chega no fim do arquivo
        while(feof(arq)== 0)
        {
            // le o arquivo
            fread(estado, sizeof(ESTADO), 1, arq);
        }
        // fecha o arquivo
        fclose(arq);
        // retorna 0
        return 0;
    }
    // retorna 1 se não foi possivel ler o arquivo
    return 1;
}

int abre_arq_jog(FILE **arqtxt)
{
    //ABRE O ARQUIVO PARA LEITURA E ESCRITA
    *arqtxt = fopen(FROGGERJOG, "r+");
    if(!*arqtxt)
    {
         //CASO O ARQUIVO NAO POSSA SER ABERTO CRIA UM NOVO ARQUIVO
         *arqtxt = fopen(FROGGERJOG, "w+");

         //SE O ARQUIVO NAO PODE SER CRIADO, RETORNA 0
         if(!*arqtxt)
         {
            return 0;
         }
    }

    //CASO CONTRARIO RETORNA 1
    return 1;
}

int le_arq_texto(FILE *arqtxt, JOGADOR *lista_jogadores)
{
    char string[TAM_MAX];
    char stringAux[TAM_MAX];
    int i = 0, tamanho;
    //LE ARQUIVO ATÉ EOF OU ATÉ O NÚMERO MAXIMO PERMITIDO
    while(fgets(string, sizeof(string), arqtxt) && i<NUMERO_MAXIMO_RANKING)
    {
        //ATRIBUI DADOS LIDOS DA LINHA LIDA PARA JOGADOR DA LISTA
        strcpy(lista_jogadores[i].nome, strtok(string, "#"));

        //RETIRA ESPACOS EM BRANCO DO FINAL DO NOME
        tamanho = strlen(lista_jogadores[i].nome)-1;

        while(lista_jogadores[i].nome[tamanho] == ' ')
        {
            lista_jogadores[i].nome[tamanho--] = '\0';
        }

        lista_jogadores[i].sapos_espera = atoi(strcpy(stringAux, strtok(NULL, " #")));
        lista_jogadores[i].sapos_salvos = atoi(strcpy(stringAux, strtok(NULL, " #")));
        lista_jogadores[i].tempoJogo = atoi(strcpy(stringAux, strtok(NULL, " #")));
        lista_jogadores[i].score = atoi(strcpy(stringAux, strtok(NULL, " #")));

        i++;
    }
    //RETORNA NUMERO DE JOGADORES
    return i;
}

int salva_lista_jogadores(FILE *arq, JOGADOR *lista_jogadores, int tam)
{
    int i = 0;
    char aux[TAM_MAX-1];

    //VOLTA PONTEIRO PARA INICIO DO ARQUIVO
    rewind(arq);

    for(i = 0; i< tam && i<NUMERO_MAXIMO_RANKING; i++)
    {
        //CRIA STRING NO FORMATO SALVO NO ARQUIVO, CONTUDO ADICIONA ESPACOS PARA TER CERTEZA QUE VAI SOBREESCREVER TODA LINHA
        sprintf(aux, "%s #%d #%d #%d #%d                                  \n\0",lista_jogadores[i].nome, lista_jogadores[i].sapos_espera, lista_jogadores[i].sapos_salvos+lista_jogadores[i].sapos_salvos_f1, lista_jogadores[i].tempoJogo+lista_jogadores[i].tempoJogo_f1, lista_jogadores[i].score);
        fputs(aux, arq);//ADICIONA LINHA NO FILE
    }
    fclose(arq);//FECHA ARQUIVO
    if(ferror(arq))//CASO OCORRA ERRO RETORNA 0
    {
        return 0;
    }
    return 1;//CASO NAO RETORNA 1
}
