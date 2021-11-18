#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>
#include "arquivos.h"

// protótipos das funções do jogo
void hidecursor();
void desenha_moldura();
void inicia_jogo();

void inicializa_fase(FASE *fase);
void fase_dois(FASE *fase, JOGADOR *jogador, SAPO *lista_sapos, int *indice_sapo);
void altera_jogador_fase_dois(FASE fase, JOGADOR *jogador);
void altera_sapo_fase_dois(FASE fase, SAPO *lista_sapos, int *indice_sapo);
void mostra_mensagem(int mensagem);
void inicializa_sapos(SAPO *lista_sapos, int num_sapos, COLORS cor);
void plota_sapo(COORDENADA coordenada, COLORS cor);
void move_sapo(SAPO *lista_sapos, int *indice_sapo, JOGADOR *jogador, FASE *fase, char *tecla);
int mata_sapo(SAPO *lista_sapos, VEICULO *lista_veiculos, int *indice_sapo, char *tecla, FASE fase);
void salva_sapo(SAPO *lista_sapos, int *indice_sapo, JOGADOR *jogador, FASE *fase, char *tecla);
void testa_sapo(COORDENADA *coordenadas, char tecla, int *chegou_no_topo);
short testa_colisao(SAPO sapo, VEICULO veiculo);
short testa_colisoes(SAPO sapo, VEICULO lista_veiculos[]);
void explode_sapo(COORDENADA coordenada);
void apaga_fogo(COORDENADA coordenada);
void redesenha_inativos(int indice_sapo, COLORS cor, int num_sapos);
void desenha_sapos_inativos(int indice_sapo, COLORS cor, int num_sapos);
void desenha_sapo_salvo(int sapos_salvos, COLORS cor);
void desenha_sapos_salvos(int sapos_salvos, COLORS cor);
void inicializa_jogador(JOGADOR *jogador);
void mostra_cronometro(JOGADOR jogador);
void pausa_cronometro(JOGADOR *jogador);
void despausa_cronometro(JOGADOR *jogador);
void calcula_score(JOGADOR *jogador);
void encerra_jogo(JOGADOR jogador);
void inicializa_veiculos(VEICULO *lista_veiculos, DIRECAO_MOVIMENTO direcao, int y_inicial);
void testa_veiculo_unitario(VEICULO *lista_veiculos, int posicao);
void desenha_veiculo(VEICULO veiculo, COLORS cor);
void desenha_lista_veiculos(VEICULO *lista_veiculos);
void apaga_veiculos(VEICULO *lista_veiculos);
void pausa_jogo(JOGADOR *jogador, SAPO lista_sapos[], VEICULO lista_veiculos1[], VEICULO lista_veiculos2[], FASE fase);
void instancia_jogo(ESTADO estado, JOGADOR *jogador, SAPO *lista_sapos, VEICULO *lista_veiculos1, VEICULO *lista_veiculos2, int *indice_sapo, FASE *fase);
void escreve_header();
void abre_arquivo(JOGADOR jogador, int fim_jogo);
void ordena_lista_jogadores(JOGADOR jog, JOGADOR *lista_jogadores, int tam);
void insere_jogador(JOGADOR jog, JOGADOR *lista_jogadores, int *tam);
void mostra_lista_jogadores(JOGADOR *lista_jogadores, JOGADOR jogador_atual, int tam);
int calcula_score_auxiliar(JOGADOR jogador);
void escreve_info_jogador(JOGADOR jogador);
void inicializa_fase(FASE *fase);
void inicializa_fase(FASE *fase);

// funcao principal
int main()
{
    srand(time(0));
    hidecursor();
    inicia_jogo();
    return 0;
}

// apaga o cursor do teclado
void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = SIZE;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// desenha a moldura com *
void desenha_moldura()
{
    struct text_info info;
    int i;

    inittextinfo();
    gettextinfo( &info );

    textcolor( CYAN );

    for(i = XINI; i <= XFIM; i++)
    {
        putchxy(i, YINI, '*' );
        putchxy(i, YFIM, '*' );
    }

    for (i = YINI+1; i < YFIM; i++)
    {
        putchxy(XINI, i, '*' );
        putchxy(XFIM, i, '*' );
    }
}

