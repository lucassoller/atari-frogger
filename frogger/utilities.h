#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <windows.h>
#include <time.h>

// coordenadas da moldura do jogo
#define XINI 1
#define XFIM 79
#define YINI 2
#define YFIM 23

// coordenada da mensagem do jogo
#define YMENSAGEM 28

// pontuação de cada sapo salvo
#define PONTO_SAPO_SALVO 50

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
#define X1_SAPO 36
#define Y1_SAPO 21
#define X2_SAPO 43
#define Y2_SAPO 22

// número de sapos
#define NUM_SAPOS 6

#define Y1_CARRO 11

// número de veiculos em cada pista
#define NUM_VEICULOS 5

// distancias minimas e maximas entre cada carro das pistas
#define DISTANCIA_MAXIMA 15
#define DISTANCIA_MINIMA 5

// coordenadas das duas pistas
#define Y_PISTA1 6
#define Y_PISTA2 13

#define SIZE 100

// tamanho máximo do nome do jogador
#define TAM 20


typedef enum
{
    MSG_COLIDIU = 0,
    MSG_ATRAVESSOU_RUA = 1,
    MSG_FIM_VIDAS = 2,
    MSG_FIM_TEMPO = 3,
    MSG_PAUSE = 4,
    MSG_SEM_ARQUIVO = 5,
    MSG_LER_ARQUIVO = 6
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
    T_RMIN = 114
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

typedef struct
{
    int x;
    int y;
} COORDENADA;

typedef struct
{
    char nome[TAM];
    int sapos_salvos;
    time_t inicioJogo;
    int tempoJogo;
    int score;
} JOGADOR;

typedef struct
{
    COORDENADA envelope[2];
    STATUS_SAPO status;
    COLORS cor;
    int fase;

} SAPO;

typedef struct
{
    COORDENADA envelope[2];
    int tamanhoEmX;
    int tamanhoEmY;
    TIPO_VEICULO tipo;
    int distancia;
    DIRECAO_MOVIMENTO dir;
    int pista;
    int valido;
    int fase;
} VEICULO;

typedef struct
{
    JOGADOR jogador;
    SAPO lista_sapos[NUM_SAPOS];
    VEICULO lista_veiculos1[NUM_VEICULOS];
    VEICULO lista_veiculos2[NUM_VEICULOS];
    int fase_atual;
} ESTADO;
