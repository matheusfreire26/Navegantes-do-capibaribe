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

    // TEMPORÁRIO — coordenadas do mouse
    Vector2 mouse = GetMousePosition();
    char pos[64];
    snprintf(pos, sizeof(pos), "X: %.0f  Y: %.0f", mouse.x, mouse.y);
    DrawText(pos, 10, 10, 20, RED);
}

void desenhar_tutorial(EstadoJogo *e) {
    // 1. Desenha a imagem do cais no fundo (por baixo da cobertura)
    DrawTexturePro(e->cenario_fundo,
        (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);

    // 2. "Cobre" a tela com uma camada preta semi-transparente (75% de opacidade)
    DrawRectangle(0, 0, LARGURA, ALTURA, Fade(BLACK, 0.75f));

    // 3. Desenha a caixa de texto estilizada para o tutorial
    int caixax = 150, caixay = 100, caixaw = 724, caixah = 400;
    DrawRectangle(caixax, caixay, caixaw, caixah, Fade(BLACK, 0.8f));
    DrawRectangleLines(caixax, caixay, caixaw, caixah, COR_TITULO);

    // 4. Instruções do Tutorial - Posições pré-calculadas para evitar MeasureText a cada frame
    // Título centralizado: "MANUAL DE RECONHECIMENTO", 28 pixels de tamanho
    DrawText("MANUAL DE RECONHECIMENTO", 318, caixay + 30, 28, COR_TITULO);
    
    DrawText("Comandos de Mobilidade:", caixax + 50, caixay + 100, 20, GOLD);
    DrawText("Use as teclas A e D (ou SETAS) para andar pelo cais.", caixax + 50, caixay + 130, 18, COR_TEXTO);

    DrawText("Sistema de Combate:", caixax + 50, caixay + 190, 20, GOLD);
    DrawText("Use a barra de ESPAÇO para disparar contra os alvos.", caixax + 50, caixay + 220, 18, COR_TEXTO);

    DrawText("Seu Objetivo Atual:", caixax + 50, caixay + 280, 20, GOLD);
    DrawText("Elimine todos os alvos de teste para liberar a navegação pelo rio.", caixax + 50, caixay + 310, 18, COR_TEXTO);

    // Texto de avançar centralizado, com efeito de piscar (oscila alpha entre 0 e 1)
    float alpha = (sinf(e->timer * 4.0f) + 1.0f) / 2.0f; 
    DrawText("Pressione ENTER ou ESPAÇO para iniciar os testes", 281, caixay + 350, 18, Fade(WHITE, alpha));
}

void desenhar_gameplay(EstadoJogo *e) {
    // Limpa a tela com preto primeiro para evitar fantasmas visuais ou travamentos de buffer
    ClearBackground(BLACK);

    // Se a textura foi carregada corretamente no main.c, desenha ela ocupando a tela inteira
    if (e->cenario_fundo.id > 0) {
        DrawTexturePro(e->cenario_fundo,
            (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    }

    // HUD superior
    DrawRectangle(0, 0, LARGURA, 40, Fade(BLACK, 0.6f));
    DrawText("FASE DE TESTES: MOVA-SE COM 'A' E 'D'", 20, 10, 18, GREEN);

    // Desenha o jogador (Círculo Azul)
    DrawCircleV(e->jogador_pos, 20, BLUE); 
    DrawCircleLinesV(e->jogador_pos, 20, WHITE);
}