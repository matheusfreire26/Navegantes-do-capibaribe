#include "desafio3.h"
#include <string.h>
#include <stdlib.h>

void iniciar_missao_ponte(EstadoJogo *e) {
    e->chico_hp_max          = 100;
    e->chico_hp              = e->chico_hp_max;
    e->turno_jogador         = true;
    e->tempo_missao          = 0.0f;
    e->ponte_rachaduras      = 0;
    e->ponte_turno_sabotador = 0;
    e->guitarra_energia_max  = 100;
    e->guitarra_energia      = e->guitarra_energia_max;
    e->sabotador_hp_max      = 200;
    e->sabotador_hp          = e->sabotador_hp_max;
    e->sabotador_dano        = 18;

    e->ponte_pos_rachaduras[0] = (Vector2){120, 300};
    e->ponte_pos_rachaduras[1] = (Vector2){280, 280};
    e->ponte_pos_rachaduras[2] = (Vector2){460, 290};
    e->ponte_pos_rachaduras[3] = (Vector2){640, 285};
    e->ponte_pos_rachaduras[4] = (Vector2){820, 295};

    for (int i = 0; i < 5; i++)
        e->ponte_posicoes[i] = 0;

    e->jogador_pos     = (Vector2){512, 400};
    e->jogador_vel     = 250.0f;
    e->jogador_frame   = 2;
    e->jogador_direcao = 0;
    e->jogador_timer   = 0.0f;

    strcpy(e->log_combate, "O sabotador avanca! Repare a ponte e lute!");
}

void processar_input_ponte(EstadoJogo *e) {
    if (e->sabotador_hp <= 0) return;

    float dt  = GetFrameTime();
    float vel = e->jogador_vel * dt;
    bool movendo = false;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        e->jogador_pos.x += vel;
        e->jogador_direcao = 2;
        movendo = true;
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        e->jogador_pos.x -= vel;
        e->jogador_direcao = 3;
        movendo = true;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        e->jogador_pos.y += vel;
        e->jogador_direcao = 0;
        movendo = true;
    } else if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        e->jogador_pos.y -= vel;
        e->jogador_direcao = 1;
        movendo = true;
    }

    if (e->jogador_pos.x < 40.0f)  e->jogador_pos.x = 40.0f;
    if (e->jogador_pos.x > 980.0f) e->jogador_pos.x = 980.0f;
    if (e->jogador_pos.y < 250.0f) e->jogador_pos.y = 250.0f;
    if (e->jogador_pos.y > 490.0f) e->jogador_pos.y = 490.0f;

    if (movendo) {
        if (e->jogador_frame < 3) e->jogador_frame = 3;
        e->jogador_timer += dt;
        if (e->jogador_timer >= 0.12f) {
            e->jogador_timer = 0.0f;
            e->jogador_frame++;
            if (e->jogador_frame > 5) e->jogador_frame = 3;
        }
    } else {
        e->jogador_frame = 2;
    }

    if (!e->turno_jogador) return;

    if (IsKeyPressed(KEY_SPACE)) {
        if (e->guitarra_energia >= 25) {
            e->guitarra_energia -= 25;
            int dano = 30;
            e->sabotador_hp -= dano;
            if (e->sabotador_hp < 0) e->sabotador_hp = 0;
            snprintf(e->log_combate, sizeof(e->log_combate),
                "Chico disparou notas! -%d HP no sabotador. [%d/%d]",
                dano, e->sabotador_hp, e->sabotador_hp_max);
        } else {
            strcpy(e->log_combate, "Guitarra sem energia! Pressione R para recarregar.");
        }
        e->turno_jogador = false;
        return;
    }

    int teclas[] = {KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE};
    for (int i = 0; i < 5; i++) {
        if (IsKeyPressed(teclas[i])) {
            if (e->ponte_posicoes[i] == 1) {
                e->ponte_posicoes[i] = 0;
                e->ponte_rachaduras--;
                snprintf(e->log_combate, sizeof(e->log_combate),
                    "P%d reparada! Rachaduras ativas: %d/5", i + 1, e->ponte_rachaduras);
            } else {
                snprintf(e->log_combate, sizeof(e->log_combate),
                    "P%d ja esta intacta.", i + 1);
            }
            e->turno_jogador = false;
            return;
        }
    }

    if (IsKeyPressed(KEY_R)) {
        e->guitarra_energia = e->guitarra_energia_max;
        strcpy(e->log_combate, "Guitarra recarregada! Turno perdido.");
        e->turno_jogador = false;
        return;
    }
}

void atualizar_ponte(EstadoJogo *e, float dt) {
    (void)dt;

    if (e->sabotador_hp <= 0) {
        e->cena_atual = CENA_MAPA;
        if (e->atual != NULL) e->atual->visitado = 1;
        return;
    }

    if (e->ponte_rachaduras >= 5) {
        strcpy(e->log_combate, "A ponte desabou! GAME OVER.");
        e->cena_atual = CENA_MENU;
        return;
    }

    if (!e->turno_jogador) {
        e->ponte_turno_sabotador++;

        int tentativas = 0;
        while (tentativas < 5) {
            int pos = rand() % 5;
            if (e->ponte_posicoes[pos] == 0) {
                e->ponte_posicoes[pos] = 1;
                e->ponte_rachaduras++;
                break;
            }
            tentativas++;
        }

        if (e->ponte_turno_sabotador % 2 == 0) {
            e->chico_hp -= e->sabotador_dano;
            if (e->chico_hp < 0) e->chico_hp = 0;
            snprintf(e->log_combate, sizeof(e->log_combate),
                "Sabotador atacou! -%d HP. Chico: %d/%d",
                e->sabotador_dano, e->chico_hp, e->chico_hp_max);

            if (e->chico_hp <= 0) {
                e->cena_atual = CENA_MENU;
                return;
            }
        }

        e->turno_jogador = true;
    }
}
