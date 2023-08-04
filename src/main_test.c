#include "attack.h"
#include "common.h"
#include "entities.h"
#include "graph.h"
#include "movement.h"
#include "rules.h"
#include "misc.h"
#include <stdint.h>
#include <stdio.h>


int main(){ 
    printf("I am in fact NOT a real main\n"); 
    
    S_GRAPH g ; 

    load_graph(&g, "toy_graph.csv",  0, 2);
    print_graph(&g, stdout);

    S_ENT_ARR e;

    init_entity_arr(&e, 70); 

    S_ATTACK_REP arep ; 

    init_attack_rep(&arep, 5); 

    uint32_t* i = malloc(15*sizeof(uint32_t));
    
    app_attack_rep(&arep, 15, i, 1, 0);


    free_attack_rep(&arep);
    free_entity_arr(&e);  
    free_graph(&g);

    return 0;
}