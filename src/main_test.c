#include "common.h"
#include "entities.h"
#include "graph.h"
#include "movement.h"
#include "rules.h"
#include "misc.h"
#include <stdio.h>


int main(){ 
    printf("I am in fact NOT a real main\n"); 
    
    S_GRAPH g ; 

   
    load_graph(&g, "toy_graph.csv",  0, 2);
    print_graph(&g, stdout);
      
    free_graph(&g);
    return 0;
}