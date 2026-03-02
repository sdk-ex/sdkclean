/**/

#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

// helper for holding state of commands (flags)
// all fields/properties are set to 0 on init
typedef struct {

    // any additional states for features should be 
    // added to this struct
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
  * 
  * Note:
  * this line is very important for the time being, but perhaps not as 
  * robust as it could be, due to argument restrictions 
***/
int parse_args(int argc, char* argv[], Appconfig* config);

#endif
