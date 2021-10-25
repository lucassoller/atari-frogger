#include <stdio.h>
#include "utilities.h"

int salva_estado_jogo(ESTADO estado, FILE *arq);
int le_jogo_salvo(ESTADO *estado, char nome_usuario[TAM], FILE *arq);