// função do gameloop
void inicia_jogo()
{
    // declaração de variáveis
    int indice_sapo = 0;
    int colide;
    char tecla = 'x';

    VEICULO lista_veiculos1[NUM_VEICULOS];
    VEICULO lista_veiculos2[NUM_VEICULOS];
    VEICULO lista_veiculos3[NUM_VEICULOS];
    VEICULO lista_veiculos4[NUM_VEICULOS];

    JOGADOR jogador;
    SAPO lista_sapos[NUM_SAPOS_F2];

    FASE fase;
    ESTADO estado;

    // inicializa valores
    inicializa_veiculos(lista_veiculos1, DIR, Y_PISTA1);
    inicializa_veiculos(lista_veiculos2, DIR, Y_PISTA2);
    //inicializa_veiculos(lista_veiculos3, ESQ, Y_PISTA3);
   // inicializa_veiculos(lista_veiculos4, ESQ, Y_PISTA4);
    inicializa_fase(&fase);
    inicializa_sapos(lista_sapos, fase.num_sapos, fase.cor_sapo);
    inicializa_jogador(&jogador);

    // desenha a moldura do jogo;
    desenha_moldura();

    // escreve mensagens acima da moldura
    escreve_header();

    // plota os sapos e sapos inativos na tela
    plota_sapo(lista_sapos[indice_sapo].envelope[0], lista_sapos[indice_sapo].cor);
    desenha_sapos_inativos(indice_sapo, WHITE, NUM_SAPOS);

    //mostra_mensagem(MSG_INICIO);

    do
    {
        // desenha as listas de veiculos no jogo
        desenha_lista_veiculos(lista_veiculos1);
        desenha_lista_veiculos(lista_veiculos2);
        //desenha_lista_veiculos(lista_veiculos3);
        //desenha_lista_veiculos(lista_veiculos4);

        mostra_cronometro(jogador);

        // testa a colisao entre o sapo e todos os veiculos da lista1
        colide = mata_sapo(lista_sapos, lista_veiculos1, &indice_sapo, &tecla, fase);

        // se não colidiu, testa a colisao entre o sapo e todos os veiculos da lista2
        if(colide == 0){
            colide = mata_sapo(lista_sapos, lista_veiculos2, &indice_sapo, &tecla, fase);
        }

        // testa se o usuario pressionou alguma tecla
        if(kbhit())
        {
            // ler tecla pressionada pelo jogador
            tecla = getch();

            // se foi pressionada uma tecla do teclado auxiliar
            if(tecla == T_ESPECIAL)
            {
                // move o sapo pelo jogo
                move_sapo(lista_sapos, &indice_sapo, &jogador, &fase, &tecla);

            // se foi pressionada a tecla P maiuscula ou minuscula, pausa o jogo
            }else if(tecla == T_PMAX || tecla == T_PMIN)
            {
                // salva todos as variaveis do jogo
                pausa_jogo(&jogador, lista_sapos, lista_veiculos1, lista_veiculos2, fase);
                // mostra mensagem de pause
                mostra_mensagem(MSG_PAUSE);
                // recalcula o tempo do jogo
                jogador.inicioJogo = time(NULL) - jogador.tempoJogo;
            // se foi pressionada a tecla C maiuscula ou minuscula, carrega o jogo
            }else if(tecla == T_CMAX || tecla == T_CMIN)
            {
                // carrega todos os valores do arquivo binario na estrutura ESTADO se encontrou o arquivo
                if(le_jogo_salvo(&estado, jogador.nome) == 0)
                {
                    // instancia cada valor de ESTADO nas variaveis do jogo
                    instancia_jogo(estado, &jogador, lista_sapos, lista_veiculos1, lista_veiculos2, &indice_sapo, &fase);
                }else
                {
                    // mostra a mensagem que não foi encontrado o arquivo
                    mostra_mensagem(MSG_SEM_ARQUIVO);
                }
            }else if(tecla == T_RMAX || tecla == T_RMIN)
            {
                calcula_score(&jogador);
                abre_arquivo(jogador, 0);
                getch();
                clrscr();

                desenha_moldura();
                escreve_header();
                desenha_sapos_inativos(indice_sapo, WHITE, NUM_SAPOS);
                desenha_sapos_salvos(jogador.sapos_salvos, BROWN);
                plota_sapo(lista_sapos[indice_sapo].envelope[0], lista_sapos[indice_sapo].cor);
            }
        }
    }
    // enquanto a tecla pressionada não for ESC
    while(tecla != T_ESC);

    textcolor(WHITE);
    // armazena nome do jogador, calcula escore
    calcula_score(&jogador);
    // mostra mensagem final do resultado
    encerra_jogo(jogador);
}

// escreve as mensagens de header e sapos salvos
void escreve_header()
{
    textcolor(YELLOW);
    cputsxy(XINI, YINI-1, "\tINF1202 Lucas & Joao \t(C)arregar jogo \t(P)ausar \t(ESC)Sair \t(R)anking");
    // sapos salvos
    textcolor(WHITE);
    cputsxy(XFIM+2, YINI, "Sapos salvos:");
    textcolor(BROWN);
    cputsxy(XFIM+2, YINI+2, "Nenhum");
}

// inicializa os valores da fase 1
void inicializa_fase(FASE *fase)
{
    fase->fase = 1;
    fase->distancia_min = DISTANCIA_MINIMA_F1;
    fase->distancia_max = DISTANCIA_MAXIMA_F1;
    fase->velocidade_min = VELOCIDADE_MIN_F1;
    fase->velocidade_max = VELOCIDADE_MAX_F1;
    fase->num_sapos = NUM_SAPOS;
    fase->cor_sapo = GREEN;
}

// inicializa os valores da fase 2
void fase_dois(FASE *fase, JOGADOR *jogador, SAPO *lista_sapos, int *indice_sapo)
{
    fase->fase = 2;
    fase->distancia_min = DISTANCIA_MINIMA_F2;
    fase->distancia_max = DISTANCIA_MAXIMA_F2;
    fase->velocidade_min = VELOCIDADE_MIN_F2;
    fase->velocidade_max = VELOCIDADE_MAX_F2;
    fase->num_sapos = NUM_SAPOS_F2;
    fase->cor_sapo = MAGENTA;

    altera_jogador_fase_dois(*fase, jogador);
    altera_sapo_fase_dois(*fase, lista_sapos, indice_sapo);
    desenha_moldura();
    escreve_header();
}

// altera os valores do jogador para iniciar a fase 2
void altera_jogador_fase_dois(FASE fase, JOGADOR *jogador)
{
    calcula_score(jogador);
    jogador->sapos_espera = fase.num_sapos - 1;
    jogador->sapos_salvos_f1 = jogador->sapos_salvos;
    jogador->sapos_salvos = 0;
    jogador->inicioJogo = time(NULL);
    jogador->tempoJogo_f1 = jogador->tempoJogo;
    jogador->tempoJogo = 0;
}

// altera os valores do sapo para iniciar a fase 2
void altera_sapo_fase_dois(FASE fase, SAPO *lista_sapos, int *indice_sapo)
{
    *indice_sapo = 0;
    inicializa_sapos(lista_sapos, fase.num_sapos, fase.cor_sapo);
    plota_sapo(lista_sapos[*indice_sapo].envelope[0], lista_sapos[*indice_sapo].cor);
    desenha_sapos_inativos(*indice_sapo, WHITE, fase.num_sapos);
}

// inicializa todos os sapos da lista
void inicializa_sapos(SAPO *lista_sapos, int num_sapos, COLORS cor)
{
    int i;
    STATUS_SAPO status_sapo;
    SAPO sapo;

    for(i = 0; i<num_sapos; i++)
    {
        // primeiro sapo fica com status ativo
        if(i == 0)
        {
            status_sapo = ATIVO;
        }
        // outros sapos ficam com status em espera
        else
        {
            status_sapo = ESPERA;
        }

        // atribui todos valores correspondentes ao sapo
        sapo.envelope[0].x = X1_SAPO;
        sapo.envelope[0].y = Y1_SAPO;
        sapo.envelope[1].x = X2_SAPO;
        sapo.envelope[1].y = Y2_SAPO;
        sapo.status = status_sapo;
        sapo.cor = cor;
        sapo.fase = 1;
        // adiciona o sapo na lista de sapos
        lista_sapos[i] = sapo;
    }
}

