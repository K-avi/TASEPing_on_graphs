#ifndef GRAPH_H
#define GRAPH_H 

#include "common.h"




typedef struct s_line{ //line struct 
    uint32_t node_index; //index of b in the line (a,b) stored as 
    //I pass by index  bc theyre smaller than pointers 
    //I know about the whole "dereferecing a pointer vs indexing an array" discourse 
    //I don't know if it's true so idk man
    //especially since the program is small so making it smaller means I can have 
    //that sweet cache speed longer
}Line; 

typedef struct sline_arr{//STATIC ARRAY  
/*the line array stores the adjacency lists of the graphs; 
the idea is to put every adjacency list one after the other 
to access them faster*/

    uint32_t size; //size of the array
    uint32_t cur_in ;//kinda usefull when initialising

    Line * line_arr ; //the main array to store the lines of the graph

    uint8_t * blocked_arr; //boolean arr to know wether a line is "open" or not; if 
    //POSITIVE it's BLOCKED 

}S_LINE_REP;


typedef struct snode_entry{

    Line * first_neighboor_ref;  //will replace by a 32 bit integer 
    //representing index to gain a bit of space 
    uint16_t neighboor_num; //Might store on one byte
}S_NODE;


typedef struct s_graph{ //the main graph structure 


    uint64_t saturation_coeff; //global graph attribute 
    //used to calculate the number of walkers necessary to make a node 
    //saturated. When a node is saturated; entities can not move to it. 

    uint32_t nb_nodes ;
    uint32_t curgen ; //generation of the simulation stored in the table cache and updated at
    // the end of an iteration in the simul

    S_NODE * node_arr; //an array containing the structure storing the nodes of the graph
    //and other stuff

    uint32_t * node_saturation_arr; //node attribute representing how many 
    //entities are on the node currently 

    S_LINE_REP * line_rep ;//a big array containing the adjacency list of each node ; it can also be used to represent 
    //the "lines" of the graph (ie: line attributes array can be indexes the same as this array)

}S_GRAPH ;

uint8_t init_graph( S_GRAPH * gt, uint32_t arrline_size, uint32_t nb_nodes, uint32_t curgen, uint64_t saturation_coeff );
void free_graph( S_GRAPH * gt);

uint8_t print_graph(S_GRAPH * gt, FILE * stream);
uint8_t load_graph( S_GRAPH *gt, char *path, uint32_t curgen, uint64_t saturation_coeff);
uint8_t write_graph(S_GRAPH * gt, char *path );

#ifdef debug_mode 
uint8_t app_line_graph( S_GRAPH * gt , uint32_t node_index);
uint8_t app_node_graph (S_GRAPH * gt, uint32_t node_index , uint32_t neighboor_num, Line*  first_neighboor_ref);

#endif

#endif 