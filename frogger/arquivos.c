#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

// salva o estado do jogo em um arquivo binario
int salva_estado_jogo(ESTADO estado)
{
    FILE *arq = NULL;
    // abre o arquivo
    if((arq = fopen(estado.jogador.nome, "wb")))
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
int le_jogo_salvo(ESTADO *estado, char nome_usuario[TAM])
{
    FILE *arq = NULL;
    // abre o arquivo
    if((arq = fopen(nome_usuario, "rb")))
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
