#include "desafio3.h"
#include <string.h>

void iniciar_missao_ponte(EstadoJogo *e) {
    e->chico_hp_max = 100;
    e->chico_hp = e->chico_hp_max;
    e->turno_jogador = true;
    e->tempo_missao = 0.0f;
    e->casaroes_salvos = 3;
    strcpy(e->log_combate, "A Ponte Giratoria esta bloqueada! Libere a passagem!");
    inicializar_fila(&e->fila_ondas);
    // TODO: adicionar inimigos da fase Ponte Giratoria
}

void processar_input_ponte(EstadoJogo *e) {
    // TODO: implementar controles da fase Ponte Giratoria
    (void)e;
}

void atualizar_ponte(EstadoJogo *e, float dt) {
    // TODO: implementar logica da fase Ponte Giratoria
    (void)dt;
    if (fila_vazia(&e->fila_ondas)) {
        e->cena_atual = CENA_MAPA;
        if (e->atual != NULL) e->atual->visitado = 1;
    }
}
