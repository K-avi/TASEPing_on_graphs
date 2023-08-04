//I took the WOG common.h hehehe
#ifndef COMMON_H
#define COMMON_H

#define debug_mode

typedef enum {
    FG_OK, G_NULL, G_MALLOC, G_REALLOC, G_READ, G_WRITE , G_INDEX, G_NOREAD, G_READFAIL,
    G_PARSE, G_NOWRITE , G_OPENFAIL, ERRFLAG_NOFILE,  GE_NULL, GE_MALLOC, GE_REALLOC, 
    G_ARFULL, ET_NULL, ET_MALLOC
}ErrFlag;

#define G_OK FG_OK //g stands for graph
#define GE_OK FG_OK //graph entry 
#define LARR_OK FG_OK //line array
#define ET_OK FG_OK //entities arr
#define R_OK FG_OK //rule ok 

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //not necessary ?
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <unistd.h> //windows compatibility is overrated anyways

#endif 
