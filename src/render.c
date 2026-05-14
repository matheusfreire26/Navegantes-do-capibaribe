#include "jogo.h"

void desenhar_mapa(EstadoJogo *e) {
    ClearBackground(COR_FUNDO);
    DrawTexturePro(e->mapa,
        (Rectangle){0, 0, (float)e->mapa.width, (float)e->mapa.height},
        (Rectangle){0, 0, LARGURA, ALTURA},
        (Vector2){0, 0}, 0.0f, WHITE);
}

void desenhar_nos(EstadoJogo *e) {
    PontoTuristico *p = e->cabeca;
    int idx = 0;
    while (p) {
        Color cor = (p == e->atual) ? COR_NO_ATIVO    :
                    p->visitado     ? COR_NO_VISITADO  :
                                      COR_NO_INATIVO;

        // Sombra
        DrawCircleV((Vector2){p->pos.x + 4, p->pos.y + 4}, 22,
                    (Color){0, 0, 0, 80});
        // Círculo
        DrawCircleV(p->pos, 22, cor);
        DrawCircleLines((int)p->pos.x, (int)p->pos.y, 22,
                        (Color){255, 255, 255, 60});

        // Número
        char num[4];
        snprintf(num, sizeof(num), "%02d", idx + 1);
        Vector2 sz = MeasureTextEx(e->fonte, num, 18, 1);
        DrawTextEx(e->fonte, num,
                   (Vector2){p->pos.x - sz.x / 2, p->pos.y - sz.y / 2},
                   18, 1, COR_FUNDO);

        // Nome abaixo
        Vector2 sn = MeasureTextEx(e->fonte, p->nome, 14, 1);
        DrawTextEx(e->fonte, p->nome,
                   (Vector2){p->pos.x - sn.x / 2, p->pos.y + 28},
                   14, 1, COR_TEXTO);

        p = p->proximo;
        idx++;
    }
}

void desenhar_barco(EstadoJogo *e) {
    float bx  = e->barco_x;
    float by  = e->barco_y;
    float bob = sinf(e->timer * 3.0f) * 3.0f;

    // Casco
    DrawEllipse((int)bx, (int)(by + 14 + bob), 24, 8, COR_BARCO);
    // Mastro
    DrawLineEx((Vector2){bx, by + 4 + bob},
               (Vector2){bx, by - 20 + bob}, 2,
               (Color){160, 100, 40, 255});
    // Vela
    Vector2 vela[3] = {
        {bx,      by - 18 + bob},
        {bx + 18, by + 4  + bob},
        {bx,      by + 4  + bob}
    };
    DrawTriangle(vela[0], vela[1], vela[2],
                 (Color){230, 220, 200, 220});
}

void desenhar_painel(EstadoJogo *e) {
    if (!e->atual) return;

    int px = 20, py = ALTURA - 130, pw = 400, ph = 110;
    DrawRectangleRounded((Rectangle){px, py, pw, ph}, 0.12f, 8,
                         (Color){10, 20, 40, 210});
    DrawRectangleRoundedLines((Rectangle){px, py, pw, ph}, 0.12f, 8,
                              COR_NO_ATIVO);

    DrawTextEx(e->fonte, e->atual->nome,
               (Vector2){px + 12, py + 10}, 20, 1, COR_TITULO);
    DrawTextEx(e->fonte, e->atual->subtitulo,
               (Vector2){px + 12, py + 36}, 14, 1, COR_TEXTO);

    char linha[128];
    snprintf(linha, sizeof(linha), "Artefato: %s", e->atual->artefato);
    DrawTextEx(e->fonte, linha,
               (Vector2){px + 12, py + 58}, 13, 1,
               (Color){180, 220, 160, 255});

    const char *nav = "";
    if (e->atual->anterior && e->atual->proximo)
        nav = "[A] Voltar   [D] Avancar";
    else if (e->atual->proximo)
        nav = "[D] Avancar";
    else
        nav = "[A] Voltar";
    DrawTextEx(e->fonte, nav,
               (Vector2){px + 12, py + 82}, 13, 1,
               (Color){150, 150, 180, 255});
}

void desenhar_titulo(EstadoJogo *e) {
    const char *titulo = "NAVEGANTES DO CAPIBARIBE";
    Vector2 sz = MeasureTextEx(e->fonte, titulo, 28, 2);
    DrawTextEx(e->fonte, titulo,
               (Vector2){LARGURA / 2 - sz.x / 2, 18}, 28, 2, COR_TITULO);

    const char *sub = "RPG 2D de Exploracao Historica - Recife Antigo";
    Vector2 ss = MeasureTextEx(e->fonte, sub, 14, 1);
    DrawTextEx(e->fonte, sub,
               (Vector2){LARGURA / 2 - ss.x / 2, 54}, 14, 1,
               (Color){180, 160, 120, 200});
}

void desenhar_animacao(EstadoJogo *e) {
    ClearBackground(BLACK);
    DrawTexturePro(
        e->anim_frames[e->anim_frame_atual],
        (Rectangle){0, 0,
            e->anim_frames[e->anim_frame_atual].width,
            e->anim_frames[e->anim_frame_atual].height},
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