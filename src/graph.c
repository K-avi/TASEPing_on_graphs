#include "graph.h"
#include "common.h"
#include "misc.h"


static uint8_t init_node( S_NODE * node_ptr ){
    /*
    initialises a non null S_NODE
    */
    if(!node_ptr) return GE_NULL;

    node_ptr->first_neighboor_ref=NULL;
    node_ptr->neighboor_num=0;

    return GE_OK;
}//tested; ok



static uint8_t init_line_rep( S_LINE_REP* line_rep, uint32_t arrline_size){
    /*
    initialises an already allocated line_rep
    */
    if(!line_rep) return G_NULL;


    line_rep->line_arr=(Line*) calloc( arrline_size, sizeof(Line));
    if(!line_rep->line_arr) return G_MALLOC;

    line_rep->blocked_arr= (uint8_t*) calloc(arrline_size, sizeof(uint8_t));
    if(!line_rep->blocked_arr) return G_MALLOC;
    
    line_rep->cur_in=0; 
    line_rep->size=arrline_size;

    return G_OK;
}// tested ; ok

static void free_line_rep( S_LINE_REP * line_rep){
    /*
    frees the content of a S_LINE_REP
    warning : doesn't free the line array itself
    */
    if(!line_rep) return;
    if(line_rep->line_arr) free(line_rep->line_arr);
    if(line_rep->blocked_arr) free(line_rep->blocked_arr);
    
} // tested ok

void print_line_rep( S_LINE_REP * line_rep, FILE * stream){
    /**/
    if(!line_rep){ fprintf(stream," line arr is null\n"); return;}
    
    for(unsigned i=0; i < line_rep->size; i++){
        fprintf(stream,"line,%u:%u\n", line_rep->line_arr[i].node_index, line_rep->blocked_arr[i]);
    }
}

uint8_t init_graph(S_GRAPH *gt, uint32_t arrline_size ,uint32_t nb_nodes, uint32_t curgen, uint64_t saturation_coeff ){
    /*
    initialises a non null graph table ; sets it's entries to default values and initialises 
    it's walker entries
    */
    if(!gt) return G_NULL;

    gt->nb_nodes= nb_nodes; 
    gt->curgen=curgen;
    gt->saturation_coeff = saturation_coeff;
    
    gt->node_arr= (S_NODE*) calloc(nb_nodes, sizeof(S_NODE) );
    if(!gt->node_arr){ report_err("init_graph", G_MALLOC); return G_MALLOC;}

    gt->node_saturation_arr= (uint32_t*) calloc(nb_nodes, sizeof(uint32_t) );
    if(!gt->node_saturation_arr){ report_err("init_graph", G_MALLOC); return G_MALLOC;}

    for(uint32_t i=0; i<nb_nodes; i++){
        uint8_t failure = init_node(&gt->node_arr[i]);
        if(failure){report_err("init_graph", failure);  return failure;}
    }

    gt->line_rep=malloc(sizeof(S_LINE_REP));
    if(!gt->line_rep){report_err("init_graph", G_MALLOC);  return G_MALLOC;}

    uint8_t failure= init_line_rep(gt->line_rep, arrline_size);
    if(failure){report_err("init_graph", failure) ;return failure;}

    return G_OK;
}//tested ok;



void free_graph( S_GRAPH * gt){
    /*
    */
    if(!gt) return;

   
    free(gt->node_arr);
    free_line_rep(gt->line_rep);
    free(gt->line_rep);
    
    return ;
}//tested; ok


uint8_t app_node_graph (S_GRAPH * gt, uint32_t node_index , uint32_t neighboor_num, Line*  first_neighboor_ref){
    /*
    doesn't if node already set to smtg  ; sets the ref of first elem , and neighboor num 
    of a node at index given

    O(1)
    */
    if(!gt) { report_err("app_node_graph", G_NULL); return G_NULL;}

    if( (gt->nb_nodes-1 < node_index) ){ report_err("app_node_graph", G_INDEX); return G_INDEX;}

    gt->node_arr[node_index].first_neighboor_ref=first_neighboor_ref;
    gt->node_arr[node_index].neighboor_num=neighboor_num;

    return G_OK;
    
}//tested ; ok


uint8_t app_line_graph( S_GRAPH * gt , uint32_t node_index){
    /*
    appends ONE line to the line tab of a graph table ;
    checks for valid node 

    O(1)
    */
    if(!gt){ report_err("app_line_graph", G_NULL); return G_NULL;}
    if(!gt->line_rep){ report_err("app_line_graph", G_NULL); return G_NULL;}
    if(node_index> gt->nb_nodes) { report_err("app_line_graph", G_INDEX); return G_INDEX;}
    if(gt->line_rep->cur_in==gt->line_rep->size){ { report_err("app_line_graph", G_ARFULL); return G_ARFULL;}}

    S_LINE_REP* arrline = gt->line_rep;

    arrline->line_arr[arrline->cur_in].node_index=node_index; 
  
    arrline->cur_in++;

    return G_OK;
}//tested  ; ok

//////////////

uint8_t print_graph(S_GRAPH * gt, FILE * stream){
    /*
    prints the graph of a GraphTab in a adjacency list format 
    probably faster to build one big string and then print it only once instead of calling print 
    every time 
    */
    if(!gt){ report_err("print_graph", G_NULL); return G_NULL;}
    if(!gt->node_arr) { report_err("print_graph", G_NULL); return G_NULL;}
 
    fprintf(stream, "%u,%u\n", gt->nb_nodes, gt->line_rep->size);
    
    for(uint32_t i=0; i<gt->nb_nodes; i++){
        fprintf(stream, "%u,%u,",i ,gt->node_arr[i].neighboor_num);
        for(uint16_t j=0; j<gt->node_arr[i].neighboor_num; j++){
            if(gt->node_arr->first_neighboor_ref){
                if(j!=gt->node_arr[i].neighboor_num-1) {

                    fprintf(stream, "%u;", (gt->node_arr[i].first_neighboor_ref+j)->node_index  );
         
                }else{
                    fprintf(stream, "%u", (gt->node_arr[i].first_neighboor_ref+j)->node_index); 
                } 
            }
        }
        fprintf(stream, "\n");
    }
    return G_OK;
}// tested; seems ok


