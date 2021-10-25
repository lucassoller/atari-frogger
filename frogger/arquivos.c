#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

int salva_estado_jogo(ESTADO estado, FILE *arq)
{
    if((arq = fopen(estado.jogador.nome, "wb")))
    {
        fwrite(&estado, sizeof(ESTADO), 1, arq);
        fclose(arq);
        return 0;
    }
    return 1;
}

int le_jogo_salvo(ESTADO *estado, char nome_usuario[TAM], FILE *arq)
{
    if((arq = fopen(nome_usuario, "rb")))
    {
        while(feof(arq)== 0)
        {
            fread(estado, sizeof(ESTADO), 1, arq);
        }
        fclose(arq);
        return 0;
    }
    return 1;
}
