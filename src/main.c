#include "jogo.h"
#include "desafio1.h"
#include "desafio3.h"

int main(void) {
    EstadoJogo e = {0};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "Navegantes do Capibaribe");

    e.target = LoadRenderTexture(LARGURA, ALTURA);
    SetTextureFilter(e.target.texture, TEXTURE_FILTER_BILINEAR);

    // Frames de animação de abertura
    for (int i = 0; i < 20; i++) {
        char caminho[64];
        snprintf(caminho, sizeof(caminho), "assets/abertura/%d.png", i + 1);
        e.anim_frames[i] = LoadTexture(caminho);
    }

    e.anim_frame_atual   = 0;
    e.anim_frame_timer   = 0.0f;
    e.anim_frame_duracao = 0.1f;
    e.anim_duracao       = 2.0f;
    e.cena_atual         = CENA_ANIMACAO;
    e.menu_opcao         = 0;

    SetTargetFPS(60);

    e.fonte        = GetFontDefault();
    e.mapa         = LoadTexture("assets/mapa.png");
    e.cenario_fundo  = LoadTexture("assets/tutorial.png");
    e.sprite_jogador = LoadTexture("assets/personagem.png");
    e.jogador_tex    = LoadTexture("assets/marco_zero.png");

    // ─── Texturas da Ponte ────────────────────────────────────────────────────
    e.cenario_ponte = LoadTexture("assets/ponte.png");
    e.tex_rachadura = LoadTexture("assets/rachadura.png");

    construir_rio(&e);

    while (!WindowShouldClose()) {
        processar_input(&e);
        atualizar(&e);
        renderizar(&e);
    }

    liberar_rio(&e);

    UnloadTexture(e.mapa);
    UnloadTexture(e.cenario_fundo);
    UnloadTexture(e.sprite_jogador);
    UnloadTexture(e.jogador_tex);
    UnloadTexture(e.cenario_ponte);
    UnloadTexture(e.tex_rachadura);

    for (int i = 0; i < 20; i++)
        UnloadTexture(e.anim_frames[i]);

    UnloadRenderTexture(e.target);
    CloseWindow();
    return 0;
}