#include "attack.h"
#include "graph.h"
#include "misc.h"
#include "common.h"
#include <bits/types/FILE.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


uint8_t init_attack_array(S_ATTACK_ARRAY * arr , uint32_t array_size){
    /**/
    if(!arr){report_err("init_attack_array", AAR_NULL); return AAR_NULL;}

    arr->arr_attacked_lines = (uint32_t*) calloc(array_size, sizeof(uint32_t));
    if(!arr->arr_attacked_lines){report_err("init_attack_array", AAR_MALLOC); return AAR_MALLOC;}


    arr->array_size = array_size;
    return AAR_OK;
}//not tested ; ok


void free_attack_array( S_ATTACK_ARRAY * arr){
    /**/
    if(!arr) return ;
    if(arr->arr_attacked_lines) free(arr->arr_attacked_lines);
}//not tested; ok 

uint8_t init_attack_rep( S_ATTACK_REP * attr, uint32_t array_size){
    /**/
    if(!attr){ report_err("init_attack_rep", AREP_NULL); return AREP_NULL;}
    
    attr->arr_attack_array = (S_ATTACK_ARRAY*) calloc( array_size, sizeof(S_ATTACK_ARRAY));
    if(!attr->arr_attack_array){ report_err("init_attack_rep", AREP_MALLOC); return AREP_MALLOC;}

    attr->arr_attack_ite = (uint32_t*) calloc(array_size, sizeof(uint32_t));
    if(!attr->arr_attack_ite){report_err("init_attack_array", AAR_MALLOC); return AAR_MALLOC;}

    attr->array_size = array_size;
    attr->cur_attack = 0;

    return AREP_OK;
}//not tested; ok

void free_attack_rep( S_ATTACK_REP * attr){
    /**/

    if(!attr) return;
    if(attr->arr_attack_array){
        for(uint32_t i = 0 ; i< attr->array_size; i++){
            free_attack_array(&attr->arr_attack_array[i]);
        }
        free(attr->arr_attack_array);
    }
    if(attr->arr_attack_ite) free(attr->arr_attack_ite);
}//not tested; ok 


uint8_t app_attack_rep(S_ATTACK_REP * attr , uint32_t array_size , uint32_t * arr_attacked_lines, uint32_t iteration_attack, uint32_t index){
    /**/
    if(!attr){ report_err("app_attack_rep", AREP_NULL); return AREP_NULL; }
    if(!arr_attacked_lines){ report_err("app_attack_rep", AAR_NULL); return AAR_NULL;}

    attr->arr_attack_array[index].arr_attacked_lines = arr_attacked_lines;
    attr->arr_attack_array[index].array_size = array_size;

    attr->arr_attack_ite[index] = index;

    return AREP_OK;
}//not tested; might be wrong i dunno

uint8_t update_attacked_links(S_GRAPH * g, S_ATTACK_REP * arep){
    /*updates the attacked links attribute of g with the next arep attack */
    if(!g){report_err("updated_attacked_links", G_NULL); return G_NULL; }
    if(!arep){report_err("update_attacked_links", AREP_NULL); return AREP_NULL; }

    S_ATTACK_ARRAY cur_arr_attack = arep->arr_attack_array[arep->cur_attack];

    memset(g->line_rep->blocked_arr, 0, g->line_rep->size * sizeof(uint8_t)); //resets every line attack attribute

    for(uint32_t i = 0; i<cur_arr_attack.array_size; i++){
        g->line_rep->blocked_arr[cur_arr_attack.arr_attacked_lines[i]] = 1; //sets the new attacked line attribute to 1
    }

    arep->cur_attack++; 
    return AREP_OK;
}//not done


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

    //parse the expected number of attacks 
    char * end1,*cur1=line;
    uint32_t  nb_attacks = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(source);  report_err("load_attack_arep parse1", AREP_PARSE); return AREP_PARSE; }    

    init_attack_rep(arep, nb_attacks );

    uint32_t cpt = 0 ; //check that the nb of attacks is the nb of lines in the file 

    while (fgets(line, 65536,  source) && cpt < nb_attacks) {
        //first cut to retrieve the huuuh the size of the array thing ; 
        //then get the line indexes in an array then append this array to the arep 
        cpt++;
    }

    fclose(source);
    return AREP_OK; 
}//not done

uint8_t write_attack_rep(S_ATTACK_REP * arep , char * dest_file){
    /*self explainatory*/

    return AREP_OK;
}//not done

