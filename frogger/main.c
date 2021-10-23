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

void mostra_mensagem(int ganhou);
void inicializa_sapos(SAPO *lista_sapos);
void plota_sapo(COORDENADA coordenada, COLORS cor);
int move_sapo(SAPO *lista_sapos, int tecla, int *indice_sapo);
int mata_sapo(SAPO *lista_sapos, VEICULO lista_veiculos[], int *indice_sapo);
int salva_sapo(SAPO *lista_sapos, int *indice_sapo);
void testa_sapo(COORDENADA *coordenadas, int tecla, int *chegou_no_topo);
short testa_colisao(SAPO sapo, VEICULO veiculo);
short testa_colisoes(SAPO sapo, VEICULO lista_veiculos[]);
void explode_sapo(COORDENADA coordenada);
void apaga_fogo(COORDENADA coordenada);
void redesenha_inativos(int indice_sapo, COLORS cor);
void desenha_sapos_inativos(int indice_sapo, COLORS cor);
void inicializa_jogador(JOGADOR *jogador);
void calcula_score(JOGADOR *jogador);
void encerra_jogo(JOGADOR jogador);
void inicializa_carros(VEICULO *lista_veiculos, DIRECAO_MOVIMENTO direcao, int y_inicial);
void testa_carro_unitario(VEICULO *lista_veiculos, int posicao);
void desenha_veiculo(VEICULO veiculo, COLORS cor);
void desenha_lista_veiculos(VEICULO *lista_veiculos);
void apaga_carros(VEICULO *lista_veiculos);
void pausa_jogo(JOGADOR jogador, SAPO sapos[], VEICULO lista_veiculos1[], VEICULO lista_veiculos2[], int fase_atual);
void instancia_jogo(ESTADO estado, JOGADOR *jogador, SAPO *sapos, VEICULO *lista_veiculos1, VEICULO *lista_veiculos2, int *indice_sapo, int *fase_atual);
void escreve_header();

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
    int i;
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
    int fase_atual = 1;
    int indice_sapo = 0;
    int colide;
    int chegou_no_topo;
    char tecla;

    int pista1_y = Y_PISTA1;
    int pista2_y = Y_PISTA2;

    VEICULO lista_veiculos1[NUM_VEICULOS];
    DIRECAO_MOVIMENTO pista1_direcao = DIR;

    VEICULO lista_veiculos2[NUM_VEICULOS];
    DIRECAO_MOVIMENTO pista2_direcao = DIR;

    JOGADOR jogador;

    SAPO lista_sapos[NUM_SAPOS];
    COORDENADA coordenada;

    // inicializa valores
    inicializa_carros(lista_veiculos1, pista1_direcao, pista1_y);
    inicializa_carros(lista_veiculos2, pista2_direcao, pista2_y);
    inicializa_sapos(lista_sapos);
    inicializa_jogador(&jogador);

    // desenha a moldura do jogo;
    desenha_moldura();

    // escreve mensagens acima da moldura
    escreve_header();

    // plota os sapos e sapos inativos na tela
    plota_sapo(lista_sapos[indice_sapo].envelope[0], lista_sapos[indice_sapo].cor);
    desenha_sapos_inativos(indice_sapo, WHITE);

    do
    {
        // desenha as listas de veiculos no jogo
        desenha_lista_veiculos(lista_veiculos1);
        desenha_lista_veiculos(lista_veiculos2);

        // testa a colisao entre o sapo e todos os carros da lista1
        colide = mata_sapo(lista_sapos, lista_veiculos1, &indice_sapo);

        // se não colidiu, testa a colisao entre o sapo e todos os carros da lista2
        if(colide == 0){
            colide = mata_sapo(lista_sapos, lista_veiculos2, &indice_sapo);
        }

        // se colidiu e nao acabaram os sapos da lista, redesenha os sapos inativos e reposiciona os carros
        if(colide == 1)
        {
            // reposiciona veiculos
            inicializa_carros(lista_veiculos1, pista1_direcao, pista1_y);
            inicializa_carros(lista_veiculos2, pista2_direcao, pista2_y);
            // desenha um sapo inativo a menos
            redesenha_inativos(indice_sapo, WHITE);

        }// se colidiu e acabaram os sapo, encerra o jogo
        else if(colide == 2)
        {
            tecla = T_ESC;
        }

        // testa se o usuario pressionou alguma tecla
        if(kbhit())
        {
            // ler tecla pressionada pelo jogador
            tecla = getch();

            // se foi pressionada uma tecla do teclado auxiliar
            if(tecla == T_ESPECIAL)
            {
                // ler a tecla novamente
                tecla = getch();

                // move o sapo pelo jogo
                chegou_no_topo = move_sapo(lista_sapos, tecla, &indice_sapo);

                // se o sapo chegou no topo
                if(chegou_no_topo != 0)
                {
                    // plota o sapo salvo
                    coordenada.x = XFIM+2;
                    coordenada.y = YINI+2 + jogador.sapos_salvos * 3;
                    plota_sapo(coordenada, BROWN);
                    // acrescenta 1 sapo salvo para o jogador
                    jogador.sapos_salvos++;
                    // se chegou no topo e não acabaram os sapos da lista, redesenha os sapos inativos e reposiciona os veiculos
                    if(chegou_no_topo == 1)
                    {
                        mostra_mensagem(1);
                        // desenha o sapo no inicio da tela
                        plota_sapo(lista_sapos[indice_sapo].envelope[0], lista_sapos[indice_sapo].cor);
                        // reposiciona veiculos
                        inicializa_carros(lista_veiculos1, pista1_direcao, pista1_y);
                        inicializa_carros(lista_veiculos2, pista2_direcao, pista2_y);
                        // desenha um sapo inativo a menos
                        redesenha_inativos(indice_sapo, WHITE);

                    }// se chegou no topo e acabaram os sapos da lista, encerra o jogo
                    else
                    {
                        tecla = T_ESC;
                    }
                    // zera a variavel chegou_no_topo
                    chegou_no_topo = 0;
                }
            // se foi pressionada a tecla P maiuscula ou minuscula, pausa o jogo
            }else if(tecla == T_PMAX || tecla == T_PMIN)
            {
                pausa_jogo(jogador, lista_sapos, lista_veiculos1, lista_veiculos2, fase_atual);

            // se foi pressionada a tecla C maiuscula ou minuscula, carrega o jogo
            }else if(tecla == T_CMAX || tecla == T_CMIN)
            {
                // carrega todos os valores do arquivo binario na estrutura ESTADO
                //
                //
                // instancia cada valor de ESTADO nas variaveis do jogo
                // instancia_jogo(estado, &jogador, lista_sapos, lista_veiculos1, lista_veiculos2, &indice_sapo, &fase_atual);
            }
        }
    }

    while(tecla != T_ESC);
    textcolor(WHITE);
    // armazena nome do jogador, calcula escore
    calcula_score(&jogador);
    // mostra mensagem final do resultado
    encerra_jogo(jogador);
}

