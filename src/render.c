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
    DrawTexturePro(e->anim_frames[8],
        (Rectangle){0, 0, (float)e->anim_frames[8].width, (float)e->anim_frames[8].height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);

    // TEMPORÁRIO — coordenadas do mouse para ajuste
    Vector2 mouse = GetMousePosition();
    char pos[64];
    snprintf(pos, sizeof(pos), "X: %.0f  Y: %.0f", mouse.x, mouse.y);
    DrawText(pos, 10, 10, 20, RED);
}

void desenhar_tutorial(EstadoJogo *e) {
    ClearBackground(BLACK);

    // Tela preta com o texto do Manual explicativo
    int caixax = 150, caixay = 100, caixaw = 724, caixah = 400;
    DrawRectangle(caixax, caixay, caixaw, caixah, Fade(BLACK, 0.9f));
    DrawRectangleLines(caixax, caixay, caixaw, caixah, GOLD);

    DrawText("MANUAL DE RECONHECIMENTO", 318, caixay + 30, 28, GOLD);
    
    DrawText("Comandos de Mobilidade:", caixax + 50, caixay + 100, 20, GOLD);
    DrawText("Use as teclas A e D (ou SETAS) para andar pelo cais.", caixax + 50, caixay + 130, 18, WHITE);

    DrawText("Sistema de Combate:", caixax + 50, caixay + 190, 20, GOLD);
    DrawText("Use a barra de ESPAÇO para disparar contra os alvos.", caixax + 50, caixay + 220, 18, WHITE);

    DrawText("Seu Objetivo Atual:", caixax + 50, caixay + 280, 20, GOLD);
    DrawText("Elimine todos os alvos de teste para liberar a navegação pelo rio.", caixax + 50, caixay + 310, 18, WHITE);

    float alpha = (sinf(e->timer * 4.0f) + 1.0f) / 2.0f; 
    DrawText("Pressione ENTER ou ESPAÇO para iniciar os testes", 281, caixay + 350, 18, Fade(WHITE, alpha));
}

void desenhar_gameplay(EstadoJogo *e) {
    if (e->sprite_jogador.id == 0) {
        e->sprite_jogador = LoadTexture("assets/abertura/personagem.png");
    }

    // Calcula as dimensões de um único frame recortado do Chico
    float frame_largura = 0.0f;
    float frame_altura = 0.0f;
    
    if (e->sprite_jogador.id > 0) {
        frame_largura = (float)e->sprite_jogador.width / 8.0f;
        frame_altura = (float)e->sprite_jogador.height / 3.0f; 
    }

    // ─── MOMENTO 1: SE A FILA ESTIVER VAZIA (Cais Livre para andar) ────────
    if (fila_vazia(&e->fila_ondas)) {
        ClearBackground(BLACK);

        if (e->cenario_fundo.id > 0) {
            DrawTexturePro(e->cenario_fundo,
                (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
                (Rectangle){0, 0, LARGURA, ALTURA},
                (Vector2){0, 0}, 0.0f, WHITE);
        }

        // DESENHO DO CHICO:
        if (e->sprite_jogador.id > 0) {
            if (e->jogador_frame == 0) e->jogador_frame = 2; 

            float posX_na_imagem = e->jogador_frame * frame_largura;
            float posY_na_imagem = e->jogador_direcao * frame_altura;

            float largura_recorte = frame_largura;
            if (e->jogador_direcao == 3) { 
                posY_na_imagem = 2 * frame_altura; 
                largura_recorte = -frame_largura; 
            }

            Rectangle origem = { posX_na_imagem, posY_na_imagem, largura_recorte, frame_altura };
            
            float escala = 1.2f;
            float largura_final = frame_largura * escala;
            float altura_final = frame_altura * escala;

            Rectangle destino = { 
                e->jogador_pos.x - (largura_final / 2.0f), 
                e->jogador_pos.y - altura_final, 
                largura_final, 
                altura_final 
            }; 
            // Deixamos o pivô em (0,0) para evitar que o espelhamento jogue o Chico para fora da tela
            DrawTexturePro(e->sprite_jogador, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawCircleV(e->jogador_pos, 20, BLUE); 
        }
        return;
    }

    // ─── MOMENTO 2: SE A FILA TIVER INIMIGOS (Combate no Marco Zero) ────────
    ClearBackground(BLACK);

    if (e->jogador_tex.id > 0) {
        DrawTexturePro(e->jogador_tex,
            (Rectangle){0, 0, (float)e->jogador_tex.width, (float)e->jogador_tex.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        DrawText("ERRO: marco_zero.png nao encontrada em assets/", 150, ALTURA/2, 20, RED);
    }

    // TROCA DO CÍRCULO ROXO PELO CHICO SCIENCE NO MEIO DA ARENA:
    if (e->sprite_jogador.id > 0) {
        Rectangle origem = { 2 * frame_largura, 0.0f, frame_largura, frame_altura };
        
        float escala = 1.2f; // Mesma escala do cais
        float largura_final = frame_largura * escala;
        float altura_final = frame_altura * escala;

        Rectangle destino = { 
            512.0f - (largura_final / 2.0f), 
            380.0f - altura_final, 
            largura_final, 
            altura_final 
        };
        
        DrawTexturePro(e->sprite_jogador, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        DrawText("CHICO SCIENCE", 450, 395, 16, GOLD);
    } else {
        DrawCircle(512, 380, 25, PURPLE);
        DrawText("CHICO", 485, 415, 16, WHITE);
    }

    // Renderização do Inimigo Atual da Fila
    Inimigo *atual = peek_inimigo(&e->fila_ondas);
    if (atual != NULL) {
        Color cor_inimigo = (atual->id == 99) ? RED : MAROON;
        DrawCircle(512, 132, 22, cor_inimigo);
        DrawText(atual->nome, 420, 75, 20, LIGHTGRAY);
        
        DrawRectangle(412, 100, 200, 15, BLACK);
        float perc_hp_inimigo = (float)atual->hp / atual->hp_max;
        DrawRectangle(412, 100, (int)(200 * perc_hp_inimigo), 15, RED);
    }

    // Interface HUD / Menu de turnos inferior
    Rectangle painel_hud = { 40, 450, LARGURA - 80, 130 };
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