#include "jogo.h"

void processar_input(EstadoJogo *e) {
    // Garante que os comandos do barco só funcionam quando estiver de fato no mapa
    if (e->cena_atual == CENA_MAPA) {
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) remar_frente(e);
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  remar_volta(e);
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

            // CORREÇÃO DO CLIQUE: Como o clique em "Opções" (mais abaixo) ativou o Y entre 300 e 400,
            // o botão "Iniciar" está localizado mais acima na tela. Ajustamos o Y para capturar a faixa de 180 a 290.
            if (mouse.x >= 510 && mouse.x <= 760 && mouse.y >= 180 && mouse.y <= 290) {
                e->cena_atual = CENA_TUTORIAL; 
                
                // Configurações iniciais do jogador para o teste de mobilidade
                e->jogador_pos = (Vector2){ 512, 480 }; 
                e->jogador_vel = 250.0f; 
                e->timer = 0.0f; 
            }
        }
        return;
    }

    // ─── 3. TELA DE TUTORIAL ─────────────────────────────────────────────────
    if (e->cena_atual == CENA_TUTORIAL) {
        // Ao clicar ESPAÇO ou ENTER, altera o estado diretamente para a gameplay limpa
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            e->cena_atual = CENA_GAMEPLAY;
        }
        return;
    }

    // ─── 4. FASE DE GAMEPLAY (TESTES DE MOBILIDADE) ──────────────────────────
    if (e->cena_atual == CENA_GAMEPLAY) {
        // Movimentação horizontal do círculo do personagem usando A/D ou Setas
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            e->jogador_pos.x -= e->jogador_vel * dt;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            e->jogador_pos.x += e->jogador_vel * dt;
        }

        // Impede o boneco de sair para fora das bordas laterais do cais
        if (e->jogador_pos.x < 30) e->jogador_pos.x = 30;
        if (e->jogador_pos.x > LARGURA - 30) e->jogador_pos.x = LARGURA - 30;
        
        return;
    }

    // ─── 5. MAPA DO RIO CAPIBARIBE ───────────────────────────────────────────
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
    
    if (e->cena_atual == CENA_ANIMACAO) {
        desenhar_animacao(e);
    } else if (e->cena_atual == CENA_MENU) {
        desenhar_menu(e);
    } else if (e->cena_atual == CENA_TUTORIAL) {
        desenhar_tutorial(e);
    } else if (e->cena_atual == CENA_GAMEPLAY) {
        desenhar_gameplay(e); 
    } else if (e->cena_atual == CENA_MAPA) {
        desenhar_mapa(e);
    } else {
        ClearBackground(BLACK);
    }
    
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