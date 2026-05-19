#include "jogo.h"
#include "desafio1.h"
#include "desafio3.h"
#include "desafio_final.h"
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

    if (e->cena_atual == CENA_PONTE) {
        processar_input_ponte(e);
    }

    if (e->cena_atual == CENA_DESAFIO_FINAL)
        processar_input_rio(e);
}

void atualizar(EstadoJogo *e) {
    float dt  = GetFrameTime();
    e->timer += dt;

    // ─── 1. ANIMAÇÃO DE ABERTURA ─────────────────────────────────────────────
    if (e->cena_atual == CENA_ANIMACAO) {
        e->anim_frame_timer += dt;
        if (e->anim_frame_atual < 19) {
            if (e->anim_frame_timer >= e->anim_frame_duracao) {
                e->anim_frame_timer = 0.0f;
                e->anim_frame_atual++;
            }
        } else {
            if (e->anim_frame_timer >= e->anim_duracao) {
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
            Vector2 raw   = GetMousePosition();
            Vector2 mouse = {(raw.x - ox) / escala, (raw.y - oy) / escala};

            if (mouse.x >= 510 && mouse.x <= 760 && mouse.y >= 180 && mouse.y <= 290) {
                e->cena_atual  = CENA_TUTORIAL;
                e->jogador_pos = (Vector2){512, 480};
                e->jogador_vel = 250.0f;
                e->timer       = 0.0f;
            }
        }
        return;
    }

    // ─── 3. TELA DE TUTORIAL ─────────────────────────────────────────────────
    if (e->cena_atual == CENA_TUTORIAL) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            e->cena_atual = CENA_GAMEPLAY;
            iniciar_missao_1(e);
        }
        return;
    }

    // ─── 4. FASE DE GAMEPLAY (Marco Zero) ────────────────────────────────────
    if (e->cena_atual == CENA_GAMEPLAY) {
        if (!fila_vazia(&e->fila_ondas)) {
            atualizar_missao1(e, dt);
        } else {
            float velocidade = 200.0f;
            bool movendo     = false;

            if (e->jogador_frame < 2 || e->jogador_frame > 5)
                e->jogador_frame = 2;

            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                e->jogador_pos.x  += velocidade * dt;
                e->jogador_direcao = 2;
                movendo            = true;
            } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                e->jogador_pos.x  -= velocidade * dt;
                e->jogador_direcao = 3;
                movendo            = true;
            }

            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                e->jogador_pos.y  += velocidade * dt;
                e->jogador_direcao = 0;
                movendo            = true;
            } else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                e->jogador_pos.y  -= velocidade * dt;
                e->jogador_direcao = 1;
                movendo            = true;
            }

            if (movendo) {
                if (e->jogador_frame < 3) e->jogador_frame = 3;
                e->jogador_timer += dt;
                if (e->jogador_timer >= 0.12f) {
                    e->jogador_timer = 0.0f;
                    e->jogador_frame++;
                    if (e->jogador_frame > 5) e->jogador_frame = 3;
                }
            } else {
                e->jogador_frame = 2;
            }

            if (e->jogador_pos.x < 40.0f)  e->jogador_pos.x = 40.0f;
            if (e->jogador_pos.x > 980.0f) e->jogador_pos.x = 980.0f;
            if (e->jogador_pos.y < 350.0f) e->jogador_pos.y = 350.0f;
            if (e->jogador_pos.y > 490.0f) e->jogador_pos.y = 490.0f;
        }
        return;
    }

    // ─── 5. FASE DA PONTE ────────────────────────────────────────────────────
    if (e->cena_atual == CENA_PONTE) {
        atualizar_ponte(e, dt);
        return;
    }

    // ─── 6. DESAFIO FINAL ────────────────────────────────────────────────────
    if (e->cena_atual == CENA_DESAFIO_FINAL) {
        atualizar_rio(e, dt);
        return;
    }

    // ─── 7. MAPA DO RIO ──────────────────────────────────────────────────────
    if (e->cena_atual == CENA_MAPA) {
        float alvo_y = e->atual->pos.y - 20.0f;
        e->barco_x  += (e->barco_alvo_x - e->barco_x) * 5.0f * dt;
        e->barco_y  += (alvo_y          - e->barco_y) * 5.0f * dt;
        if (fabsf(e->barco_x - e->barco_alvo_x) < 1.0f)
            e->animando = false;

        // Ao chegar em um nó não visitado, inicia a fase correspondente
        if (!e->animando && e->atual != NULL && !e->atual->visitado) {
            // Nó 1 = Marco Zero (CENA_GAMEPLAY já cuida)
            // Nó 2 = Ponte Giratória
            if (e->atual == e->cabeca->proximo) {
                iniciar_missao_ponte(e);
                e->cena_atual = CENA_PONTE;
            }
        }
    }
}

void renderizar(EstadoJogo *e) {
    BeginTextureMode(e->target);

    if      (e->cena_atual == CENA_ANIMACAO)  desenhar_animacao(e);
    else if (e->cena_atual == CENA_MENU)      desenhar_menu(e);
    else if (e->cena_atual == CENA_TUTORIAL)  desenhar_tutorial(e);
    else if (e->cena_atual == CENA_GAMEPLAY)  desenhar_gameplay(e);
    else if (e->cena_atual == CENA_PONTE)         desenhar_ponte(e);
    else if (e->cena_atual == CENA_DESAFIO_FINAL) desenhar_desafio_final(e);
    else if (e->cena_atual == CENA_MAPA)          desenhar_mapa(e);
    else                                      ClearBackground(BLACK);

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

// ─── Fila ────────────────────────────────────────────────────────────────────
void inicializar_fila(FilaInimigos *f) {
    f->frente  = NULL;
    f->tras    = NULL;
    f->tamanho = 0;
}

void enqueue_inimigo(FilaInimigos *f, Inimigo info) {
    NodeInimigo *novo = (NodeInimigo*)malloc(sizeof(NodeInimigo));
    novo->dado     = info;
    novo->proximo  = NULL;
    if (f->tras == NULL) {
        f->frente = novo;
        f->tras   = novo;
    } else {
        f->tras->proximo = novo;
        f->tras          = novo;
    }
    f->tamanho++;
}

void dequeue_inimigo(FilaInimigos *f) {
    if (f->frente == NULL) return;
    NodeInimigo *temp = f->frente;
    f->frente = f->frente->proximo;
    if (f->frente == NULL) f->tras = NULL;
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
    while (!fila_vazia(f)) dequeue_inimigo(f);
}