#include "attack.h"
#include "graph.h"
#include "misc.h"
#include "common.h"
#include <stdint.h>
#include <stdio.h>

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
}//tested; ok

void free_attack_rep( S_ATTACK_REP * arep){
    /**/
    if(!arep) return;
    
    if(arep->arr_attack_array) free(arep->arr_attack_array);
    if(arep->arr_start_of_attack_ite) free(arep->arr_start_of_attack_ite);
    if(arep->arr_start_attacks) free(arep->arr_start_attacks);
}//tested; ok 

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
}//tested ; based on graph fn; prolly ok


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

    return AREP_OK;
}//tested  ; prolly ok
//based on app_line function 
//doesn't check that line_index is a valid index ; will check 
//in the load function

uint8_t update_attacked_links(S_GRAPH * g, S_ATTACK_REP * arep){
    /*updates the attacked links attribute of g with the next arep attack */
    if(!g){report_err("updated_attacked_links", AREP_NULL); return AREP_NULL; }
    if(!arep){report_err("update_attacked_links", AREP_NULL); return AREP_NULL; }

    S_ATTACK_START attack_start_infos = arep->arr_start_attacks[arep->cur_attack];

    memset(g->line_rep->blocked_arr, 0, g->line_rep->size * sizeof(uint8_t)); //resets every line attack attribute

    for(uint32_t i = 0; i<attack_start_infos.nb_lines_attacked; i++){
        g->line_rep->blocked_arr[attack_start_infos.ref_first_line_attacked[i]] = 1; //sets the new attacked line attribute to 1
    }

    arep->cur_attack++; 
    return AREP_OK;
}//seems ok idk

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

    char * end1,*cur1=line;
    uint32_t  nb_attack_start = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(source);  report_err("load_graph parse1", AREP_PARSE); return AREP_PARSE; }
    
    uint32_t tot_nb_attacked_lines = (uint32_t) strtol(cur1, &end1, 10);
   
    if(cur1!=end1) cur1=(++end1);
    else{fclose(source); report_err("load_attack_rep parse2", AREP_PARSE); return AREP_PARSE;}

    //initialises graph with values consumed on the first line
    uint8_t failure= init_attack_rep(arep, tot_nb_attacked_lines, nb_attack_start);
    if(failure) { fclose(source);  report_err("load_attack_rep" ,failure); return failure;}

    uint32_t cpt=0; 
    uint32_t index_arr_start_attack = 0 ;
    uint32_t index_arr_attacked_lines = 0; 
    //keep track of nblines consumed ; report error if nbline consumed higher than counter

    while (fgets(line, 65536,  source) && cpt < nb_attack_start) {
        //first cut to retrieve the huuuh the size of the array thing ; 
        //then get the line indexes in an array then append this array to the arep 
        char* end, *cur=line;
        if(emptyLine(cur)) continue; // ignores empty lines
        
        cpt++;  

        uint32_t nb_lines_attacked = (uint32_t) strtol(cur, &end, 10); //retrieves neighboor num
        if(peek(end, ',') && cur!=end) cur=(++end);
        else {  fclose(source);  report_err("load_attack_rep parse3", AREP_PARSE); return AREP_PARSE;}

        //appends node to the node section
        uint8_t errflag= app_attack_start_arep(arep, index_arr_start_attack, nb_lines_attacked, &(arep->arr_attack_array[index_arr_attacked_lines]) );
      
        if(errflag) { report_err("load_attack_rep parse4", errflag); return  errflag; }

        //loop to retrieve the indexes of the attacked lines 
        //checks that the line exist
        for(uint32_t i=0; i<nb_lines_attacked; i++){
            /*
            adds the lines
            */
            uint32_t line_attacked= (uint32_t) strtol(cur, &end, 10);

            if(i!=nb_lines_attacked-1){
                if(peek(end, ';') && cur!=end ) cur=(++end);
                else {
                    fclose(source); 
                    fprintf(stderr,"at line :%s %s\n", line, cur);
                    report_err("load_attack_rep parse5", AREP_PARSE);
                    return AREP_PARSE;
                }
            }else{
                if( cur==end){
                    fclose(source);
                    fprintf(stderr,"at line :%s\n", line);
                    report_err("load_attack_rep parse6", AREP_PARSE);
                    return AREP_PARSE; 
                }
            }

            uint8_t errflag_in = app_attacked_line_arep(arep, index_arr_attacked_lines++, line_attacked);
            if(errflag_in ){report_err("load_attack_rep parse7", errflag_in); return errflag_in;}
            
        }
        index_arr_start_attack++;
    }

    fclose(source);
    return AREP_OK; 
}//tested; seems ok

