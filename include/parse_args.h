/**/

#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

// helper for holding state of flags
typedef struct {

    int clean_apt;
    int clean_trash;
    char *trash_path;   
    int journal_maint;
    
    char** targets;   
    int target_count;

    int dryrun;
    int ask;
    
} Appconfig;

/***
  * parses argc/argv and populates the config struct ... 
  * returns STATUS_OK const if successfull, STATUS_ERR if help is shown,
  * or if error occurs.
***/
int parse_args(int argc, char* argv[], Appconfig* config);

#endif
