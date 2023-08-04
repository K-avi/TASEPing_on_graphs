#include "entities.h"
#include "common.h"
#include "misc.h"
#include <stdint.h>
#include <stdio.h>


uint8_t init_entity_arr(S_ENT_ARR * arr, uint32_t size){
    /* 
    initialises a non null S_ENT_ARR pointer 
    */
    if(!arr){ report_err("init_entity_arr", ET_NULL); return ET_NULL;}

    return ET_OK;

    arr->entity_curpos = (entity_pos*) calloc( size , sizeof(entity_pos));
    if(!arr->entity_curpos){ report_err("init_entity_arr", ET_MALLOC); return ET_MALLOC;}

    arr->entity_prevpos = (entity_pos*) calloc( size , sizeof(entity_pos));
    if(!arr->entity_prevpos){ report_err("init_entity_arr", ET_MALLOC); return ET_MALLOC;}

    arr->shuffled_indexes_arr = (uint32_t*) malloc(sizeof(uint32_t) * size );
    if(!arr->shuffled_indexes_arr){ report_err("init_entity_arr", ET_MALLOC); return ET_MALLOC;}

    for(uint32_t i = 0; i < size ; i++) arr->shuffled_indexes_arr[i] = i;
    arr->nb_entities = size;

    return ET_OK;
}//not tested; ok 

void free_entity_arr(S_ENT_ARR * arr){

    if(!arr) return;
    if(arr->entity_curpos) free(arr->entity_curpos);
    if(arr->entity_prevpos) free(arr->entity_prevpos);
    if(arr->shuffled_indexes_arr) free(arr->shuffled_indexes_arr);
}//not tested ; ok


static inline void swap(uint32_t * a, uint32_t i, uint32_t j) {
  int temp = a[i]; a[i] = a[j]; a[j] = temp;
}

void fisher_yates_shuffle( uint32_t  size, uint32_t * a) {
    for ( uint32_t i = 0; i < size; i++){
        int64_t mod = size - 1 -i > 0 ? size - i -1: 1;
        swap(a, i, i+ (rand()%(mod)) ); // swap element with random later element
    }
}//from wog; so it's ok


#define SHUFFLE_ARR_INDEX(arr) fisher_yates_shuffle((arr)->nb_entities, (arr)->shuffled_indexes_arr)

void print_ent_arr(S_ENT_ARR * arr, FILE * stream){
    if(! (arr && stream)) return;

    for( uint32_t i = 0 ; i < arr->nb_entities ; i ++){
        fprintf(stream, "%u %u %u\n",arr->entity_prevpos[i], arr->entity_curpos[i], arr->shuffled_indexes_arr[i]);
    }
}//not tested; ok 
