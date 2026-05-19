#include "jogo.h"
#include <math.h>

void desenhar_mapa(EstadoJogo *e) {
    ClearBackground(COR_FUNDO);
    DrawTexturePro(e->mapa,
        (Rectangle){0, 0, (float)e->mapa.width, (float)e->mapa.height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);
}

void desenhar_animacao(EstadoJogo *e) {
    ClearBackground(BLACK);
    DrawTexturePro(
        e->anim_frames[e->anim_frame_atual],
        (Rectangle){0, 0,
            (float)e->anim_frames[e->anim_frame_atual].width,
            (float)e->anim_frames[e->anim_frame_atual].height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);
}

void desenhar_menu(EstadoJogo *e) {
    ClearBackground(BLACK);
    DrawTexturePro(e->anim_frames[19],
        (Rectangle){0, 0, (float)e->anim_frames[19].width, (float)e->anim_frames[19].height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);
}

void desenhar_tutorial(EstadoJogo *e) {
    ClearBackground(BLACK);

    if (e->cenario_fundo.id > 0) {
        DrawTexturePro(e->cenario_fundo,
            (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    }

    int caixax = 150, caixay = 100, caixaw = 724, caixah = 400;
    DrawRectangle(caixax, caixay, caixaw, caixah, Fade(BLACK, 0.9f));
    DrawRectangleLines(caixax, caixay, caixaw, caixah, GOLD);

    DrawText("MANUAL DE RECONHECIMENTO", 318, caixay + 30, 28, GOLD);

    DrawText("Comandos de Mobilidade:", caixax + 50, caixay + 100, 20, GOLD);
    DrawText("Use as teclas A e D (ou SETAS) para andar pelo cais.", caixax + 50, caixay + 130, 18, WHITE);

    DrawText("Sistema de Combate:", caixax + 50, caixay + 190, 20, GOLD);
    DrawText("Use a barra de ESPACO para disparar contra os alvos.", caixax + 50, caixay + 220, 18, WHITE);

    DrawText("Seu Objetivo Atual:", caixax + 50, caixay + 280, 20, GOLD);
    DrawText("Elimine todos os alvos de teste para liberar a navegacao pelo rio.", caixax + 50, caixay + 310, 18, WHITE);

    float alpha = (sinf(e->timer * 4.0f) + 1.0f) / 2.0f;
    DrawText("Pressione ENTER ou ESPACO para iniciar os testes", 281, caixay + 350, 18, Fade(WHITE, alpha));
}

void desenhar_gameplay(EstadoJogo *e) {
    if (e->sprite_jogador.id == 0) {
        e->sprite_jogador = LoadTexture("assets/personagem.png");
    }

    float frame_largura = 0.0f;
    float frame_altura  = 0.0f;

    if (e->sprite_jogador.id > 0) {
        frame_largura = (float)e->sprite_jogador.width / 8.0f;
        frame_altura  = (float)e->sprite_jogador.height / 3.0f;
    }

    // ─── FILA VAZIA: cais livre para andar ───────────────────────────────────
    if (fila_vazia(&e->fila_ondas)) {
        ClearBackground(BLACK);

        if (e->cenario_fundo.id > 0) {
            DrawTexturePro(e->cenario_fundo,
                (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
                (Rectangle){0, 0, LARGURA, ALTURA},
                (Vector2){0, 0}, 0.0f, WHITE);
        }

        if (e->sprite_jogador.id > 0) {
            if (e->jogador_frame == 0) e->jogador_frame = 2;

            float posX_na_imagem  = e->jogador_frame * frame_largura;
            float posY_na_imagem  = e->jogador_direcao * frame_altura;
            float largura_recorte = frame_largura;

            if (e->jogador_direcao == 3) {
                posY_na_imagem   = 2 * frame_altura;
                largura_recorte  = -frame_largura;
            }

            Rectangle origem  = {posX_na_imagem, posY_na_imagem, largura_recorte, frame_altura};
            float escala      = 1.2f;
            float larg_final  = frame_largura * escala;
            float alt_final   = frame_altura  * escala;
            Rectangle destino = {
                e->jogador_pos.x - (larg_final / 2.0f),
                e->jogador_pos.y - alt_final,
                larg_final, alt_final
            };
            DrawTexturePro(e->sprite_jogador, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawCircleV(e->jogador_pos, 20, BLUE);
        }
        return;
    }

    // ─── FILA COM INIMIGOS: combate Marco Zero ───────────────────────────────
    ClearBackground(BLACK);

    if (e->jogador_tex.id > 0) {
        DrawTexturePro(e->jogador_tex,
            (Rectangle){0, 0, (float)e->jogador_tex.width, (float)e->jogador_tex.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawText("ERRO: marco_zero.png nao encontrada em assets/", 150, ALTURA/2, 20, RED);
    }

    if (e->sprite_jogador.id > 0) {
        Rectangle origem  = {2 * frame_largura, 0.0f, frame_largura, frame_altura};
        float escala      = 1.2f;
        float larg_final  = frame_largura * escala;
        float alt_final   = frame_altura  * escala;
        Rectangle destino = {
            512.0f - (larg_final / 2.0f),
            380.0f - alt_final,
            larg_final, alt_final
        };
        DrawTexturePro(e->sprite_jogador, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        DrawText("CHICO SCIENCE", 450, 395, 16, GOLD);
    } else {
        DrawCircle(512, 380, 25, PURPLE);
        DrawText("CHICO", 485, 415, 16, WHITE);
    }

    Inimigo *atual = peek_inimigo(&e->fila_ondas);
    if (atual != NULL) {
        Color cor_inimigo = (atual->id == 99) ? RED : MAROON;
        DrawCircle(512, 132, 22, cor_inimigo);
        DrawText(atual->nome, 420, 75, 20, LIGHTGRAY);
        DrawRectangle(412, 100, 200, 15, BLACK);
        float perc_hp_inimigo = (float)atual->hp / atual->hp_max;
        DrawRectangle(412, 100, (int)(200 * perc_hp_inimigo), 15, RED);
    }

    Rectangle painel_hud = {40, 450, LARGURA - 80, 130};
    DrawRectangleRec(painel_hud, (Color){10, 10, 15, 240});
    DrawRectangleLinesEx(painel_hud, 2, COR_TITULO);

    DrawText(TextFormat("CHICO SCIENCE HP: %d/%d", e->chico_hp, e->chico_hp_max), 60, 465, 22, COR_TEXTO);
    DrawText(TextFormat("Casaroes Preservados: %d/3", e->casaroes_salvos), 60, 500, 18, COR_TITULO);
    DrawText(TextFormat("Inimigos Restantes na Fila: %d", e->fila_ondas.tamanho), 60, 530, 16, GRAY);
    DrawText(e->log_combate, 450, 465, 16, ORANGE);

    if (e->turno_jogador) {
        DrawText("[1] Atacar (Guitarra)", 720, 510, 20, GREEN);
        DrawText("[2] Postura Defensiva", 720, 540, 20, COR_TEXTO);
    } else {
        DrawText("Aguardando ataque inimigo...", 720, 525, 18, RED);
    }
}

void desenhar_ponte(EstadoJogo *e) {
    ClearBackground(BLACK);

    // ─── FUNDO: cenário da ponte ─────────────────────────────────────────────
    if (e->cenario_ponte.id > 0) {
        DrawTexturePro(e->cenario_ponte,
            (Rectangle){0, 0, (float)e->cenario_ponte.width, (float)e->cenario_ponte.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    }

    // ─── RACHADURAS NA PONTE ─────────────────────────────────────────────────
    for (int i = 0; i < 5; i++) {
        if (e->ponte_posicoes[i] == 1 && e->tex_rachadura.id > 0) {
            float rw = 80.0f;
            float rh = 60.0f;
            DrawTexturePro(e->tex_rachadura,
                (Rectangle){0, 0,
                    (float)e->tex_rachadura.width,
                    (float)e->tex_rachadura.height},
                (Rectangle){
                    e->ponte_pos_rachaduras[i].x - rw / 2.0f,
                    e->ponte_pos_rachaduras[i].y - rh / 2.0f,
                    rw, rh},
                (Vector2){0, 0}, 0.0f, WHITE);
        } else if (e->ponte_posicoes[i] == 0) {
            // Indicador visual de posição intacta (número da posição)
            DrawText(TextFormat("P%d", i + 1),
                (int)e->ponte_pos_rachaduras[i].x - 8,
                (int)e->ponte_pos_rachaduras[i].y - 8,
                16, Fade(GREEN, 0.5f));
        }
    }

    // ─── PERSONAGEM (Chico Science) ──────────────────────────────────────────
    if (e->sprite_jogador.id > 0) {
        float frame_largura = (float)e->sprite_jogador.width / 8.0f;
        float frame_altura  = (float)e->sprite_jogador.height / 3.0f;

        float posX_na_imagem  = e->jogador_frame * frame_largura;
        float posY_na_imagem  = e->jogador_direcao * frame_altura;
        float largura_recorte = frame_largura;

        if (e->jogador_direcao == 3) {
            posY_na_imagem  = 2 * frame_altura;
            largura_recorte = -frame_largura;
        }

        Rectangle origem  = {posX_na_imagem, posY_na_imagem, largura_recorte, frame_altura};
        float escala      = 1.2f;
        float larg_final  = frame_largura * escala;
        float alt_final   = frame_altura  * escala;
        Rectangle destino = {
            e->jogador_pos.x - (larg_final / 2.0f),
            e->jogador_pos.y - alt_final,
            larg_final, alt_final
        };
        DrawTexturePro(e->sprite_jogador, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
    }

    // ─── HUD INFERIOR ────────────────────────────────────────────────────────
    Rectangle painel = {40, 450, LARGURA - 80, 140};
    DrawRectangleRec(painel, (Color){10, 10, 15, 230});
    DrawRectangleLinesEx(painel, 2, COR_TITULO);

    // HP do Chico
    DrawText(TextFormat("CHICO HP: %d/%d", e->chico_hp, e->chico_hp_max), 60, 460, 20, COR_TEXTO);
    int barra_hp_w = (int)(200.0f * ((float)e->chico_hp / e->chico_hp_max));
    DrawRectangle(60, 482, 200, 12, DARKGRAY);
    DrawRectangle(60, 482, barra_hp_w, 12, GREEN);

    // Energia da guitarra
    DrawText(TextFormat("GUITARRA: %d/%d", e->guitarra_energia, e->guitarra_energia_max), 60, 500, 18, COR_TITULO);
    int barra_guit_w = (int)(200.0f * ((float)e->guitarra_energia / e->guitarra_energia_max));
    DrawRectangle(60, 520, 200, 10, DARKGRAY);
    DrawRectangle(60, 520, barra_guit_w, 10, GOLD);

    // HP do sabotador
    DrawText(TextFormat("SABOTADOR HP: %d/%d", e->sabotador_hp, e->sabotador_hp_max), 300, 460, 20, RED);
    int barra_sab_w = (int)(200.0f * ((float)e->sabotador_hp / e->sabotador_hp_max));
    DrawRectangle(300, 482, 200, 12, DARKGRAY);
    DrawRectangle(300, 482, barra_sab_w, 12, RED);

    // Estado das 5 posições da ponte
    DrawText("PONTE:", 560, 460, 18, COR_TITULO);
    for (int i = 0; i < 5; i++) {
        Color cor       = e->ponte_posicoes[i] == 0 ? GREEN : RED;
        const char *est = e->ponte_posicoes[i] == 0 ? "OK" : "!!";
        DrawText(TextFormat("[P%d:%s]", i + 1, est), 560 + (i * 90), 482, 16, cor);
    }
    DrawText(TextFormat("Rachaduras: %d/5", e->ponte_rachaduras), 560, 505, 16, ORANGE);

    // Log de combate
    DrawText(e->log_combate, 60, 535, 15, ORANGE);

    // Controles
    DrawText("[ESPACO] Atacar  [1-5] Reparar posicao  [R] Recarregar guitarra", 60, 558, 14, GRAY);
}

void desenhar_desafio_final(EstadoJogo *e) {
    ClearBackground(BLACK);

    // Barras de vida
    DrawText("GUARDIAO DO CAPIBARIBE", 300, 30, 24, GOLD);

    // Vida do boss
    DrawRectangle(50, 80, 400, 25, DARKGRAY);
    DrawRectangle(50, 80, (int)(400 * ((float)e->boss_atual.vida / e->boss_atual.vida_max)), 25, RED);
    DrawText("Boss", 50, 58, 16, WHITE);

    // Vida do jogador
    DrawRectangle(574, 80, 400, 25, DARKGRAY);
    DrawRectangle(574, 80, (int)(400 * ((float)e->vida_jogador / 100)), 25, GREEN);
    DrawText("Voce", 574, 58, 16, WHITE);

    // Estado do boss
    const char *estado_txt =
        e->boss_atual.estado == ESTADO_AGRESSIVO   ? "AGRESSIVO"   :
        e->boss_atual.estado == ESTADO_CAUTELOSO   ? "CAUTELOSO"   :
                                                      "DESESPERADO";
    DrawText(estado_txt, 450, 120, 18, ORANGE);

    // Log de combate
    DrawRectangle(50, 520, 924, 50, (Color){0,0,0,180});
    DrawText(e->log_combate, 60, 535, 16, YELLOW);

    // Instrução
    const char *instrucao = e->turno_jogador ?
        "[A] Ataque forte (-20)   [D] Ataque rapido (-8)" :
        "Aguarde o inimigo...";
    DrawText(instrucao, 250, 480, 16, WHITE);
}