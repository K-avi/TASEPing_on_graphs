#ifndef ENTITIES_H 
#define ENTITIES_H 

#include "common.h"


typedef uint32_t entity_pos;
typedef struct{

    uint32_t nb_entities ; //size of the array 
    uint32_t moved_entities; //number of entities 
    //moved during an iteration 

    entity_pos * entity_curpos ; //current pos of the entities 
    entity_pos * entity_prevpos ;  //previous pos of the entities so that they don't go back 

    uint32_t * shuffled_indexes_arr; //shuffle indexes to iterate in different orders (async stuff )

}S_ENT_ARR;


//basic init stuff 
uint8_t init_entity_arr(S_ENT_ARR * arr, uint32_t size);
void free_entity_arr(S_ENT_ARR * arr);

//shuffle the index array
void fisher_yates_shuffle( uint32_t  size, uint32_t * a);
#define SHUFFLE_ARR_INDEX(arr) fisher_yates_shuffle((arr)->nb_entities, (arr)->shuffled_indexes_arr)

#ifdef debug_mode 

void print_ent_arr(S_ENT_ARR * arr, FILE * stream);

#endif

#endif 