void escreve_header()
{
    textcolor(YELLOW);
    cputsxy(XINI, YINI-1, "INF1202 Lucas & Joao \t(C)arregar jogo \t(P)ausar \t(ESC)Sair");
    // sapos salvos
    textcolor(WHITE);
    cputsxy(XFIM+2, YINI, "Sapos salvos:");
    textcolor(BROWN);
    cputsxy(XFIM+2, YINI+2, "Nenhum");
}

// inicializa todos os sapos da lista
void inicializa_sapos(SAPO *lista_sapos)
{
    int i;
    STATUS_SAPO status_sapo;
    SAPO sapo;

    for(i = 0; i<NUM_SAPOS; i++)
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
        sapo.cor = GREEN;
        sapo.fase = 1;
        // adiciona o sapo na lista de sapos
        lista_sapos[i] = sapo;
    }
}

// inicializa as coordenadas da lista de carros na tela
void inicializa_carros(VEICULO *lista_veiculos, DIRECAO_MOVIMENTO direcao, int y_inicial)
{
    // coordenadas iniciais do primeiro carro
    int i, xAux = XINI+1, yAux=y_inicial;

    // apaga todos os carros para reposicionar
    apaga_carros(lista_veiculos);

    for(i=0; i<NUM_VEICULOS; i++)
    {
        lista_veiculos[i].tamanhoEmX = 3;
        lista_veiculos[i].tamanhoEmY = 2;
        // a cada 2 pontos, da o espaco entre os carros e volta y pro inicio
        lista_veiculos[i].tipo = rand() % 3;//randomiza o tipo de veiculo
        lista_veiculos[i].dir = direcao;
        xAux+= DISTANCIA_MINIMA +(rand() % DISTANCIA_MAXIMA);//randomiza distancias entre veiculos
        yAux= y_inicial;

        lista_veiculos[i].envelope[0].x=xAux;
        lista_veiculos[i].envelope[0].y=yAux;
        lista_veiculos[i].envelope[1].x=xAux + lista_veiculos[i].tamanhoEmX;
        lista_veiculos[i].envelope[1].y=yAux + lista_veiculos[i].tamanhoEmY;
        // armazena as coordenadas do carro
    }
}

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
        //caso o x inicial do carro não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do carro
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
        //caso o x inicial do carro não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do carro
        if(xc1 < XFIM && xc1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1, yc1, 'o');
                putchxy(xc1, yc1+1, 'G');
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
                putchxy(xc1 + 1, yc1+1, 'G');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1+1, yc1, 'o');
                putchxy(xc1+1, yc1+1, 'G');
                putchxy(xc1+1, yc1+2, 'o');
            }

        }
        if(xc1 + 2 < XFIM && xc1+2 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 +2, yc1, 'o');
                putchxy(xc1 +2, yc1+1, 'G');
                putchxy(xc1 +2, yc1+2, 'o');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1 + 2, yc1+1, 'G');
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
                putchxy(xc1 +3, yc1+1, 'G');
                putchxy(xc1 +3, yc1+2, 'o');
            }
        }
        break;
    case SEDAN:
        //caso o x inicial do carro não esteja dentro dos limites do jogo, retorna 0, caso não plota a secao do carro
        if(xc1 < XFIM && xc1 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1, yc1, 'o');
                putchxy(xc1, yc1+1, 'X');
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
                putchxy(xc1 + 1, yc1+1, 'X');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1+1, yc1, 'o');
                putchxy(xc1+1, yc1+1, 'X');
                putchxy(xc1+1, yc1+2, 'o');
            }

        }
        if(xc1 + 2 < XFIM && xc1+2 > XINI)
        {
            if(direcao == DIR)
            {
                putchxy(xc1 +2, yc1, 'o');
                putchxy(xc1 +2, yc1+1, 'X');
                putchxy(xc1 +2, yc1+2, 'o');

            }
            else if(direcao == ESQ)
            {
                putchxy(xc1 + 2, yc1+1, 'X');
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
                putchxy(xc1 +3, yc1+1, 'X');
                putchxy(xc1 +3, yc1+2, 'o');
            }
        }
        break;
    default:
        break;
    }
}

