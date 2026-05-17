#include "jogo.h"
#include <math.h>

void processar_input(EstadoJogo *e) {
    if (e->cena_atual == CENA_MAPA) {
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) remar_frente(e);
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  remar_volta(e);
    }
    if (e->cena_atual == CENA_GAMEPLAY) {
        if (fila_vazia(&e->fila_ondas)) {
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  e->jogador_pos.x -= e->jogador_vel * GetFrameTime();
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) e->jogador_pos.x += e->jogador_vel * GetFrameTime();
        } else {
            processar_input_missao1(e);
        }
    }
}

void atualizar(EstadoJogo *e) {
    float dt  = GetFrameTime();
    e->timer += dt;

    // ─── 1. ANIMAÇÃO DE ABERTURA ─────────────────────────────────────────────
    if (e->cena_atual == CENA_ANIMACAO) {
        e->anim_frame_timer += dt;
        if (e->anim_frame_timer >= e->anim_frame_duracao) {
            e->anim_frame_timer = 0.0f;
            e->anim_frame_atual++;
            if (e->anim_frame_atual >= 9) {
                e->anim_frame_atual = 8;
                e->cena_atual = CENA_MENU;
            }
        }
        return;
    }

    // ─── 2. MENU PRINCIPAL ───────────────────────────────────────────────────
    if (e->cena_atual == CENA_MENU) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int sw = GetScreenWidth(), sh = GetScreenHeight();
            float escala = fminf((float)sw / LARGURA, (float)sh / ALTURA);
            float ox = (sw - LARGURA * escala) / 2.0f;
            float oy = (sh - ALTURA * escala) / 2.0f;
            Vector2 raw = GetMousePosition();
            Vector2 mouse = {(raw.x - ox) / escala, (raw.y - oy) / escala};

            // Clique ajustado para a região do botão Iniciar (com base nas suas coordenadas)
            if (mouse.x >= 510 && mouse.x <= 760 && mouse.y >= 180 && mouse.y <= 290) {
                e->cena_atual = CENA_TUTORIAL; 
                e->jogador_pos = (Vector2){ 512, 480 }; 
                e->jogador_vel = 250.0f; 
                e->timer = 0.0f; 
            }
        }
        return;
    }

    // ─── 3. TELA DE TUTORIAL ─────────────────────────────────────────────────
    if (e->cena_atual == CENA_TUTORIAL) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            e->cena_atual = CENA_GAMEPLAY;
        }
        return;
    }

    // ─── 4. FASE DE GAMEPLAY ─────────────────────────────────────────────────
    if (e->cena_atual == CENA_GAMEPLAY) {
        if (!fila_vazia(&e->fila_ondas)) {
            atualizar_missao1(e, dt);
        }
        return;
    }

    // ─── 5. MAPA DO RIO ──────────────────────────────────────────────────────
    if (e->cena_atual == CENA_MAPA) {
        float alvo_y = e->atual->pos.y - 20.0f;
        e->barco_x  += (e->barco_alvo_x - e->barco_x) * 5.0f * dt;
        e->barco_y  += (alvo_y          - e->barco_y) * 5.0f * dt;
        if (fabsf(e->barco_x - e->barco_alvo_x) < 1.0f)
            e->animando = false;
    }
}

void renderizar(EstadoJogo *e) {
    BeginTextureMode(e->target);
    
    if (e->cena_atual == CENA_ANIMACAO)      desenhar_animacao(e);
    else if (e->cena_atual == CENA_MENU)     desenhar_menu(e);
    else if (e->cena_atual == CENA_TUTORIAL) desenhar_tutorial(e);
    else if (e->cena_atual == CENA_GAMEPLAY) desenhar_gameplay(e);
    else if (e->cena_atual == CENA_MAPA)     desenhar_mapa(e);
    else                                     ClearBackground(BLACK);
    
    DrawFPS(LARGURA - 70, 8);
    EndTextureMode();

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float escala = fminf((float)sw / LARGURA, (float)sh / ALTURA);
    float ox = (sw - LARGURA * escala) / 2.0f;
    float oy = (sh - ALTURA * escala) / 2.0f;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        e->target.texture,
        (Rectangle){0, 0, LARGURA, -ALTURA},
        (Rectangle){ox, oy, LARGURA * escala, ALTURA * escala},
        (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}

void inicializar_fila(FilaInimigos *f) {
    f->frente = NULL;
    f->tras = NULL;
    f->tamanho = 0;
}

void enqueue_inimigo(FilaInimigos *f, Inimigo info) {
    NodeInimigo *novo = (NodeInimigo*)malloc(sizeof(NodeInimigo));
    novo->dado = info;
    novo->proximo = NULL;
    if (f->tras == NULL) {
        f->frente = novo;
        f->tras = novo;
    } else {
        f->tras->proximo = novo;
        f->tras = novo;
    }
    f->tamanho++;
}

void dequeue_inimigo(FilaInimigos *f) {
    if (f->frente == NULL) return;
    NodeInimigo *temp = f->frente;
    f->frente = f->frente->proximo;
    if (f->frente == NULL) {
        f->tras = NULL;
    }
    free(temp);
    f->tamanho--;
}

Inimigo* peek_inimigo(FilaInimigos *f) {
    if (f->frente == NULL) return NULL;
    return &(f->frente->dado);
}

int fila_vazia(FilaInimigos *f) {
    return (f->frente == NULL);
}

void limpar_fila(FilaInimigos *f) {
    while (!fila_vazia(f)) {
        dequeue_inimigo(f);
    }
}

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