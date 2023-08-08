#include "movement.h"
#include "attack.h"
#include "common.h"
#include "entities.h"
#include "graph.h"
#include "misc.h"
#include "rules.h"
#include <stdint.h>
#include <stdio.h>


static uint8_t prepare_ite(S_GRAPH * g, S_ENT_ARR * entities, S_ATTACK_REP * arep){
    /*updates fields relevant to the iterations before they actually take place*/
    if(!g){report_err("prepare_ite",  G_NULL); return G_NULL;}
    if(!entities){report_err("prepare_ite", ET_NULL); return ET_NULL;}
    if(!arep){report_err("prepare_ite", AREP_NULL); return AREP_NULL; } 

    entities->moved_entities = 0; 
    SHUFFLE_ARR_INDEX(entities);
    g->curgen++;

    if(g->curgen == arep->arr_start_of_attack_ite[arep->cur_attack]){//updates blocked link attributes 
        update_attacked_links(g, arep);
    }
    
    return IT_OK;
}//not tested; error prone


static uint8_t iterate_once(S_GRAPH * g , S_ENT_ARR * entities){
    /*pretty simple tbh*/
    if(!g){report_err("iterate_once",  G_NULL); return G_NULL;}
    if(!entities){report_err("iterate_once", ET_NULL); return ET_NULL;}
    uint8_t failure;
    for(uint32_t i = 0 ; i < entities->nb_entities; i++){
        failure = rule_tasep(g, entities, entities->entity_curpos[entities->shuffled_indexes_arr[i]],
                     entities->shuffled_indexes_arr[i]);
        if(failure){ report_err("iterate_once", failure); return failure;}
    }

    return IT_OK;
}//not tested; 


uint8_t iterate_ntimes(S_GRAPH * g, S_ENT_ARR * entities, S_ATTACK_REP * arep , char * fname , uint32_t nb_it , uint16_t flags){
    /*kinda a wrapper around iterate once*/
    if(!g){report_err("iterate_ntimes",  G_NULL); return G_NULL;}
    if(!entities){report_err("iterate_ntimes", ET_NULL); return ET_NULL;}
    if(!arep){report_err("iterate_ntimes", AREP_NULL); return AREP_NULL; } 

    uint8_t failure; 
    FILE * f = fopen(fname, "w");

    for(uint32_t i = 0; i < nb_it; i++){
        failure = prepare_ite(g, entities, arep);
        if(failure){report_err("iterate_ntimes", failure); return failure;}
        
        //flags thing 
        fprintf(f, "%u\n", entities->moved_entities);

        failure = iterate_once(g, entities);
        if(failure){report_err("iterate_ntimes", failure); return failure;}
    }

    fclose(f);

    return IT_OK;
}//not done 