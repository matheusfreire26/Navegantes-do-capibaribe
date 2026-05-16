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
    ClearBackground(BLACK);

    // Se a textura foi carregada corretamente lá do novo caminho, renderiza ela limpa
    if (e->cenario_fundo.id > 0) {
        DrawTexturePro(e->cenario_fundo,
            (Rectangle){0, 0, (float)e->cenario_fundo.width, (float)e->cenario_fundo.height},
            (Rectangle){0, 0, LARGURA, ALTURA},
            (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        // Diagnóstico visual caso ainda dê algum erro de carregamento
        DrawText("ERRO: tutorial.png nao encontrada em assets/abertura/", 150, ALTURA/2, 20, RED);
    }

    // Desenha o jogador (Círculo Azul) controlável sobre a imagem
    DrawCircleV(e->jogador_pos, 20, BLUE); 
    DrawCircleLinesV(e->jogador_pos, 20, WHITE);
}