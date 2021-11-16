#include <stdio.h>
#include "utilities.h"

int salva_estado_jogo(ESTADO estado);
int le_jogo_salvo(ESTADO *estado, char nome_usuario[TAM]);
int abre_arq_jog(FILE **arqtxt);
int le_arq_texto(FILE *arqtxt, JOGADOR *lista_jogadores);
int salva_lista_jogadores(FILE *arq, JOGADOR *lista_jogadores, int tam);
