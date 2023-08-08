#include "attack.h"
#include "common.h"

#include "entities.h"
#include "graph.h"
#include "misc.h"
#include "movement.h"
#include "rules.h"
#include <stdint.h>


int main(int argc , char ** argv){
    
    int8_t c; 
    uint8_t hflag = 0, saturation_coeff = 1, optcount = 0 ; //add option for the saturation coeff 
    while ((c = getopt(argc, argv, "hs:")) != -1) {   
        switch (c){
        
        case 'h':
            hflag=1;
        break;
        case 's':
            optcount += 2; 
            saturation_coeff = atoi(optarg); 
            if(saturation_coeff == 0){
                report_err("main options s option parse", ERRFLAG_INVALIDOPT); 
                return ERRFLAG_INVALIDOPT;
            }  
        break;

        case '?':
            if(optopt=='s'){
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                report_err("main options no opt", ERRFLAG_INVALIDOPT); 
                return ERRFLAG_INVALIDOPT;
            }else{
                fprintf(stderr, "Unknown option character `\\x%x'.", optopt);
                exit(ERRFLAG_INVALIDOPT);    
            }
        break;

        default:
            abort();
        }     
    }

    if(hflag){ //prints help
        fprintf(stdout, "usage : ./taseping_on_graphs path/of/graph path/of/attack output/file \
        iteration_num coeff_entities\n \
        check out the docs directory for more informations\n");
        return 0;
    }

    if(argc < 6){ //checks that the number of args is ok
        fprintf(stderr, "usage : ./taseping_on_graphs path/of/graph path/of/attack output/file \
        iteration_num coeff_entities\n\
        check out the docs directory for more informations\n");
        return ERRFLAG_NOFILE;
    }

    char * gpath = argv[1+optcount]; 
    char * attack_path = argv[2+optcount]; 
    char * output_file = argv[3+optcount];

    //parses number of iterations
    char * end = argv[4+optcount];
    uint32_t iteration_num = (uint32_t ) strtol( argv[4+optcount], &end , 10);
 
    if(end== argv[4+optcount]){
        fprintf(stderr, "usage : ./taseping_on_graphs path/of/graph path/of/attack output/file \
        iteration_num coeff_entities\n\
        check out the docs directory for more informations\n");
        return ERRFLAG_INVALID_ARG;
    }

    //parses coeff of entities
    end = argv[5+optcount];
    double coeff_ent = (double) strtod( argv[5+optcount], &end );
 
    if(end== argv[5+optcount]){
        fprintf(stderr, "usage : ./taseping_on_graphs path/of/graph path/of/attack output/file \
        iteration_num coeff_entities\n\
        check out the docs directory for more informations\n");
        return ERRFLAG_INVALID_ARG;
    }

    //inits graph
    S_GRAPH g; 
    uint8_t failure = load_graph(&g, gpath, 0, saturation_coeff);
    if(failure){ report_err("in main load_graph", failure); return failure;}

    //inits attack array
    S_ATTACK_REP arep; 
    failure = load_attack_rep(&arep, attack_path); 
    if(failure){ report_err("in main load_attack_rep", failure); return failure;}

    //inits entities
    S_ENT_ARR entities;
    failure = init_entity_arr(&entities, (uint32_t)((double)g.nb_nodes*(double)coeff_ent));
    
    iterate_ntimes(&g, &entities, &arep, output_file, iteration_num, 0);

    free_graph(&g);
    free_attack_rep(&arep);
    free_entity_arr(&entities);

    return 0 ;
}//not tested