// inicializa as coordenadas da lista de veiculos na tela
void inicializa_veiculos(VEICULO *lista_veiculos, DIRECAO_MOVIMENTO direcao, int y_inicial)
{
    // coordenadas iniciais do primeiro veiculo
    int i, xAux = XINI+1, velocidade;

    // apaga todos os veiculos para reposicionar
    apaga_veiculos(lista_veiculos);
    // velocidade da pista
    //velocidade = VELOCIDADE_MIN_F1 + (rand() % VELOCIDADE_MAX_F1);
    velocidade = 1;

    for(i=0; i<NUM_VEICULOS; i++)
    {
        lista_veiculos[i].velocidade = velocidade;
        lista_veiculos[i].tamanhoEmX = 3;
        lista_veiculos[i].tamanhoEmY = 2;

        lista_veiculos[i].tipo = rand() % 3;//randomiza o tipo de veiculo
        lista_veiculos[i].dir = direcao;
        xAux += DISTANCIA_MINIMA_F1 +(rand() % DISTANCIA_MAXIMA_F1); //randomiza distancias entre veiculos

        lista_veiculos[i].envelope[0].x = xAux;
        lista_veiculos[i].envelope[0].y = y_inicial;
        lista_veiculos[i].envelope[1].x = xAux + lista_veiculos[i].tamanhoEmX;
        lista_veiculos[i].envelope[1].y = y_inicial + lista_veiculos[i].tamanhoEmY;
        // armazena as coordenadas do veiculo
    }
}

// reiniciliza os veiculos da pista, mantendo a sua direcao e y_pista
void reinicializa_veiculos(VEICULO *lista_veiculos)
{
    inicializa_veiculos(lista_veiculos, lista_veiculos[0].dir, lista_veiculos[0].envelope[0].y);
}

// desenha cada veiculo na tela
void desenha_veiculo(VEICULO veiculo, COLORS cor)
{
    int xc1 = veiculo.envelope[0].x, yc1 = veiculo.envelope[0].y;
    int tipo = veiculo.tipo;
    int direcao = veiculo.dir;

    textcolor(cor);

    //REALIZA UM SWITCH, CONFORME PEDIDO E MUDA O TEMPLATE DO VEICULO
    //A SER PLOTADO
    switch(tipo)
    {
    case ESPORTE:
        //caso o x inicial do veiculo não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do veiculo
        if(xc1 < XFIM && xc1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1, yc1, 'o');
                putchxy(xc1, yc1+1, 'H');
                putchxy(xc1, yc1+2, 'o');
            }
            else if(direcao == ESQ)
            {
                putchxy(xc1, yc1+1, '<');
            }

        }
        //caso as secoes posteriores nao estejam na area do jogo nao tem problema, pois
        //mesmo assim o veiculo ja esta parcialmente na area do jogo,
        if(xc1 +1 < XFIM && xc1 + 1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 + 1, yc1+1, 'H');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1+1, yc1, 'o');
                putchxy(xc1+1, yc1+1, 'H');
                putchxy(xc1+1, yc1+2, 'o');
            }

        }
        if(xc1 + 2 < XFIM && xc1+2 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 +2, yc1, 'o');
                putchxy(xc1 +2, yc1+1, 'H');
                putchxy(xc1 +2, yc1+2, 'o');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1 + 2, yc1+1, 'H');
            }

        }

        if(xc1 + 3 < XFIM && xc1 +3> XINI)
        {
            if(direcao==DIR)
            {
                putchxy(xc1 + 3, yc1+1, '>');
            }
            else if(direcao==ESQ)
            {
                putchxy(xc1 +3, yc1, 'o');
                putchxy(xc1 +3, yc1+1, 'H');
                putchxy(xc1 +3, yc1+2, 'o');
            }
        }
        break;
    case ONIBUS:
        //caso o x inicial do veiculo não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do veiculo
        if(xc1 < XFIM && xc1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1, yc1+1, '-');
            }
            else if(direcao == ESQ)
            {
                putchxy(xc1, yc1+1, '<');
            }

        }
        //caso as secoes posteriores nao estejam na area do jogo nao tem problema, pois
        //mesmo assim o veiculo ja esta parcialmente na area do jogo,
        if(xc1 +1 < XFIM && xc1 + 1 > XINI)
        {
            putchxy(xc1+1, yc1, 'O');
            putchxy(xc1+1, yc1+1,'H');
            putchxy(xc1+1, yc1+2, 'O');

        }
        if(xc1 + 2 < XFIM && xc1+2 > XINI)
        {
            putchxy(xc1+2, yc1, 'O');
            putchxy(xc1+2, yc1+1, 'H');
            putchxy(xc1+2, yc1+2, 'O');
        }

        if(xc1 + 3 < XFIM && xc1 +3> XINI)
        {
            putchxy(xc1+3, yc1+1, 'H');
        }

        if(xc1 + 4 < XFIM && xc1 +4> XINI)
        {
            putchxy(xc1+4, yc1+1, 'H');
        }

        if(xc1 + 5 < XFIM && xc1 +5> XINI)
        {
            putchxy(xc1+5, yc1, 'O');
            putchxy(xc1+5, yc1+1, 'H');
            putchxy(xc1+5, yc1+2, 'O');
        }

        if(xc1 + 6 < XFIM && xc1 +6> XINI)
        {
            putchxy(xc1+6, yc1, 'O');
            putchxy(xc1+6, yc1+1, 'H');
            putchxy(xc1+6, yc1+2, 'O');
        }

        if(xc1 + 7 < XFIM && xc1 +7> XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 + 7, yc1+1, '>');
            }
            else if(direcao == ESQ)
            {
                putchxy(xc1 +7, yc1+1, '-');
            }
        }
        break;
    case SEDAN:
        //caso o x inicial do veiculo não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do veiculo
        if(xc1 < XFIM && xc1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1, yc1+1, '=');
            }
            else if(direcao == ESQ)
            {
                putchxy(xc1, yc1+1, '[');
            }

        }
        //caso as secoes posteriores nao estejam na area do jogo nao tem problema, pois
        //mesmo assim o veiculo ja esta parcialmente na area do jogo,
        if(xc1 +1 < XFIM && xc1 + 1 > XINI)
        {
            putchxy(xc1+1, yc1, 'O');
            putchxy(xc1+1, yc1+1,'|');
            putchxy(xc1+1, yc1+2, 'O');

        }
        if(xc1 + 2 < XFIM && xc1+2 > XINI)
        {
            putchxy(xc1+2, yc1+1, '|');
        }

        if(xc1 + 3 < XFIM && xc1 +3> XINI)
        {
            putchxy(xc1+3, yc1+1, '|');
        }

        if(xc1 + 4 < XFIM && xc1 +4> XINI)
        {
            putchxy(xc1+4, yc1, 'O');
            putchxy(xc1+4, yc1+1, '|');
            putchxy(xc1+4, yc1+2, 'O');

        }

        if(xc1 + 5 < XFIM && xc1 +5> XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 +5, yc1+1, ']');
            }
            else if(direcao == ESQ)
            {
                putchxy(xc1 +5, yc1+1, '=');
            }
        }
        break;
    default:
        break;
    }
}

