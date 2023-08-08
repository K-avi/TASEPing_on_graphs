#ifndef ATTACK_H 
#define ATTACK_H 

#include "common.h"
#include "graph.h"
#include <stdint.h>



typedef struct{ 

    uint32_t nb_lines_attacked; 
    uint32_t * ref_first_line_attacked;
}S_ATTACK_START;

typedef struct{ //this structure is a simple way 
//to store "multiple attacks" 

    
    uint32_t cur_attack; //current attack index, updated at each attack 

    uint32_t size_start_arrays; //size of the next two arrays
    uint32_t * arr_start_of_attack_ite ; //iteration at which each attack start
    S_ATTACK_START * arr_start_attacks ; //index of the start of each attack in the attack array
   
    uint32_t size_attack_array; //size of the attack array
    uint32_t * arr_attack_array; //array of every line attack ever ; to know when an array  starts / ends
    //use arr_start_attacks 
    
}S_ATTACK_REP;





void free_attack_rep( S_ATTACK_REP * arep);

uint8_t load_attack_rep(S_ATTACK_REP * arep , char * source_file);
uint8_t write_attack_rep(S_ATTACK_REP * arep , char * dest_file);

uint8_t print_attack_rep(S_ATTACK_REP * arep , FILE * stream);

uint8_t update_attacked_links(S_GRAPH * g, S_ATTACK_REP * arep);

#ifdef debug_mode 


uint8_t init_attack_rep( S_ATTACK_REP * attr, uint32_t attack_array_size, uint32_t start_array_size);
uint8_t app_attack_rep(S_ATTACK_REP * attr , uint32_t array_size , uint32_t * arr_attacked_lines, uint32_t iteration_attack, uint32_t index);

#endif 

#endif