#include "jogo.h"

int main(void) {
    EstadoJogo e = {0};

    InitWindow(LARGURA, ALTURA, "Navegantes do Capibaribe");
    for (int i = 0; i < 9; i++) {
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "assets/abertura/%d.png", i + 1);
    e.anim_frames[i] = LoadTexture(caminho);
    }
    e.anim_frame_atual    = 0;
    e.anim_frame_timer    = 0.0f;
    e.anim_frame_duracao  = 0.1f;
    e.anim_duracao        = 9 * 0.1f;
    e.cena_atual  = CENA_ANIMACAO;
    e.menu_opcao  = 0;
    SetTargetFPS(60);

    e.fonte = GetFontDefault();
    e.mapa  = LoadTexture("assets/mapa.png");

    construir_rio(&e);

    while (!WindowShouldClose()) {
        processar_input(&e);
        atualizar(&e);
        renderizar(&e);
    }

    liberar_rio(&e);
    UnloadTexture(e.mapa);

    for (int i = 0; i < 9; i++)
    UnloadTexture(e.anim_frames[i]);
    
    CloseWindow();

    return 0;
}