//apaga os veiculos da tela
void apaga_veiculos(VEICULO *lista_veiculos)
{
    int i;
    for(i=0; i<NUM_VEICULOS; i++)
    {
        desenha_veiculo(lista_veiculos[i], BLACK);
    }
}

// testa a movimentacao unitaria do veiculo na tela
void testa_veiculo_unitario(VEICULO *lista_veiculos, int posicao)
{
    int tecla;
    int i;

    tecla = lista_veiculos[posicao].dir;

    if(tecla == T_ESPECIAL)
    {
        // ler a tecla novamente
        tecla = getch();
    }
    switch(tecla)
    {
    case DIR:
        // se nao atingir a parede direita

        for(i = 0; i<=1; i++)
        {
            if(lista_veiculos[posicao].envelope[i].x + lista_veiculos[posicao].velocidade < XFIM)
            {
                // move para direita
                lista_veiculos[posicao].envelope[i].x += lista_veiculos[posicao].velocidade;
            }
            else
            {
                // posicona antes da parede esquerda
                lista_veiculos[posicao].envelope[i].x =XINI-2;
            }
        }

        break;
    case ESQ:
        for(i = 0; i<=1; i++)
        {
            if(lista_veiculos[posicao].envelope[i].x - lista_veiculos[posicao].velocidade > XINI)
            {
                // move para direita
                lista_veiculos[posicao].envelope[i].x -= lista_veiculos[posicao].velocidade;
            }
            else
            {
                // posicona antes da parede esquerda
                lista_veiculos[posicao].envelope[i].x = XFIM+2;
            }
        }
        break;
    }
}

// desenha a lista de veiculos
void desenha_lista_veiculos(VEICULO *lista_veiculos)
{
    int i;
    for(i=0; i<NUM_VEICULOS; i++)
    {
        //PARA CADA VEICULO APAGA A SUA IMAGEM ATUAL
        desenha_veiculo(lista_veiculos[i], BLACK);
        //ATUALIZA SUAS COORDENADAS
        testa_veiculo_unitario(lista_veiculos, i);
        //PLOTA NA NOVA POSICAO COM A COR DESEJADA
        desenha_veiculo(lista_veiculos[i], LIGHTRED);
    }
}

// move o sapo pela tela
void move_sapo(SAPO *lista_sapos, int *indice_sapo, JOGADOR *jogador, FASE *fase,  char *tecla)
{
    int chegou_no_topo = 0;

    // ponteiro *sapo aponta para o endereço do sapo da lista que esta na posição informada
    SAPO *sapo = &lista_sapos[*indice_sapo];

    // apaga o sapo do jogo
    plota_sapo(sapo->envelope[0], BLACK);

    // testa se o movimento é possivel
    testa_sapo(sapo->envelope, *tecla, &chegou_no_topo);

    // se o sapo chegou no topo, testa se ainda tem sapos restantes
    if(chegou_no_topo)
    {
        // verifica se é o ultimo sapo e faz as alteracoes necessarias
        salva_sapo(lista_sapos, indice_sapo, jogador, fase, tecla);
    }
    else
    {
        // plota o sapo no jogo
        plota_sapo(sapo->envelope[0], sapo->cor);
    }
}

// testa a movimentacao do sapo na tela
void testa_sapo(COORDENADA *coordenadas, char tecla, int *chegou_no_topo)
{

    // ler a tecla novamente
    tecla = getch();

    switch(tecla)
    {
    case T_CIMA:
        // se nao atingir a parede de cima
        if(coordenadas[0].y - 1 > YINI)
        {
            // chegou no topo
            if(coordenadas[0].y - 1 == YINI + 1)
            {
                *chegou_no_topo = 1;
            }
            // move para cima
            coordenadas[0].y--;
            coordenadas[1].y--;
        }
        break;
    case T_BAIXO:
        // se nao atingir a parede de baixo
        if(coordenadas[1].y + 1 < YFIM)
        {
            // move para baixo
            coordenadas[0].y++;
            coordenadas[1].y++;
        }
        break;
    case T_DIREITA:
        // se nao atingir a parede direita
        if(coordenadas[1].x + 1 < XFIM)
        {
            // move para direita
            coordenadas[0].x++;
            coordenadas[1].x++;
        }
        break;
    case T_ESQUERDA:
        // se nao atingir a parede esquerda
        if(coordenadas[0].x - 1 > XINI)
        {
            // move para esquerda
            coordenadas[0].x--;
            coordenadas[1].x--;
        }
        break;
    }
}

// desenha o sapo na tela
void plota_sapo(COORDENADA coordenada, COLORS cor)
{
    textcolor(cor);
    cputsxy(coordenada.x, coordenada.y,    "  (00)  ");
    cputsxy(coordenada.x, coordenada.y+1, "\\^{  }^/");
    textcolor(WHITE);
}

