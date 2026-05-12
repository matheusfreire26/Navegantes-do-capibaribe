#include "jogo.h"

PontoTuristico *criar_parada(const char *nome, const char *subtitulo,
                              const char *artefato, Vector2 pos) {
    PontoTuristico *p = (PontoTuristico *)malloc(sizeof(PontoTuristico));
    strncpy(p->nome,      nome,      99);
    strncpy(p->subtitulo, subtitulo, 99);
    strncpy(p->artefato,  artefato,  99);
    p->visitado = 0;
    p->pos      = pos;
    p->anterior = NULL;
    p->proximo  = NULL;
    return p;
}

void construir_rio(EstadoJogo *e) {
    // ── Ajuste as posições para bater com o seu PNG ──
    Vector2 posicoes[NUM_NOS] = {
        {120, 320},
        {340, 230},
        {620, 300},
        {880, 220}
    };
    const char *nomes[NUM_NOS] = {
        "Marco Zero",
        "Torre Malakoff",
        "Patio de Sao Pedro",
        "Casa da Cultura"
    };
    const char *subtitulos[NUM_NOS] = {
        "Coracao colonial do Recife",
        "Ciencia e arte flamenga",
        "Ritmo e resistencia",
        "Memoria e liberdade"
    };
    const char *artefatos[NUM_NOS] = {
        "Anzol Sagrado",
        "Luneta Holandesa",
        "Pandeiro Ancestral",
        "Corrente Quebrada"
    };

    PontoTuristico *anterior = NULL;
    for (int i = 0; i < NUM_NOS; i++) {
        PontoTuristico *p = criar_parada(nomes[i], subtitulos[i],
                                         artefatos[i], posicoes[i]);
        if (anterior) {
            anterior->proximo = p;
            p->anterior       = anterior;
        } else {
            e->cabeca = p;
        }
        anterior = p;
    }

    e->atual         = e->cabeca;
    e->atual->visitado = 1;
    e->barco_x       = e->cabeca->pos.x;
    e->barco_y       = e->cabeca->pos.y - 20.0f;
    e->barco_alvo_x  = e->barco_x;
}

void liberar_rio(EstadoJogo *e) {
    PontoTuristico *p = e->cabeca;
    while (p) {
        PontoTuristico *prox = p->proximo;
        free(p);
        p = prox;
    }
    e->cabeca = NULL;
    e->atual  = NULL;
}

void remar_frente(EstadoJogo *e) {
    if (e->atual->proximo && !e->animando) {
        e->atual           = e->atual->proximo;
        e->atual->visitado = 1;
        e->barco_alvo_x    = e->atual->pos.x;
        e->animando        = true;
    }
}

void remar_volta(EstadoJogo *e) {
    if (e->atual->anterior && !e->animando) {
        e->atual        = e->atual->anterior;
        e->barco_alvo_x = e->atual->pos.x;
        e->animando     = true;
    }
}