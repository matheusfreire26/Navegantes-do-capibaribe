#include "jogo.h"

void processar_input(EstadoJogo *e) {
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) remar_frente(e);
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  remar_volta(e);
}

void atualizar(EstadoJogo *e) {
    float dt  = GetFrameTime();
    e->timer += dt;

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

    if (e->cena_atual == CENA_MENU) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int sw = GetScreenWidth(), sh = GetScreenHeight();
            float escala = fminf((float)sw / LARGURA, (float)sh / ALTURA);
            float ox = (sw - LARGURA * escala) / 2.0f;
            float oy = (sh - ALTURA * escala) / 2.0f;
            Vector2 raw = GetMousePosition();
            Vector2 mouse = {(raw.x - ox) / escala, (raw.y - oy) / escala};

            // Iniciar — ajuste os valores depois de anotar as coordenadas
            if (CheckCollisionPointRec(mouse, (Rectangle){400, 280, 200, 50}))
                e->cena_atual = CENA_MAPA;

            // Carregar — ajuste os valores
            if (CheckCollisionPointRec(mouse, (Rectangle){400, 350, 200, 50}))
                e->cena_atual = CENA_MAPA;

            // Opções — ajuste os valores
            if (CheckCollisionPointRec(mouse, (Rectangle){400, 420, 200, 50}))
                e->cena_atual = CENA_MAPA; // trocar por CENA_OPCOES depois
        }
        return;
    }

    if (e->cena_atual == CENA_MAPA) {
        float alvo_y = e->atual->pos.y - 20.0f;
        e->barco_x  += (e->barco_alvo_x - e->barco_x) * 5.0f * dt;
        e->barco_y  += (alvo_y          - e->barco_y) * 5.0f * dt;
        if (fabsf(e->barco_x - e->barco_alvo_x) < 1.0f)
            e->animando = false;
    }
}
void renderizar(EstadoJogo *e) {
    // Desenha tudo na tela virtual 1024×600
    BeginTextureMode(e->target);
    if (e->cena_atual == CENA_ANIMACAO) {
        desenhar_animacao(e);
    } else if (e->cena_atual == CENA_MENU) {
        desenhar_menu(e);
    } else {
        ClearBackground(BLACK);
    }
    DrawFPS(LARGURA - 70, 8);
    EndTextureMode();

    // Escala a tela virtual para preencher a janela real
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float escala = fminf((float)sw / LARGURA, (float)sh / ALTURA);
    float ox = (sw - LARGURA * escala) / 2.0f;
    float oy = (sh - ALTURA * escala) / 2.0f;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        e->target.texture,
        (Rectangle){0, 0, LARGURA, -ALTURA},   // -ALTURA corrige o flip do RenderTexture
        (Rectangle){ox, oy, LARGURA * escala, ALTURA * escala},
        (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}