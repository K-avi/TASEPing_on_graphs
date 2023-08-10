#include "rules.h"
#include "graph.h"
#include <stdint.h>


uint8_t rule_tasep(S_GRAPH * g , S_ENT_ARR * ent_arr, uint32_t node_from , uint32_t entity_index){
    /*
    moves the entity at entity_index fril the node_from node to 
    one of it's neighbor if allowed (max capacity isn't reached and 
    link isn't broken) 

    if the entity can not move it will sleep and will NOT update it's
    previous position and current position
    */

    uint32_t nodes_to_arr[g->node_arr[node_from].neighboor_num];
    uint32_t cur_in_nta = 0 ;

    for(uint32_t i = 0 ; i < g->node_arr[node_from].neighboor_num; i++){

        Line * cur_line = (g->node_arr[node_from].first_neighboor_ref + i);
        uint32_t cur_line_index =  cur_line - g->line_rep->line_arr;

        if(! g->line_rep->blocked_arr[cur_line_index]){//if blocked arr is set ; excludes movement possibility

          
            uint32_t cur_neighbor_index = cur_line->node_index;
            
        
            if( (g->node_saturation_arr[cur_neighbor_index] < (g->saturation_coeff * g->node_arr[cur_neighbor_index].neighboor_num) )
                && (cur_neighbor_index != ent_arr->entity_prevpos[entity_index]) ){
             
            
                //checks that the node is not saturated AND that it isn't the previous node of the entity
                nodes_to_arr[cur_in_nta] = cur_neighbor_index; 
                cur_in_nta++;
            }
        }
        
    }

    if(cur_in_nta){//if there is at least one node in the target nodes array

        g->node_saturation_arr[node_from]--; //entity will get out of it's cur node

        uint32_t node_to_index = nodes_to_arr[rand()%cur_in_nta]; //chooses a random node from nodes_to_arr

        g->node_saturation_arr[node_to_index]++; //updates saturation of this node

        ent_arr->entity_prevpos[entity_index] = node_from; //updates position
        ent_arr->entity_curpos[entity_index] = node_to_index;

        ent_arr->moved_entities++;
       
    }

    return RU_OK;
}//not tested; possibly wrong 
//tested; was wronged; seems fixed ;try more stuff 