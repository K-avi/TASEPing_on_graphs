#include "attack.h"
#include "common.h"

#include "entities.h"
#include "graph.h"
#include "misc.h"
#include "movement.h"
#include "rules.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>


int main(int argc , char ** argv){
    

    time_t t;
    time(&t); 

    srand(t);

    int8_t c; 
    uint8_t hflag = 0, saturation_coeff = 1, optcount = 0 ; //add option for the saturation coeff 
    uint32_t first_attack_it = 0; 
    while ((c = getopt(argc, argv, "hs:f:")) != -1) {   
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
        case 'f':
            optcount += 2; 
            first_attack_it = atoi(optarg); 

        break;

        case '?':
            if(optopt=='s'){
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                report_err("main options no opt", ERRFLAG_INVALIDOPT); 
                return ERRFLAG_INVALIDOPT;
            }if(optopt=='f'){
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
        fprintf(stdout, "usage : ./taseping_on_graphs [path of graph]  [path of attack] [output file] iteration_num coeff_entities\n \
        check out the docs directory for more informations\n");
        return 0;
    }

    if(argc < 6){ //checks that the number of args is ok
        fprintf(stderr, "usage : ./taseping_on_graphs [path of graph]  [path of attack] [output file] iteration_num coeff_entities\n \
        check out the docs directory for more informations\n");
        return ERRFLAG_NOFILE;
    }

    //retrieves names from parameters
    char * gpath = argv[1+optcount]; 
    char * attack_path = argv[2+optcount]; 
    char * output_file = argv[3+optcount];

    //parses number of iterations
    char * end = argv[4+optcount];
    uint32_t iteration_num = (uint32_t ) strtol( argv[4+optcount], &end , 10);
 
    if(end== argv[4+optcount]){
        fprintf(stderr, "usage : ./taseping_on_graphs [path of graph]  [path of attack] [output file] iteration_num coeff_entities\n\
        check out the docs directory for more informations\n");
        return ERRFLAG_INVALID_ARG;
    }

    //parses coeff of entities
    end = argv[5+optcount];
    double coeff_ent = (double) strtod( argv[5+optcount], &end );

    if(end== argv[5+optcount]){
        fprintf(stderr, "usage : ./taseping_on_graphs [path of graph]  [path of attack] [output file] iteration_num coeff_entities\n\
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
    
    //printf("nb ett %u\n", (uint32_t)((double)g.nb_nodes*(double)coeff_ent) );
    if(failure){ report_err("in main init_entity_arr", failure); return failure;}

    //checks that the lines in the arep match the lines in the graph
    failure = check_validity(&arep, &g); 
    if(failure){ report_err("in main check_validity", failure); return failure;}

    //sets the iteration where the attack will change 
    failure = set_attack_start(&arep,  iteration_num, first_attack_it );
    if(failure){ report_err("in main check_validity", failure); return failure;}
    
    //iterates for real for real
    failure = iterate_ntimes(&g, &entities, &arep, output_file, iteration_num, 0);
    if(failure){report_err("in main iterate_ntimes", failure); return failure;}

    //frees the memory
    free_graph(&g);
    free_attack_rep(&arep);
    free_entity_arr(&entities);

    return 0 ;
}//not tested