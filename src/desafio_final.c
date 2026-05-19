#include "desafio_final.h"
#include <string.h>

void iniciar_boss(EstadoJogo *e) {
    strncpy(e->boss_atual.nome, "tubarão Ratão");
    e->boss_atual.vida_max = 100;
    e->boss_atual.vida =  100;
    e->boss_atual.estado = ESTADO_CAUTELOSO;
    e->boss_atual.acao_timer = 0.0f;
    e->boss_atual.acao_intervalo = 1.5f;
    e->boss_atual.atacando      = 0;
    e->boss_atual.defendendo    = 0;
    e->vida_jogador             = 100;
    strcpy(e->log_combate, "O Guardiao apareceu!");

}

void atualizar_boss(EstadoJogo *e){
    Boss *b = &e->boss_atual;
    if(b->vida > b->vida_max *0.8f){
        b->estado = ESTADO_CAUTELOSO;
    }else if(b->vida < b->vida_max *0.5f){
        b->estado = ESTADO_AGRESSIVO;
    }else if(b->vida < vida_max *0.2f){
        b->estado = ESTADO_DESESPERADO;
    }
}

void executar_acao_boss(EstadoJogo *e) {
    Boss *b = &e->boss_atual;
    switch (b->estado) {
        case ESTADO_AGRESSIVO:
            b->atacando       = 1;
            b->defendendo     = 0;
            e->vida_jogador  -= 15;
            b->acao_intervalo = 1.0f;
            strcpy(e->log_combate, "Guardiao ATACA com forca maxima!");
            break;
        case ESTADO_CAUTELOSO:
            if (b->atacando) {
                b->atacando   = 0;
                b->defendendo = 1;
                strcpy(e->log_combate, "Guardiao se defende...");
            } else {
                b->atacando       = 1;
                b->defendendo     = 0;
                e->vida_jogador  -= 8;
                strcpy(e->log_combate, "Guardiao ataca com cautela.");
            }
            b->acao_intervalo = 1.5f;
            break;
        case ESTADO_DESESPERADO:
            if (b->vida < b->vida_max)
            b->vida += 5;
            b->atacando       = 1;
            e->vida_jogador  -= GetRandomValue(5, 20);
            b->acao_intervalo = 0.8f;
            strcpy(e->log_combate, "Guardiao esta DESESPERADO e se cura!");
            break;
    }
    if (e->vida_jogador < 0)       e->vida_jogador = 0;
    if (b->vida > b->vida_max)     b->vida = b->vida_max;
}

void processar_input_rio(EstadoJogo *e) {
    if (!e->turno_jogador) return;

    if (IsKeyPressed(KEY_A)) {
        e->boss_atual.vida -= 20;
        e->turno_jogador    = false;
        strcpy(e->log_combate, "Voce ATACOU o Guardiao!");
    }
    if (IsKeyPressed(KEY_D)) {
        e->boss_atual.vida -= 8;
        e->turno_jogador    = false;
        strcpy(e->log_combate, "Voce desferiu um golpe rapido!");
    }
}

void atualizar_rio(EstadoJogo *e, float dt) {
    Boss *b = &e->boss_atual;
    atualizar_estado_boss(e);

    if (!e->turno_jogador) {
        b->acao_timer += dt;
        if (b->acao_timer >= b->acao_intervalo) {
            b->acao_timer    = 0.0f;
            e->turno_jogador = true;
            executar_acao_boss(e);
        }
    }

    // Condições de fim
    if (b->vida <= 0) {
        b->vida       = 0;
        e->cena_atual = CENA_MAPA;
        if (e->atual) e->atual->visitado = 1;
    }
    if (e->vida_jogador <= 0) {
        e->vida_jogador = 0;
        e->cena_atual   = CENA_MENU; // game over → volta ao menu
    }
}