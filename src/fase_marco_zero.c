#include "fase_marco_zero.h"
#include <string.h>
#include <stdio.h>

void iniciar_missao_1(EstadoJogo *e) {
    e->chico_hp_max = 100;
    e->chico_hp = e->chico_hp_max;
    e->turno_jogador = true;
    e->tempo_missao = 0.0f;
    e->casaroes_salvos = 3;
    strcpy(e->log_combate, "A fila de capangas avanca! Defenda o Marco Zero!");
    inicializar_fila(&e->fila_ondas);

    for (int i = 1; i <= 4; i++) {
        Inimigo capanga;
        capanga.id = i;
        capanga.hp_max = 40 + (i * 5);
        capanga.hp = capanga.hp_max;
        capanga.dano = 10 + i;
        snprintf(capanga.nome, sizeof(capanga.nome), "Capanga de Aluguel #%d", i);
        enqueue_inimigo(&e->fila_ondas, capanga);
    }

    Inimigo boss;
    boss.id = 99;
    boss.hp_max = 120;
    boss.hp = boss.hp_max;
    boss.dano = 22;
    strcpy(boss.nome, "Mestre das Construtoras (BOSS)");
    enqueue_inimigo(&e->fila_ondas, boss);
}

void processar_input_missao1(EstadoJogo *e) {
    if (!e->turno_jogador) return;
    Inimigo *atual = peek_inimigo(&e->fila_ondas);
    if (atual == NULL) return;

    if (IsKeyPressed(KEY_ONE)) {
        int dano_chico = 25;
        atual->hp -= dano_chico;
        snprintf(e->log_combate, sizeof(e->log_combate), "Chico disparou notas de Maracatu! Causou %d de dano.", dano_chico);
        if (atual->hp <= 0) {
            snprintf(e->log_combate, sizeof(e->log_combate), "%s foi derrotado!", atual->nome);
            dequeue_inimigo(&e->fila_ondas);
        }
        e->turno_jogador = false;
    }

    if (IsKeyPressed(KEY_TWO)) {
        strcpy(e->log_combate, "Chico entra em postura defensiva focado nas batidas.");
        e->turno_jogador = false;
    }
}

void atualizar_missao1(EstadoJogo *e, float dt) {
    if (fila_vazia(&e->fila_ondas)) {
        e->cena_atual = CENA_MAPA;
        if (e->atual != NULL) e->atual->visitado = 1;
        return;
    }

    e->tempo_missao += dt;
    if (e->tempo_missao >= 45.0f && e->casaroes_salvos > 1) {
        e->casaroes_salvos--;
        e->tempo_missao = 0.0f;
        strcpy(e->log_combate, "AVISO: A demora custou caro! Um casarao antigo desmoronou!");
    }

    if (!e->turno_jogador) {
        Inimigo *atual = peek_inimigo(&e->fila_ondas);
        if (atual != NULL) {
            int dano_recebido = (IsKeyDown(KEY_TWO) || IsKeyReleased(KEY_TWO)) ? (atual->dano / 2) : atual->dano;
            e->chico_hp -= dano_recebido;
            snprintf(e->log_combate, sizeof(e->log_combate), "%s revidou e causou %d de dano em Chico!", atual->nome, dano_recebido);
            if (e->chico_hp <= 0) {
                e->cena_atual = CENA_MENU;
                limpar_fila(&e->fila_ondas);
            }
        }
        e->turno_jogador = true;
    }
}
