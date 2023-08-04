//I took the WOG common.h hehehe
#ifndef COMMON_H
#define COMMON_H

#define debug_mode

typedef enum {
    FG_OK, G_NULL, G_MALLOC, G_REALLOC, G_READ, G_WRITE , G_INDEX, G_NOREAD, G_READFAIL,
    G_PARSE, G_NOWRITE , G_OPENFAIL, ERRFLAG_NOFILE,  GE_NULL, GE_MALLOC, GE_REALLOC, 
    G_ARFULL, ET_NULL, ET_MALLOC, AAR_NULL, AAR_MALLOC, AREP_NULL, AREP_MALLOC,
    ERR_NULL, ERR_FOPEN, F_FORMAT, AREP_PARSE,

}ErrFlag;

#define G_OK FG_OK //g stands for graph
#define GE_OK FG_OK //graph entry 
#define LARR_OK FG_OK //line array
#define ET_OK FG_OK //entities arr
#define R_OK FG_OK //rule ok 
#define AAR_OK FG_OK //attack array 
#define AREP_OK FG_OK //attack rep
#define IT_OK FG_OK

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //not necessary ?
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <unistd.h> //all my homies hate windows

#endif 
