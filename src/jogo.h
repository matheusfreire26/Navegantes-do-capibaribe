#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ─── Dimensões ───────────────────────────────────────────────────────────────
#define LARGURA  1024
#define ALTURA   600
#define NUM_NOS  4

// ─── Paleta ──────────────────────────────────────────────────────────────────
#define COR_NO_ATIVO    (Color){255, 200, 60,  255}
#define COR_NO_VISITADO (Color){80,  180, 120, 255}
#define COR_NO_INATIVO  (Color){100, 100, 120, 255}
#define COR_TEXTO       (Color){230, 220, 200, 255}
#define COR_TITULO      (Color){255, 200, 60,  255}
#define COR_BARCO       (Color){200, 140, 60,  255}
#define COR_FUNDO       (Color){0,   0,   0,   255}

// ─── Cenas ───────────────────────────────────────────────────────────────────
typedef enum {
    CENA_ANIMACAO,
    CENA_MENU,
    CENA_MAPA,
    CENA_TUTORIAL,
    CENA_GAMEPLAY
} Cena;

typedef struct Inimigo {
    int id;
    int hp;
    int hp_max;
    int dano;
    char nome[32];
} Inimigo;

typedef struct NodeInimigo {
    Inimigo dado;
    struct NodeInimigo *proximo;
} NodeInimigo;

typedef struct {
    NodeInimigo *frente;
    NodeInimigo *tras;
    int tamanho;
} FilaInimigos;

// ─── Estrutura do nó ─────────────────────────────────────────────────────────
typedef struct PontoTuristico {
    char nome[100];
    char subtitulo[100];
    char artefato[100];
    int  visitado;
    Vector2 pos;
    struct PontoTuristico *anterior;
    struct PontoTuristico *proximo;
} PontoTuristico;

// ─── Estado global do jogo ───────────────────────────────────────────────────
typedef struct {
    PontoTuristico *cabeca;
    PontoTuristico *atual;
    float barco_x;
    float barco_y;
    float barco_alvo_x;
    bool  animando;
    float timer;
    Texture2D mapa;
    Font  fonte;
    Texture2D anim_frames[9];
    int   anim_frame_atual;
    float anim_frame_timer;
    float anim_frame_duracao;  
    float anim_duracao;
    Cena  cena_atual;
    int   menu_opcao;
    RenderTexture2D target;
    Vector2 jogador_pos;
    float jogador_vel;
    Texture2D jogador_tex;
    Texture2D cenario_fundo;
    Texture2D sprite_jogador;
    int jogador_frame;
    int jogador_direcao;
    float jogador_timer;
    int chico_hp;
    int chico_hp_max;
    FilaInimigos fila_ondas;
    bool turno_jogador;
    char log_combate[128];
    float tempo_missao;
    int casaroes_salvos;
} EstadoJogo;

PontoTuristico *criar_parada(const char *nome, const char *subtitulo,
                              const char *artefato, Vector2 pos);
void construir_rio(EstadoJogo *e);
void liberar_rio(EstadoJogo *e);
void remar_frente(EstadoJogo *e);
void remar_volta(EstadoJogo *e);

void desenhar_animacao(EstadoJogo *e);
void desenhar_menu(EstadoJogo *e);

void desenhar_tutorial(EstadoJogo *e);
void desenhar_gameplay(EstadoJogo *e);
void desenhar_mapa(EstadoJogo *e);

void processar_input(EstadoJogo *e);
void atualizar(EstadoJogo *e);
void renderizar(EstadoJogo *e);

void inicializar_fila(FilaInimigos *f);
void enqueue_inimigo(FilaInimigos *f, Inimigo info);
void dequeue_inimigo(FilaInimigos *f);
Inimigo* peek_inimigo(FilaInimigos *f);
int fila_vazia(FilaInimigos *f);
void limpar_fila(FilaInimigos *f);

void iniciar_missao_1(EstadoJogo *e);
void processar_input_missao1(EstadoJogo *e);
void atualizar_missao1(EstadoJogo *e, float dt);

#endif