//APAGA VEICULOS DA TELA
void apaga_carros(VEICULO *lista_veiculos)
{
    int i;
    for(i=0; i<NUM_VEICULOS; i++)
    {
        desenha_veiculo(lista_veiculos[i], BLACK);
    }
}

// testa a movimentacao unitaria do carro na tela
void testa_carro_unitario(VEICULO *lista_veiculos, int posicao)
{
    int tecla;
    int velocidade=1;
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
            if(lista_veiculos[posicao].envelope[i].x + velocidade < XFIM)
            {
                // move para direita
                lista_veiculos[posicao].envelope[i].x+=velocidade;
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
            if(lista_veiculos[posicao].envelope[i].x - velocidade > XINI)
            {
                // move para direita
                lista_veiculos[posicao].envelope[i].x-=velocidade;
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


void desenha_lista_veiculos(VEICULO *lista_veiculos)
{
    int i;
    for(i=0; i<NUM_VEICULOS; i++)
    {
        //PARA CADA VEICULO APAGA A SUA IMAGEM ATUAL
        desenha_veiculo(lista_veiculos[i], BLACK);
        //ATUALIZA SUAS COORDENADAS
        testa_carro_unitario(lista_veiculos, i);
        //PLOTA NA NOVA POSICAO COM A COR DESEJADA
        desenha_veiculo(lista_veiculos[i], LIGHTRED);
    }
}

// move o sapo pela tela
int move_sapo(SAPO *lista_sapo, int tecla, int *indice_sapo)
{
    int chegou_no_topo = 0;
    int ultimo_sapo;

    // ponteiro *sapo aponta para o endereço do sapo da lista que esta na posição informada
    SAPO *sapo = &lista_sapo[*indice_sapo];

    // apaga o sapo do jogo
    plota_sapo(sapo->envelope[0], BLACK);

    // testa se o movimento é possivel
    testa_sapo(sapo->envelope, tecla, &chegou_no_topo);

    // se o sapo chegou no topo, testa se ainda tem sapos restantes
    if(chegou_no_topo)
    {
        // verifica se é o ultimo sapo
        ultimo_sapo = salva_sapo(lista_sapo, indice_sapo);

        // se for o ultimo encerra o jogo
        if(ultimo_sapo)
        {
            mostra_mensagem(2);
            chegou_no_topo = 2;
        }
    }
    else
    {
        // plota o sapo no jogo
        plota_sapo(sapo->envelope[0], sapo->cor);
    }
    return chegou_no_topo;
}

// testa a movimentacao do sapo na tela
void testa_sapo(COORDENADA *coordenadas, int tecla, int *chegou_no_topo)
{

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
int salva_sapo(SAPO *lista_sapos, int *indice_sapo)
{
    // altera o status do sapo ativo para salvo
    lista_sapos[*indice_sapo].status = SALVO;
    // se eh o indice do ultimo sapo
    if(*indice_sapo+1 == NUM_SAPOS)
    {
        return 1;
    }
    // se não eh o indice do ultimo sapo
    else
    {
        // muda para o proximo sapo
        *indice_sapo += 1;

        // altera o status do sapo em espera para ativo
        lista_sapos[*indice_sapo].status = ATIVO;
        return 0;
    }
}

// testa se o sapo colidiu, mostra mensagens e inicializa
int mata_sapo(SAPO *lista_sapos, VEICULO lista_veiculos[], int *indice_sapo)
{
    // testa colisao entre o sapo ativo e todos os veiculos
    if(testa_colisoes(lista_sapos[*indice_sapo], lista_veiculos))
    {
        // altera o status do sapo ativo para morto
        lista_sapos[*indice_sapo].status = MORTO;

        // explode o sapo
        explode_sapo(lista_sapos[*indice_sapo].envelope[0]);

        // se eh o indice do ultimo sapo
        if(*indice_sapo+1 == NUM_SAPOS)
        {
            // mostra mensagem de sapo morto
            mostra_mensagem(2);
            return 2;
        }
        else
        {
            // mostra mensagem de sapo morto
            mostra_mensagem(0);

            // apaga o fogo
            apaga_fogo(lista_sapos[*indice_sapo].envelope[0]);

            // muda para o proximo sapo
            *indice_sapo += 1;

            // altera o status do sapo em espera para ativo
            lista_sapos[*indice_sapo].status = ATIVO;

            // desenha o sapo no inicio da tela
            plota_sapo(lista_sapos[*indice_sapo].envelope[0], lista_sapos[*indice_sapo].cor);

            return 1;
        }
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
void redesenha_inativos(int indice_sapo, COLORS cor)
{
    desenha_sapos_inativos(indice_sapo-1, BLACK);
    desenha_sapos_inativos(indice_sapo, cor);
}

// desenha fora da moldura os sapos inativos
void desenha_sapos_inativos(int indice_sapo, COLORS cor)
{
    int xSapo = XINI+1, ySapo = YFIM+2, i, sapos_inativos = NUM_SAPOS - indice_sapo - 1;
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

// mostra as mensagens de acordo com o resultado do sapo
void mostra_mensagem(int ganhou)
{
    char tecla;

    if(ganhou == 0)
    {
        gotoxy(XINI, YMENSAGEM);
        textcolor(RED);
        printf("VOCE MATOU O SAPO!! APERTE ESPACO PARA CONTINUAR!");
    }
    else if (ganhou == 1)
    {
        gotoxy(XINI, YMENSAGEM);
        textcolor(YELLOW);
        printf("PARABENS VOCE CHEGOU DO OUTRO LADO DA RUA E SALVOU O SAPO!!! APERTE ESPACO PARA CONTINUAR!");
    }
    else if(ganhou == 2)
    {
        int y = YFIM+2;
        gotoxy(XINI, y);
        textcolor(YELLOW);
        printf("FIM DE JOGO!! VOCE NAO POSSUI MAIS SAPOS. APERTE ESPACO PARA CONTINUAR!");
    }
    else
    {
        gotoxy(XINI, YMENSAGEM);
        textcolor(RED);
        printf("FIM DE JOGO!! O TEMPO ACABOU. APERTE ESPACO PARA CONTINUAR!");
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
    jogador->sapos_salvos = 0;
    // pega horario atual do clock
    jogador->inicioJogo = time(NULL);
    jogador->tempoJogo = 0;
    jogador->score = 0;

    textcolor(WHITE);
    printf("Digite o seu nome:\n");
    gets(jogador->nome);

    // depois de o jogador inserir o nome, espera 1 segundo para o jogo comecar
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
    jogador->score = score + jogador->sapos_salvos * PONTO_SAPO_SALVO;

    printf("\nA sua pontuacao final eh: %d pontos", jogador->score);
}

// mostra mensagem final pro jogador
void encerra_jogo(JOGADOR jogador)
{
    // limpa a tela
    clrscr();

    printf("Nome: %s\n"
           "Sapos salvos: %d\n"
           "Tempo de jogo: %d segundos\n"
           "Pontuacao final: %d pontos\n\n", jogador.nome, jogador.sapos_salvos, jogador.tempoJogo, jogador.score);
}

void pausa_jogo(JOGADOR jogador, SAPO sapos[], VEICULO lista_veiculos1[], VEICULO lista_veiculos2[], int fase_atual)
{
    ESTADO estado;
    int i;

    estado.jogador = jogador;
    estado.fase_atual = fase_atual;

    for(i = 0; i< NUM_SAPOS; i++){
        estado.sapos[i] = sapos[i];
    }

    for(i = 0; i< NUM_VEICULOS; i++){
        estado.lista_veiculos1[i] = lista_veiculos1[i];
        estado.lista_veiculos2[i] = lista_veiculos2[i];
    }
    salva_estado_jogo(estado);
}

void instancia_jogo(ESTADO estado, JOGADOR *jogador, SAPO *sapos, VEICULO *lista_veiculos1, VEICULO *lista_veiculos2, int *indice_sapo, int *fase_atual)
{
    int i;

    *jogador = estado.jogador;
    *fase_atual = estado.fase_atual;

    for(i = 0; i< NUM_SAPOS; i++){
        if(estado.sapos[i].status == ATIVO){
            *indice_sapo = i;
        }
        sapos[i] = estado.sapos[i];
    }

    for(i = 0; i< NUM_VEICULOS; i++){
        lista_veiculos1[i] = estado.lista_veiculos1[i];
        lista_veiculos2[i] = estado.lista_veiculos2[i];
    }
}
