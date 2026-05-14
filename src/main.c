#include "jogo.h"

int main(void) {
    EstadoJogo e = {0};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "Navegantes do Capibaribe");
    e.target = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(e.target.texture, TEXTURE_FILTER_BILINEAR);
    for (int i = 0; i < 9; i++) {
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "assets/abertura/%d.png", i + 1);
    e.anim_frames[i] = LoadTexture(caminho);
}

// Carrega menu — coloque o nome correto do seu PNG
e.mapa     = LoadTexture("assets/9.png");
    e.anim_frame_atual    = 0;
    e.anim_frame_timer    = 0.0f;
    e.anim_frame_duracao  = 0.1f;
    e.anim_duracao        = 9 * 1.0f;
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
    UnloadRenderTexture(e.target);
    CloseWindow();

    return 0;
}