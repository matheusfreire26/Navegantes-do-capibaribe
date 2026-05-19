#include "desafio_final.h"
#include <string.h>

void iniciar_missao_rio(EstadoJogo *e) {
    e->chico_hp_max = 100;
    e->chico_hp = e->chico_hp_max;
    e->turno_jogador = true;
    e->tempo_missao = 0.0f;
    e->casaroes_salvos = 3;
    strcpy(e->log_combate, "As aguas do Capibaribe estao em perigo! Navegue e lute!");
    inicializar_fila(&e->fila_ondas);
    // TODO: adicionar inimigos da fase Rio Capibaribe
}

void processar_input_rio(EstadoJogo *e) {
    // TODO: implementar controles da fase Rio Capibaribe
    (void)e;
}

void atualizar_rio(EstadoJogo *e, float dt) {
    // TODO: implementar logica da fase Rio Capibaribe
    (void)dt;
    if (fila_vazia(&e->fila_ondas)) {
        e->cena_atual = CENA_MAPA;
        if (e->atual != NULL) e->atual->visitado = 1;
    }
}
