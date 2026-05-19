#include "desafio2.h"
#include <string.h>

void iniciar_missao_torre(EstadoJogo *e) {
    e->chico_hp_max = 100;
    e->chico_hp = e->chico_hp_max;
    e->turno_jogador = true;
    e->tempo_missao = 0.0f;
    e->casaroes_salvos = 3;
    strcpy(e->log_combate, "A Torre de Cristal esta sob ameaca! Proteja o patrimonio!");
    inicializar_fila(&e->fila_ondas);
    // TODO: adicionar inimigos da fase Torre de Cristal
}

void processar_input_torre(EstadoJogo *e) {
    // TODO: implementar controles da fase Torre de Cristal
    (void)e;
}

void atualizar_torre(EstadoJogo *e, float dt) {
    // TODO: implementar logica da fase Torre de Cristal
    (void)dt;
    if (fila_vazia(&e->fila_ondas)) {
        e->cena_atual = CENA_MAPA;
        if (e->atual != NULL) e->atual->visitado = 1;
    }
}
