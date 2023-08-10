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

    load_graph(&g, "paris_final_gendown.csv",  0, 2);
   // print_graph(&g, stdout);

    S_ENT_ARR e;

    init_entity_arr(&e, 70); 

    S_ATTACK_REP arep ; 

    load_attack_rep(&arep, "test_attack_arr.csv");
    
    print_attack_rep(&arep, stdout);


    free_attack_rep(&arep);
    free_entity_arr(&e);  
    free_graph(&g);

    return 0;
}