// testa se o sapo chegou no topo, mostra mensagens e inicializa
void salva_sapo(SAPO *lista_sapos, int *indice_sapo, JOGADOR *jogador, FASE *fase, char *tecla)
{
    // plota o sapo salvo
    desenha_sapo_salvo(jogador->sapos_salvos, BROWN);

    // altera os sapos do jogador
    jogador->sapos_espera--;
    jogador->sapos_salvos++;

    // altera o status do sapo ativo para salvo
    lista_sapos[*indice_sapo].status = SALVO;

    // se eh o indice do ultimo sapo
    if(*indice_sapo+1 == fase->num_sapos)
    {

        // se o jogador salvou todos os sapos da fase 1, inicia a fase 2
        if(fase->fase == 1 && jogador->sapos_salvos == fase->num_sapos)
        {
            mostra_mensagem(MSG_FASE_2);
            // implementar fase 2
            fase_dois(fase, jogador, lista_sapos, indice_sapo);
        }else{
            // mostra mensagem que acabaram as vidas do sapo
            mostra_mensagem(MSG_FIM_VIDAS);
            *tecla = T_ESC;
        }
    }
    // se não eh o indice do ultimo sapo
    else
    {
        // muda para o proximo sapo
        *indice_sapo += 1;

        // altera o status do sapo em espera para ativo
        lista_sapos[*indice_sapo].status = ATIVO;

        // mostra a mensagem que o sapo atravessou a rua
        mostra_mensagem(MSG_ATRAVESSOU_RUA);

        // desenha o sapo no inicio da tela
        plota_sapo(lista_sapos[*indice_sapo].envelope[0], lista_sapos[*indice_sapo].cor);

        // desenha um sapo inativo a menos
        redesenha_inativos(*indice_sapo, WHITE, fase->num_sapos);
    }
}