static  bool peek(const char * str, char expected){
    return *str==expected;
}

static bool emptyLine( char * str){
    while(*str==' ' || *str=='\r') str++;
    return (*str=='\0' || *str=='\n');
}

uint8_t load_graph( S_GRAPH *gt, char *path, uint32_t curgen, uint64_t saturation_coeff){
    /*
    takes a non initialised ; non null , empty gt and loads a graph stored at path into it
    also pass walk table entry size parameter cuz it's better looking than to init w a global var

    if there is more line than space for the graph (i.e : the size of the graph is actually inferior to)
    the number of lines) the lines after the limit of the graph will be ignored

    warray coeff is a double used to determine the nb of walkers in the graph; 
    it is multiplied by the nb of nodes in the graph to calculate the nb of walkers.

    on a graph with 1000 nodes , if warray coeff is 1 there will be 1000 walkers; 
    if it's 0.8 there will be 800 and so on 
    if its 3 there will be 3000...
    */
    if(!gt){ report_err("load_graph", G_NULL); return G_NULL;}

    FILE * f = fopen(path, "r");
    if(!f) { report_err("load_graph", G_READFAIL); return G_READFAIL;};

    char line[262144];
    memset(line, 0, 262144);

    if(!fgets(line, 262144, f)) {//consume first line to retrieve size of graph and size of arrline
        fclose(f);
        report_err("load_graph", G_NOREAD); 
        return G_NULL;
    }

    char * end1,*cur1=line;
    uint32_t  nb_nodes = (uint32_t) strtol(cur1, &end1, 10);
    if(peek(end1, ',') && cur1!=end1) cur1=(++end1);
    else{ fclose(f);  report_err("load_graph parse0", G_PARSE); return G_PARSE; }
    
    uint32_t arrline_size = (uint32_t) strtol(cur1, &end1, 10);
   
    if(cur1!=end1) cur1=(++end1);
    else{fclose(f); report_err("load_graph parse1", G_PARSE); return G_PARSE;}

    //initialises graph with values consumed on the first line
    uint8_t failure= init_graph(gt, arrline_size,  nb_nodes, curgen, saturation_coeff);
    if(failure) { fclose(f);  report_err("load_graph" ,failure); return failure;}

    uint32_t cpt=0; 
    //keep track of nblines consumed ; report error if nbline consumed higher than counter
    
    while(fgets(line, 262144, f) && cpt<nb_nodes){
        char* end, *cur=line;
        if(emptyLine(cur)) continue; // ignores empty lines
        cpt++; 

        uint32_t node_index = (uint32_t) strtol(cur, &end, 10); //retrieves index
        if(peek(end, ',') && cur!=end) cur=(++end);
        else{  fclose(f);  report_err("load_graph parse2", G_PARSE);return G_PARSE;}

        uint32_t neighboor_num = (uint32_t) strtol(cur, &end, 10); //retrieves neighboor num
        if(peek(end, ',') && cur!=end) cur=(++end);
        else {  fclose(f);  report_err("load_graph parse3", G_PARSE); return G_PARSE;}

        //appends node to the node section
        uint8_t errflag= app_node_graph(gt, node_index, neighboor_num, &(gt->line_rep->line_arr[gt->line_rep->cur_in]));
        if(errflag) return errflag; 
        
        //loop to retrieve the formated neighboors 
        for(uint32_t i=0; i<neighboor_num; i++){
            /*
            adds the lines
            */
          
            uint32_t new_neighboor= (uint32_t) strtol(cur, &end, 10);
            if(peek(end, ':') && cur!=end) cur=(++end);    
            else {
                fclose(f);
                fprintf(stderr,"at line :%s\n", line);
                report_err("load_graph parse4", G_PARSE);
                return G_PARSE;
            }

           strtol(cur, &end, 10);
            if(i!=neighboor_num-1){
                if(peek(end, ';') && cur!=end ) cur=(++end);
                else {
                    fclose(f); 
                    fprintf(stderr,"at line :%s\n", line);
                    report_err("load_graph parse5", G_PARSE);
                    return G_PARSE;
                }
            }else{
                if( cur==end){
                    fclose(f);
                    fprintf(stderr,"at line :%s\n", line);
                    report_err("load_graph parse6", G_PARSE);
                    return G_PARSE; 
                }
            }

            uint8_t errflag_in = app_line_graph(gt, new_neighboor);
            if(errflag_in ) return errflag_in;
            
        }
        memset(line, 0, 262144);
    }  
    fclose(f);
    return G_OK;
}//tested; seems ok ; ugly 
//WARNING : initialises both flux_cur and flux_next to the flux value read; it might be a problem 
//if the fn is used wrong ; I might change format to path 2 flux values

uint8_t write_graph(S_GRAPH * gt, char *path ){
    /*
    more of a wrapper on the graph print function than anything else tbh 
    security flaws ; not sure they are really relevant though
    */
    FILE * f = fopen(path, "w");
    if(!f){
         report_err("write_graph parse0", G_OPENFAIL);
        return G_OPENFAIL;
    }

    uint8_t failure= print_graph(gt, f);
    fclose(f);
    if(failure) report_err("write_graph",failure);
    return failure;
}//tested; ok


