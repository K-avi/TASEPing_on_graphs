#ifndef MOVEMENT_H 
#define MOVEMENT_H 

#include "common.h"
#include "graph.h"
#include "entities.h"
#include "attack.h"

uint8_t iterate_ntimes(S_GRAPH * g, S_ENT_ARR * entities, S_ATTACK_REP * arep , char * fname , uint32_t nb_it , uint16_t flags);

#ifdef debug_mode 
#endif

#endif