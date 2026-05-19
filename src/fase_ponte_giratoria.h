#ifndef FASE_PONTE_GIRATORIA_H
#define FASE_PONTE_GIRATORIA_H

#include "jogo.h"

void iniciar_missao_ponte(EstadoJogo *e);
void processar_input_ponte(EstadoJogo *e);
void atualizar_ponte(EstadoJogo *e, float dt);

#endif
