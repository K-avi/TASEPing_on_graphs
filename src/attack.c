#include "attack.h"
#include "graph.h"
#include "misc.h"
#include "common.h"
#include <bits/types/FILE.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>




uint8_t init_attack_rep( S_ATTACK_REP * arep, uint32_t attack_array_size, uint32_t start_array_size){
    /**/
    if(!arep){ report_err("init_attack_rep", AREP_NULL); return AREP_NULL;}
    
    arep->arr_attack_array = (uint32_t*) calloc( attack_array_size ,sizeof(uint32_t));
    if(!arep->arr_attack_array){report_err("init_attack_rep", AREP_MALLOC); return AREP_MALLOC;}

    arep->arr_start_attacks = (S_ATTACK_START*) calloc(start_array_size,  sizeof(S_ATTACK_START));
    if(!arep->arr_start_attacks){report_err("init_attack_rep", AREP_MALLOC); return AREP_MALLOC;}

    arep->arr_start_of_attack_ite = (uint32_t*) calloc(start_array_size,  sizeof(uint32_t));
    if(!arep->arr_start_of_attack_ite){report_err("init_attack_rep", AREP_MALLOC); return AREP_MALLOC;}

    arep->size_attack_array = attack_array_size; 
    arep->size_start_arrays = start_array_size;
    arep->cur_attack = 0;

    return AREP_OK;
}//not tested; ok

void free_attack_rep( S_ATTACK_REP * arep){
    /**/

    if(!arep) return;
    
    if(arep->arr_attack_array) free(arep->arr_attack_array);
    if(arep->arr_start_of_attack_ite) free(arep->arr_start_of_attack_ite);
    if(arep->arr_start_attacks) free(arep->arr_start_attacks);
}//not tested; ok 

uint8_t app_attack_start_arep (S_ATTACK_REP * arep, uint32_t start_attack_index , uint32_t nb_lines_attacked, uint32_t *  first_line_ref){
    /*
    doesn't if node already set to smtg  ; sets the ref of first elem , and neighboor num 
    of a node at index given

    O(1)
    */
    if(!arep) { report_err("app_attack_start_arep", AREP_NULL); return AREP_NULL;}

    if( (arep->size_start_arrays - 1 < start_attack_index) ){ report_err("app_attack_start_arep", AREP_INDEX); return AREP_INDEX;}

    arep->arr_start_attacks[start_attack_index].ref_first_line_attacked = first_line_ref;
    arep->arr_start_attacks[start_attack_index].nb_lines_attacked = nb_lines_attacked;

    return AREP_OK;
    
}//not tested ; based on graph fn; prolly ok


uint8_t app_attacked_line_arep( S_ATTACK_REP * arep , uint32_t array_index ,uint32_t line_index){
    /*
    appends ONE line to the line tab of a graph table ;
    checks for valid node 

    O(1)
    */
    if(!arep){ report_err("app_attacked_line_arep", AREP_NULL); return AREP_NULL;}
    if(!arep->arr_attack_array){ report_err("app_attacked_line_arep", AREP_NULL); return AREP_NULL;}
    if( array_index > arep->size_attack_array - 1 ) { report_err("app_attacked_line_arep", AREP_INDEX); return AREP_INDEX;}
   
    arep->arr_attack_array[array_index] = line_index;

    return G_OK;
}//not tested  ; prolly ok
//based on app_line function 
//doesn't check that line_index is a valid index ; will check 
//in the load function

uint8_t update_attacked_links(S_GRAPH * g, S_ATTACK_REP * arep){
    /*updates the attacked links attribute of g with the next arep attack */
    if(!g){report_err("updated_attacked_links", G_NULL); return G_NULL; }
    if(!arep){report_err("update_attacked_links", AREP_NULL); return AREP_NULL; }

    S_ATTACK_START attack_start_infos = arep->arr_start_attacks[arep->cur_attack];

    memset(g->line_rep->blocked_arr, 0, g->line_rep->size * sizeof(uint8_t)); //resets every line attack attribute

    for(uint32_t i = 0; i<attack_start_infos.nb_lines_attacked; i++){
        g->line_rep->blocked_arr[attack_start_infos.ref_first_line_attacked[i]] = 1; //sets the new attacked line attribute to 1
    }

    arep->cur_attack++; 
    return AREP_OK;
}//not tested; might be wrong 


static  bool peek(const char * str, char expected){
    return *str==expected;
}

static bool emptyLine( char * str){
    while(*str==' ' || *str=='\r') str++;
    return (*str=='\0' || *str=='\n');
}

uint8_t load_attack_rep(S_ATTACK_REP * arep , char * source_file){
    /*
    loads an attack rep from a custom csv source file representing an 
    attack; will report a bunch of errors if it's wrong 


    very similar to the graph parse function 
    */
    if(!arep){ report_err("load_attack_arep",  AREP_NULL); return AREP_NULL;}
    if(!source_file){ report_err("load_attack_arep", ERR_NULL); return ERR_NULL;}

    FILE * source = fopen(source_file, "r");
    if(!source){ report_err("load_attack_arep", ERR_FOPEN); return ERR_FOPEN;}

    char line[65536];
    memset(line, 0,  65536);

    if(!fgets(line, 65536,  source)){
        fclose(source);
        report_err("load_attack_arep  parse0 ", AREP_PARSE); 
        return F_FORMAT;
    }

    //parse the expected number of attacks and the size of the sum of the attack array 
    char * end1,*cur1=line;
    uint32_t  start_array_size = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(source);  report_err("load_attack_rep parse1", AREP_PARSE); return AREP_PARSE; }
    
    uint32_t attack_array_size = (uint32_t) strtol(cur1, &end1, 10);
   
    if(cur1!=end1) cur1=(++end1);
    else{fclose(source); report_err("load_attack_arep parse2", AREP_PARSE); return AREP_PARSE;}

    //initialises graph with values consumed on the first line
    uint8_t failure= init_attack_rep(arep, attack_array_size,  start_array_size);
    if(failure) { fclose(source);  report_err("load_graph" ,failure); return failure;}

   
    uint32_t cpt = 0 ; //check that the nb of attacks is the nb of lines in the file 
    uint32_t nb_attacks = attack_array_size;

    while (fgets(line, 65536,  source) && cpt < nb_attacks) {
        //first cut to retrieve the huuuh the size of the array thing ; 
        //then get the line indexes in an array then append this array to the arep 
        char* end, *cur=line;
        if(emptyLine(cur)) continue; // ignores empty lines
        
        cpt++;

        

        
    }

    fclose(source);
    return AREP_OK; 
}//not done

uint8_t write_attack_rep(S_ATTACK_REP * arep , char * dest_file){
    /*self explainatory*/

    return AREP_OK;
}//not done

