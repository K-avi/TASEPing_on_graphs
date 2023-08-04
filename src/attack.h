#ifndef ATTACK_H 
#define ATTACK_H 

#include "common.h"
#include "graph.h"


typedef struct{ 

    uint32_t array_size; 
    uint32_t * arr_attacked_lines; //the indexes of the lines that are attacked 

}S_ATTACK_ARRAY;

typedef struct{ //this structure is a simple way 
//to store "multiple attacks" 

    uint32_t array_size; //size of the array
    uint32_t cur_attack; //current attack, updated at each attack 

    uint32_t * arr_attack_ite; //iteration of each attack
    S_ATTACK_ARRAY * arr_attack_array; //array of attack arrays structure 
    
}S_ATTACK_REP;
//actually don't do it like so ; 
//smarter to only have two arrays : 

//one contaning every attack array ever and one w the size of each attack (index where it begins )



void free_attack_rep( S_ATTACK_REP * attr);

uint8_t load_attack_rep(S_ATTACK_REP * arep , char * source_file);
uint8_t write_attack_rep(S_ATTACK_REP * arep , char * dest_file);

uint8_t update_attacked_links(S_GRAPH * g, S_ATTACK_REP * arep);

#ifdef debug_mode 

uint8_t init_attack_array(S_ATTACK_ARRAY * arr , uint32_t array_size);
uint8_t init_attack_rep( S_ATTACK_REP * attr, uint32_t array_size);
uint8_t app_attack_rep(S_ATTACK_REP * attr , uint32_t array_size , uint32_t * arr_attacked_lines, uint32_t iteration_attack, uint32_t index);

#endif 

#endif