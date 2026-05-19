#ifndef FASE_TORRE_CRISTAL_H
#define FASE_TORRE_CRISTAL_H

#include "jogo.h"

void iniciar_missao_torre(EstadoJogo *e);
void processar_input_torre(EstadoJogo *e);
void atualizar_torre(EstadoJogo *e, float dt);

#endif
