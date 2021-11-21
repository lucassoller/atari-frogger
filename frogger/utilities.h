#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>

// coordenadas da moldura do jogo
#define XINI 1
#define XFIM 104
#define YINI 2
#define YFIM 25

#define XDISP 102

// coordenada da mensagem do jogo
#define YMENSAGEM 29
#define X_TEXTO_CRON 13
#define X_CRON 8

// pontuação de cada sapo salvo
#define PONTO_SAPO_SALVO 50

// nome do arquivo de texto
#define FROGGERJOG "FROGGERJog.txt"
// número de jogadores permitido no arquivo de texto
#define NUMERO_MAXIMO_RANKING 10

// tempo maximo de jogo para pontuar
#define TEMPO_MAXIMO 80

// tempo em milissegundos para o jogo comecar
#define TEMPO_COMECA_JOGO 500

// tamanho do desenho do fogo quando acontece uma colisão
#define TAM_XFOGO 8
#define TAM_YFOGO 5

// distância entre o desenho de cada sapo abaixo da moldura
#define ESPACO_SAPO_INATIVO 10

// coordenadas iniciais do sapo
#define X1_SAPO 48
#define Y1_SAPO 23
#define X2_SAPO 55
#define Y2_SAPO 24

// número de sapos
#define NUM_SAPOS 6
#define NUM_SAPOS_F2 8

#define Y1_CARRO 11

//tempo padrao entre iteracoes do fluxo principal
#define ESPERA_PRINCIPAL 10

// número de veiculos em cada pista
#define NUM_VEICULOS 4

// número de pistas no jogo
#define NUM_PISTAS 4

// distancias minimas e maximas entre cada veiculo das pistas
#define DISTANCIA_MAXIMA_F1 20
#define DISTANCIA_MINIMA_F1 10

// velocidades minimas e maximas das pistas
#define VELOCIDADE_MIN_F1 1
#define VELOCIDADE_MAX_F1 2

// coordenadas das duas pistas
#define Y_PISTA1 5
#define Y_PISTA2 9
#define Y_PISTA3 15
#define Y_PISTA4 19

// número de pistas
#define NUM_PISTAS 4
#define SIZE 100

// tamanho máximo do nome do jogador
#define TAM 20
#define TAM_MAX 256

typedef enum
{
    MSG_COLIDIU = 0,
    MSG_ATRAVESSOU_RUA = 1,
    MSG_FIM_VIDAS = 2,
    MSG_FIM_TEMPO = 3,
    MSG_PAUSE = 4,
    MSG_SEM_ARQUIVO = 5,
    MSG_LER_ARQUIVO = 6,
    MSG_INICIO = 7,
    MSG_FASE_2 = 8
} MENSAGEM;

typedef enum
{
    T_CIMA = 72,
    T_BAIXO = 80,
    T_ESQUERDA = 75,
    T_DIREITA = 77,
    T_ESPECIAL = -32,
    T_ESC = 27,
    T_ESPACO = 32,
    T_CMAX = 67,
    T_CMIN = 99,
    T_PMAX = 80,
    T_PMIN = 112,
    T_RMAX = 82,
    T_RMIN = 114,
    T_JMAX = 74,
    T_JMIN = 106
} TECLAS;

typedef enum
{
    ESPERA = 1,
    ATIVO = 2,
    SALVO = 3,
    MORTO = 4
} STATUS_SAPO;

typedef enum
{
    CIMA = 0,
    BAIXO = 1,
    ESQ = 2,
    DIR = 3
} DIRECAO_MOVIMENTO;

typedef enum
{
    ESPORTE = 0,
    SEDAN = 1,
    ONIBUS = 2
} TIPO_VEICULO;


typedef enum
{
    TAM_ESPORTE = 3,
    TAM_SEDAN = 5,
    TAM_ONIBUS = 7
} TAMANHO_VEICULO_X;


typedef struct
{
    int x;
    int y;
} COORDENADA;

typedef struct
{
    char nome[TAM];
    int sapos_salvos;
    int sapos_salvos_f1;
    int sapos_espera;
    time_t inicioJogo;
    int tempoJogo;
    int tempoJogo_f1;
    int score;
} JOGADOR;

typedef struct
{
    COORDENADA envelope[2];
    STATUS_SAPO status;
    COLORS cor;
} SAPO;

typedef struct
{
    COORDENADA envelope[2];
    int tamanhoEmX;
    int tamanhoEmY;
    int distancia;
    int valido;
    int velocidade;
    TIPO_VEICULO tipo;
    DIRECAO_MOVIMENTO dir;
    COLORS cor;
} VEICULO;

typedef struct
{
    DIRECAO_MOVIMENTO dir;
    int velocidade;
} PISTA;

typedef struct
{
    int fase;
    int num_sapos;
    COLORS cor_sapo;
} FASE;


typedef struct
{
    JOGADOR jogador;
    SAPO lista_sapos[NUM_SAPOS];
    VEICULO lista_veiculos1[NUM_VEICULOS];
    VEICULO lista_veiculos2[NUM_VEICULOS];
    VEICULO lista_veiculos3[NUM_VEICULOS];
    VEICULO lista_veiculos4[NUM_VEICULOS];
    FASE fase;
} ESTADO;