// testa se o sapo colidiu, mostra mensagens e inicializa
int mata_sapo(SAPO *lista_sapos, VEICULO *lista_veiculos, int *indice_sapo, char *tecla, FASE fase)
{
    // testa colisao entre o sapo ativo e todos os veiculos
    if(testa_colisoes(lista_sapos[*indice_sapo], lista_veiculos))
    {
        // altera o status do sapo ativo para morto
        lista_sapos[*indice_sapo].status = MORTO;

        // explode o sapo
        explode_sapo(lista_sapos[*indice_sapo].envelope[0]);

        // se eh o indice do ultimo sapo
        if(*indice_sapo+1 == fase.num_sapos)
        {
            // mostra mensagem de sapo morto
            mostra_mensagem(MSG_FIM_VIDAS);
            *tecla = T_ESC;
        }
        else
        {
            // mostra mensagem de sapo morto
            mostra_mensagem(MSG_COLIDIU);

            // apaga o fogo
            apaga_fogo(lista_sapos[*indice_sapo].envelope[0]);

            // muda para o proximo sapo
            *indice_sapo += 1;

            // altera o status do sapo em espera para ativo
            lista_sapos[*indice_sapo].status = ATIVO;

            // desenha o sapo no inicio da tela
            plota_sapo(lista_sapos[*indice_sapo].envelope[0], lista_sapos[*indice_sapo].cor);

            // desenha um sapo inativo a menos
            redesenha_inativos(*indice_sapo, WHITE, fase.num_sapos);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

// testa se um sapo e uma lista de veiculos colidiram
short testa_colisoes(SAPO sapo, VEICULO lista_veiculos[])
{

    int colidiu=0, i=0;

    // testa colisao enquanto nao colidir e nao chegar no fim da lista de veiculos
    while(!colidiu && i<NUM_VEICULOS)
    {
        colidiu = testa_colisao(sapo, lista_veiculos[i]);
        i++;
    }
    return colidiu;
}

// testa se dois objetos se colidiram
short testa_colisao(SAPO sapo, VEICULO veiculo)
{
    // coordenadas x1 e y1 do obj1
    COORDENADA c1Sapo = sapo.envelope[0];
    // coordenadas x2 e y2 do obj1
    COORDENADA c2Sapo = sapo.envelope[1];

    // coordenadas x1 e y1 do obj2
    COORDENADA c1Veiculo = veiculo.envelope[0];
    // coordenadas x2 e y2 do obj2
    COORDENADA c2Veiculo = veiculo.envelope[1];

    //colidiu
    if(c1Veiculo.x <= c2Sapo.x && c2Veiculo.x >= c1Sapo.x && c1Veiculo.y <= c2Sapo.y && c2Veiculo.y >= c1Sapo.y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// recebe as coordenadas do sapo apos colidir e desenha um fogo sobre o sapo
void explode_sapo(COORDENADA coordenada)
{
    int i, j;
    // desenho do fogo em numeros que serao trocados pela cor correspondente
    int fogo[TAM_YFOGO][TAM_XFOGO] = {{BLACK,BLACK,BLACK,RED,BLACK,BLACK,BLACK,BLACK},
        {BLACK,BLACK,BLACK,RED,RED,BLACK,BLACK,BLACK},
        {RED,BLACK,RED,RED,YELLOW,RED,BLACK,RED},
        {BLACK,RED,RED,YELLOW,YELLOW,RED,RED,BLACK},
        {RED,RED,YELLOW,YELLOW,YELLOW,YELLOW,RED,RED}
    };

    // pinta todo o envelope do fogo de vermelho
    textbackground(RED);
    for(i = 0; i<TAM_YFOGO; i++)
    {
        for(j= 0; j<TAM_XFOGO; j++)
        {
            gotoxy(coordenada.x+j, coordenada.y+i);
            printf(" ");
        }
    }

    for(i = 0; i<TAM_YFOGO; i++)
    {
        for(j= 0; j<TAM_XFOGO; j++)
        {

            // altera a cor de fundo de acordo com a matriz
            textbackground(fogo[i][j]);
            // escreve " " para que apenas o fundo fique pintado
            cputsxy(coordenada.x+j, coordenada.y+i, " ");
        }
    }

    // muda para a cor de fundo inicial
    textbackground(BLACK);
}

// recebe as coordenadas do envelope fogo e pinta de preto
void apaga_fogo(COORDENADA coordenada)
{
    int i, j;
    // pinta todo o envelope do fogo de preto
    textbackground(BLACK);
    for(i = 0; i<TAM_YFOGO; i++)
    {
        for(j= 0; j<TAM_XFOGO; j++)
        {
            gotoxy(coordenada.x+j, coordenada.y+i);
            printf(" ");
        }
    }
}

// apaga um sapo inativo e redesenha o resto
void redesenha_inativos(int indice_sapo, COLORS cor, int num_sapos)
{
    desenha_sapos_inativos(indice_sapo-1, BLACK, num_sapos);
    desenha_sapos_inativos(indice_sapo, cor, num_sapos);
}

// desenha um sapo salvo no lado direito apos o ultimo sapo salvo
void desenha_sapo_salvo(int sapos_salvos, COLORS cor)
{
    COORDENADA coordenada;
    coordenada.x = XFIM+2;
    coordenada.y = YINI+2 + sapos_salvos * 3;
    plota_sapo(coordenada, cor);
}

// desenha fora da moldura os sapos inativos
void desenha_sapos_inativos(int indice_sapo, COLORS cor, int num_sapos)
{
    int xSapo = XINI+1, ySapo = YFIM+1, i, sapos_inativos = num_sapos - indice_sapo - 1;
    COORDENADA coordenada;

    for(i = 0; i < sapos_inativos; i++)
    {
        coordenada.x = xSapo;
        coordenada.y = ySapo;
        // plota o sapo inativo na coordenada passada
        plota_sapo(coordenada, cor);
        // aumenta o espcao de cada sapo inativo
        xSapo += ESPACO_SAPO_INATIVO;
    }
}

// desenha no lado direito todos os sapos salvos
void desenha_sapos_salvos(int sapos_salvos, COLORS cor)
{
    int i;
    for(i = 0; i<sapos_salvos; i++)
    {
        desenha_sapo_salvo(i, cor);
    }
}

// mostra as mensagens de acordo com o resultado do jogo
void mostra_mensagem(int mensagem)
{
    char tecla;

    switch(mensagem)
    {
        case MSG_COLIDIU:
            gotoxy(XINI, YMENSAGEM);
            textcolor(RED);
            printf("VOCE MATOU O SAPO!! APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_ATRAVESSOU_RUA:
            gotoxy(XINI, YMENSAGEM);
            textcolor(YELLOW);
            printf("PARABENS VOCE CHEGOU DO OUTRO LADO DA RUA E SALVOU O SAPO!!! APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_FIM_VIDAS:
            gotoxy(XINI, YFIM+2);
            textcolor(YELLOW);
            printf("FIM DE JOGO!! VOCE NAO POSSUI MAIS SAPOS. APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_FIM_TEMPO:
            gotoxy(XINI, YMENSAGEM);
            textcolor(RED);
            printf("FIM DE JOGO!! O TEMPO ACABOU. APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_PAUSE:
            gotoxy(XINI, YMENSAGEM);
            textcolor(YELLOW);
            printf("JOGO PAUSADO. APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_SEM_ARQUIVO:
            gotoxy(XINI, YMENSAGEM);
            textcolor(RED);
            printf("NAO FOI POSSIVEL CARREGAR O JOGO. APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_LER_ARQUIVO:
            gotoxy(XINI, YMENSAGEM);
            textcolor(YELLOW);
            printf("JOGO CARREGADO COM SUCESSO! APERTE ESPACO PARA CONTINUAR!");
            break;
        case MSG_INICIO:
            gotoxy(XINI, YMENSAGEM);
            textcolor(YELLOW);
            printf("APERTE ESPACO PARA COMECAR O JOGO");
            break;
        case MSG_FASE_2:
            gotoxy(XINI, YFIM+2);
            textcolor(YELLOW);
            printf("VOCE SALVOU TODOS OS SAPOS, APERTE ESPACO PARA COMECAR A FASE 2");
            break;
    }
    do
    {
        tecla = getch();
    }
    while(tecla != T_ESPACO);

    delline();
    textcolor(WHITE);
}

// inicializa os dados do jogador
void inicializa_jogador(JOGADOR *jogador)
{
    textcolor(WHITE);
    printf("Digite o seu nome:\n");
    gets(jogador->nome);

    jogador->sapos_salvos = 0;
    jogador->sapos_espera = NUM_SAPOS - 1;
    // pega horario atual do clock
    jogador->inicioJogo = time(NULL);
    jogador->tempoJogo = 0;
    jogador->score = 0;

    // depois de o jogador inserir o nome, espera meio segundo para o jogo comecar
    Sleep(TEMPO_COMECA_JOGO);
}

// calcula a pontuacao do jogador apos o fim do jogo
void calcula_score(JOGADOR *jogador)
{
    // O JOGADOR POSSUI 2 FORMAS DE PONTUAR
    // 1 - SE TERMINOU O JOGO MORRENDO OU SALVANDO TODOS SAPOS, SUBTRAI O TEMPO QUE LEVOU PARA TERMINAR O JOGO
    // DO TEMPO MAXIMO DO JOGO. SE ULTRAPASSOU O TEMPO MAXIMO, NÃO PONTUA
    // 2 - SE SALVOU ALGUM SAPO. CADA SAPO SALVO ADICIONA UM VALOR CONSTANTE AO SOMATORIO DA PONTUACAO
    // AO FINAL SOMA-SE A PONTUACAO 1 E PONTUACAO 2

    int score = 0;
    // limpa a tela
    clrscr();

    // subtrai o tempo do inicio do jogo do tempo atual
    jogador->tempoJogo = (int) difftime(time(NULL), jogador->inicioJogo);

    // se completou o jogo ate o tempo maximo, pontua
    if(jogador->tempoJogo < TEMPO_MAXIMO)
    {
        // subtrai o tempo que levou para concluir o jogo do tempo maximo
        score = TEMPO_MAXIMO - jogador->tempoJogo;
    }

    // soma a pontuacao do tempo do jogo com a pontuacao de cada sapo salvo
    jogador->score += score + jogador->sapos_salvos * PONTO_SAPO_SALVO;
}

//CALCULA E RETORNA INTEIRO DA PONTUACAO DE DETERMINADO JOGADOR
int calcula_score_auxiliar(JOGADOR jogador)
{
    // O JOGADOR POSSUI 2 FORMAS DE PONTUAR
    // 1 - SE TERMINOU O JOGO MORRENDO OU SALVANDO TODOS SAPOS, SUBTRAI O TEMPO QUE LEVOU PARA TERMINAR O JOGO
    // DO TEMPO MAXIMO DO JOGO. SE ULTRAPASSOU O TEMPO MAXIMO, NÃO PONTUA
    // 2 - SE SALVOU ALGUM SAPO. CADA SAPO SALVO ADICIONA UM VALOR CONSTANTE AO SOMATORIO DA PONTUACAO
    // AO FINAL SOMA-SE A PONTUACAO 1 E PONTUACAO
    int score = 0;

    if(jogador.tempoJogo < TEMPO_MAXIMO)
    {
        // subtrai o tempo que levou para concluir o jogo do tempo maximo
        score = TEMPO_MAXIMO - jogador.tempoJogo;
    }

    // soma a pontuacao do tempo do jogo com a pontuacao de cada sapo salvo
    return (score + jogador.sapos_salvos * PONTO_SAPO_SALVO);
}

// mostra mensagem final pro jogador
void encerra_jogo(JOGADOR jogador)
{
    // abre o arquivo de ranking e mostra na tela
    abre_arquivo(jogador, 1);

    printf("\nRESUMO DO SEU JOGO:\n");

    printf("Nome: %s\n"
           "Sapos salvos: %d\n"
           "Tempo de jogo: %d segundos\n"
           "Pontuacao final: %d pontos\n\n", jogador.nome, jogador.sapos_salvos+jogador.sapos_salvos_f1, jogador.tempoJogo+jogador.tempoJogo_f1, jogador.score);
}

// salva o estado atual do jogo em um arquivo binario
void pausa_jogo(JOGADOR *jogador, SAPO lista_sapos[], VEICULO lista_veiculos1[], VEICULO lista_veiculos2[], FASE fase)
{
    ESTADO estado;
    int i;

    // subtrai o tempo do inicio do jogo do tempo atual
    jogador->tempoJogo = (int) difftime(time(NULL), jogador->inicioJogo);
    estado.jogador = *jogador;
    estado.fase = fase;

    // percorre a lista de sapos
    for(i = 0; i< fase.num_sapos; i++){
        estado.lista_sapos[i] = lista_sapos[i];
    }

    // percorre as listas de veiculos
    for(i = 0; i< NUM_VEICULOS; i++){
        estado.lista_veiculos1[i] = lista_veiculos1[i];
        estado.lista_veiculos2[i] = lista_veiculos2[i];
    }

    // salvo o estado do jogo
    salva_estado_jogo(estado);
}

// instancia todos as variaveis do jogo com os valores do arquivo binario
void instancia_jogo(ESTADO estado, JOGADOR *jogador, SAPO *lista_sapos, VEICULO *lista_veiculos1, VEICULO *lista_veiculos2, int *indice_sapo, FASE *fase)
{
    int i;
    // mostra mensagem que leu o arquivo com sucesso
    mostra_mensagem(MSG_LER_ARQUIVO);

    // apaga o sapo e veiculoss antigos da tela
    plota_sapo(lista_sapos[*indice_sapo].envelope[0], BLACK);
    apaga_veiculos(lista_veiculos1);
    apaga_veiculos(lista_veiculos2);

    // apaga todos sapos inativos e salvos
    desenha_sapos_inativos(0, BLACK, fase->num_sapos);
    desenha_sapos_salvos(jogador->sapos_salvos, BLACK);

    *jogador = estado.jogador;
    // recalcula o tempo do jogo
    jogador->inicioJogo = time(NULL) - jogador->tempoJogo;

    *fase = estado.fase;

    // instancia todos os sapos
    for(i = 0; i< fase->num_sapos; i++){
        // se o sapo tem status ativo, altera o indice do sapo
        if(estado.lista_sapos[i].status == ATIVO){
            *indice_sapo = i;
        }
        lista_sapos[i] = estado.lista_sapos[i];
    }

    // instancia todos os veiculos
    for(i = 0; i< NUM_VEICULOS; i++){
        lista_veiculos1[i] = estado.lista_veiculos1[i];
        lista_veiculos2[i] = estado.lista_veiculos2[i];
    }

    // desenha o sapo na posicao atual
    plota_sapo(lista_sapos[*indice_sapo].envelope[0], lista_sapos[*indice_sapo].cor);

    // desenha todos sapos inativos
    desenha_sapos_inativos(*indice_sapo, WHITE, fase->num_sapos);

    // se o jogador nao salvou nenhum sapo, mostra o header inicial
    if(jogador->sapos_salvos == 0)
    {
        escreve_header();
    }else
    {
        // desenha todos os sapos salvos
        desenha_sapos_salvos(jogador->sapos_salvos, BROWN);
    }

    // desenha novamente os veiculos
    desenha_lista_veiculos(lista_veiculos1);
    desenha_lista_veiculos(lista_veiculos2);
}

// mostra na tela o cronometro do jogo
void mostra_cronometro(JOGADOR jogador)
{
    int tempo;

    tempo = (int) difftime(time(NULL), jogador.inicioJogo);

    cputsxy(XFIM-X_TEXTO_CRON, YFIM+1, "Tempo de jogo");
    gotoxy(XFIM-X_CRON, YFIM+2);
    printf("%ds", tempo);
}

// pausa o cronometro do jogador
void pausa_cronometro(JOGADOR *jogador)
{
    // subtrai o tempo do inicio do jogo do tempo do clock
    jogador->tempoJogo = (int) difftime(time(NULL), jogador->inicioJogo);
}

// despausa o cronometro para o valor anterior
void despausa_cronometro(JOGADOR *jogador)
{
    // subtrai o tempo do jogo do tempo do clock
    jogador->inicioJogo = time(NULL) - jogador->tempoJogo;
}

// ESCREVE INFO DO JOGADOR NA TELA
void escreve_info_jogador(JOGADOR jogador)
{
    char aux[15];
    //CALCULA SEGUNDOS DE JOGO POR MEIO DA DIFERENCA DE TEMPO ENTRE INICIO E MOMENTO ATUAL
    int tempo = (int) difftime(time(NULL), jogador.inicioJogo);
    jogador.tempoJogo = tempo;
    //CALCULA SCORE ATUAL POR MEIO DE FUNCAO AUXILIAR
    jogador.score = calcula_score_auxiliar(jogador);
    textcolor(WHITE);
    cputsxy(XFIM+20, YINI, "Suas informacoes:");
    cputsxy(XFIM+20, YINI+2, "Nome: ");
    cputsxy(XFIM+20, YINI+5, "Pontos: ");
    cputsxy(XFIM-5, YINI+2, "Tempo de jogo: ");
    textcolor(YELLOW);
    cputsxy(XFIM+20, YINI+3, jogador.nome);
    sprintf(aux, "%d",jogador.score);
    cputsxy(XFIM+20, YINI+6, aux);
    sprintf(aux, "%ds", tempo);
    cputsxy(XFIM+20, YINI+9, aux);
}

void abre_arquivo(JOGADOR jogador, int fim_jogo)
{
    FILE* arq;
    int retorno, tamanho;
    JOGADOR lista_jogadores[NUMERO_MAXIMO_RANKING+1];

    clrscr();
    //abre arquivo do ranking
    retorno = abre_arq_jog(&arq);
    if(retorno == 0){
        //caso ocorra erro apresenta mensagem
        printf("\nErro ao abrir ranking de jogadores\n\n");
    }else{

        //le arquivo do ranking e atribui retorno, o número de jogadores, a variavel de tamanho
        tamanho = le_arq_texto(arq, lista_jogadores);
        //exibe ranking final
        mostra_lista_jogadores(lista_jogadores, jogador, tamanho);

        // se esta chamando no fim do jogo
        if(fim_jogo == 1)
        {
            //insere jogador atual na lista
            insere_jogador(jogador, lista_jogadores, &tamanho);
            //ordena jogadores
            ordena_lista_jogadores(jogador, lista_jogadores, tamanho);
            //salva novo arquivo
            retorno = salva_lista_jogadores(arq, lista_jogadores, tamanho);

            if(retorno==0){
                //caso nao seja possivel salvar exibe mensagem
                printf("\nNão foi possível salvar sua pontuação\n\n");
            }
        }
    }
}

void ordena_lista_jogadores(JOGADOR jog, JOGADOR *lista_jogadores, int tam)
{
    int i, j;
    JOGADOR temp;
    //REALIZA UM BUBBLE SORT PARA ORDENAR DE MANEIRA DECRESCENTE
    for (i = 1; i < tam; i++)
    {
        for (j = 0; j < i; j++)
        {
            //CASO JOGADOR DO INDICE I TTENHA SCORE SUPERIOR A DO INDICE J
            if (lista_jogadores[i].score > lista_jogadores[j].score)
            {
              //TROCA POSICOES DE AMBOS POR MEIO DE VARIAVEL AUXILIAR, ANALOGO A SWAP
                temp = lista_jogadores[i];
                lista_jogadores[i] = lista_jogadores[j];
                lista_jogadores[j] = temp;
            }
        }
    }
}

void insere_jogador(JOGADOR jog, JOGADOR *lista_jogadores, int *tam)
{
    int i, pos = -1;
    for(i = 0; i<*tam; i++)
    {
        if(strcmp(lista_jogadores[i].nome, jog.nome)==0)
        {
          //CASO OS NOMES SEJAM IGUAIS E A PONTUACAO O VALOR DE I E ATRIBUIDO A POS
            pos = i;
        }
    }
    if(pos!=-1)
    {
      //CASO A POSICAO SEJA DIFERENTE DE -1, OS NOMES COINCIDEM NA POSICAO POS
      //PORTANTO O JOGADOR DA LISTA E SOBRESCRITO SE POSSUIR UMA PONTUACAO MENOR QUE O JOGADOR ATUAL
      if(jog.score>=lista_jogadores[pos].score){
          lista_jogadores[pos] = jog;
      }

    }
    else
    {
      //CASO CONTRARIO O JOGADOR E POSTO NO FINAL DO ARRAY

        lista_jogadores[*tam] = jog;

        //E O TAMANHO DO ARRAY E INCREMENTADO
        *tam=*tam+1;

    }
}

void mostra_lista_jogadores(JOGADOR *lista_jogadores, JOGADOR jogador_atual, int tam)
{
    //cria lista somente para exibicao, nao sera salva
    JOGADOR lista_jogadores_exibicao[NUMERO_MAXIMO_RANKING+1];
    int i, pos =-1;

    for(i = 0; i<tam; i++)
    {
        if(strcmp(lista_jogadores[i].nome, jogador_atual.nome)==0)
        {
          //caso os nomes sejam os mesmos guarda indice
          pos=i;
        }else{
          //caso contrario, copia jogador para lista de exibicao
          lista_jogadores_exibicao[i] = lista_jogadores[i];
        }
    }
    if(pos!=-1){
        //se o jogador tiver uma pontuacao no ranking
        //exibe a mais recente, contudo nao salva
        //(a salva e a mais alta sempre)
       lista_jogadores_exibicao[pos] = jogador_atual;
       sprintf(lista_jogadores_exibicao[pos].nome, "%s (score + recente)", jogador_atual.nome);
    }else{

      //atibui jogador pro final da lista e incrementa auxiliar de tamanho
      lista_jogadores_exibicao[tam++] = jogador_atual;
    }
    //ordena lista de exibicao
    ordena_lista_jogadores(jogador_atual, lista_jogadores_exibicao, tam);
    //exibe ranking
    printf("RANKING DE JOGADORES:\n");
    for(i = 0; i < tam && i <= NUMERO_MAXIMO_RANKING; i++)
    {
      //IMPRIME O NOME E O SCORE DOS JOGADORES DA LISTA
        printf("%d - %-25s : %d PONTOS\n", i+1, lista_jogadores_exibicao[i].nome, lista_jogadores_exibicao[i].score);
    }
}