uint8_t print_attack_rep(S_ATTACK_REP * arep, FILE * stream){
    /* wrapper around the write function*/
   /*self explainatory*/
    if(!arep){report_err("write_attack_rep", AREP_NULL); return AREP_NULL;}
    if(!stream){report_err("write_attack_rep", ERR_NULL); return ERR_NULL;}
    fprintf(stream, "%u,%u\n", arep->size_start_arrays, arep->size_attack_array);
    for(uint32_t i = 0 ; i < arep->size_start_arrays; i++){

        fprintf(stream, "%u,", arep->arr_start_attacks[i].nb_lines_attacked);
        for(uint32_t j = 0 ; j < arep->arr_start_attacks[i].nb_lines_attacked; j++){
            if(j == arep->arr_start_attacks[i].nb_lines_attacked -1 ){
                fprintf(stream, "%u", arep->arr_start_attacks[i].ref_first_line_attacked[j]);
            }else{
    
                fprintf(stream, "%u;", arep->arr_start_attacks[i].ref_first_line_attacked[j]);
            }
        }
        fprintf(stream, "\n");
    }
  
    return AREP_OK; 
}// tested; seems ok

uint8_t write_attack_rep(S_ATTACK_REP * arep , char * dest_file){
    /*wrapper around the print fn*/
    FILE * f = fopen("dest_file", "w");
    if(!f){report_err("write_attack_rep", ERR_FOPEN ); return ERR_FOPEN;}
    uint8_t failure = print_attack_rep(arep, f);
    fclose(f);

    if(failure){ report_err("write_attack_rep", failure); return failure;}

    return failure; 
}//tested ; ok 


uint8_t check_validity(S_ATTACK_REP * arep , S_GRAPH * g){
    /* 
    checks that the lines in an arep aren't superior to the last 
    line of a graph g 
    */
    if(!arep){report_err("check_validity", AREP_NULL); return AREP_NULL;}
    if(!g){report_err("check_validity", G_NULL); return G_NULL;}

    for(uint32_t i = 0; i < arep->size_attack_array; i++){
        if(arep->arr_attack_array[i] > g->line_rep->cur_in){
            
            report_err("check_validity", AREP_INVALID);
            fprintf(stderr, "line %u at index %u is higher than nb of lines in the graph %u\n", arep->arr_attack_array[i],i,g->line_rep->cur_in);
            return AREP_INVALID;
        }
    }
    return AREP_OK;;
}//not tested

uint8_t set_attack_start( S_ATTACK_REP * arep , uint32_t nb_iterations, uint32_t start ){
    /*
    sets the start of each attack to match a certain number of iterations 
    the goal is to equally disribute attacks for a sim of nb_ite steps. 

    atm it's very simple and silly ; it just starts an attack every n step where n 
    is nb_ite / nb_attack 
    it can be offset w a start 
    */
    if(!arep){ report_err("set_attack_start", AREP_NULL); return AREP_NULL;}

    uint32_t nb_attacks = arep->size_start_arrays ; 
    uint32_t div = nb_iterations - start / nb_attacks;

    uint64_t sum = start ;

    if(nb_iterations <= nb_attacks){//silly cases but heh 
        for (uint32_t i = 0 ; i < nb_attacks; i++){
            arep->arr_start_of_attack_ite[i] = i ;
        }
    }else{
        for (uint32_t i = 0 ; i < nb_attacks; i++){
            arep->arr_start_of_attack_ite[i] = sum ;
            sum += div; 
        }
    }

    return AREP_OK;
}//not tested
