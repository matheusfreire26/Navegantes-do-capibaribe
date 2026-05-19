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
