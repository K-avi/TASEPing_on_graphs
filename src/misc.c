#include "misc.h"
#include "common.h"

char * str_flag(uint8_t flag){

  switch (flag) {

  case FG_OK : return "how did you get here this flag is supposed to mean everything is okay";
  case G_NULL :return "graphtable is null";
  case G_MALLOC: return "couldn't allocate memory to graphtable"; 
  case G_REALLOC : return "realloc of graphtable failed" ;
  case G_READ : return "couldn't open graphtable file";
  case G_WRITE : return "couldn't write graphtable";
  case G_INDEX : return "warning unused flag";
  case G_NOREAD: return "couldn't retrieve lines from file"; 
  case G_READFAIL: return "couldn't read graphtable from file";
  case G_PARSE: return "parsing of graph file failed check that the file is formatted correctly"; 
  case G_NOWRITE: return "couldn't write to fail for graphtable";
  case G_OPENFAIL: return "couldn't open graphtable file"; 
  case ERRFLAG_NOFILE: return "file doesn't exist";  
  case GE_NULL: return "graph entry is null";
  case GE_MALLOC: return "coulnd't allocate memory to graphentry";  
  case GE_REALLOC:return "realloc of graphentry failed"; 
  case G_ARFULL: return "coulnd't add a node to the graphtable"; 

  case ET_NULL : return "entities arr pointer is NULL";
  case ET_MALLOC : return "couldn't allocate memory for the entity array";

  case AAR_NULL : return "attack array struct pointer is NULL";
  case AAR_MALLOC : return "couldn't allocate memory for the attack array";

  case AREP_NULL :  return "attack rep struct pointer is NULL";
  case AREP_MALLOC : return "couldn't allocate memory for the array of attack array";

  case ERR_NULL: return "NULL pointer caught";
  case ERR_FOPEN: return "can't open file"; 
  case F_FORMAT : return "invalid file format";

  case AREP_PARSE : return "parsing of attack file failed ; please check that the file is formatted correctly";
  case AREP_INDEX : return "index passed is higher than max index";

  case ERRFLAG_INVALIDOPT : return "invalid option passed";
  case ERRFLAG_INVALID_ARG : return "argument passed to program isn't recognised; please check the docs";

  default : return "unknown error ; how did you get here?\n";
  }
}//ok

void report_err( char * msg , uint8_t flag){ //could be a macro 
  fprintf(stderr, "error : %s at %s\n", str_flag(flag), msg);
}

void report_warning( char * msg , uint8_t flag){ //could be a macro 
  fprintf(stderr, "warning : %s at %s\n", str_flag(flag), msg);
}